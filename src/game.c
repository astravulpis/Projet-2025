#include "game.h"
#include "bars.h"
#include "common.h"
#include "entity.h"
#include "event.h"
#include "file_parsing.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "triggers.h"

void closeGame(gameContext *ctx)
{
    if (ctx->levels != NULL) {
        for (size_t i = 0; i < ctx->level_count; ++i) {
            destroyLevel(&ctx->levels[i]);
        }
    }
    free(ctx->levels);
    ctx->levels = NULL;

    if (ctx->menus != NULL) {
        for (menu_kind kind = 0; kind < __menu_count; ++kind) {
            if (ctx->menus[kind] != NULL) {
                destroyMenu(&ctx->menus[kind]);
            }
        }
    }
    free(ctx->menus);
    ctx->menus = NULL;

    destroyGuns(&ctx->guns);
    deleteBullets(&ctx->bullet_arr);
    destroyPlayer(&ctx->player);
    closeCtx(&ctx->sdl_ctx);
}

level_t *getLoadedLevel(gameContext *ctx)
{
    return ctx->levels[ctx->loadedLevelIdx];
}

room_t *beginLevel(int argc, char **argv, gameContext *ctx)
{
    level_t *level = NULL;
    room_t *curr = NULL;
    parseFlag(argc, argv, &ctx->sdl_ctx, &level);
    assert(level != NULL && "Parse flag didn't work");

    if (ctx->levels == NULL) {
        ctx->levels = malloc(sizeof(level_t *) * LEVEL_COUNT);
        if (ctx->levels == NULL) {
            nob_log(ERROR, "Failed to allocate space for %d levels", LEVEL_COUNT);
            return NULL;
        }
        memset(ctx->levels, 0, sizeof(level_t *) * LEVEL_COUNT);
    }

    ctx->levels[ctx->level_count++] = level;
    ctx->loadedLevelIdx = level->levelID;

    curr = getLoadedRoom(level);
    movePlayer(ctx->player, (V2f){curr->startPos.x * ctx->sdl_ctx->screenRatio, curr->startPos.y * ctx->sdl_ctx->screenRatio});
    return curr;
}

bool loadAllLevels(gameContext *ctx)
{
    char *paths[5] = {"levelA","levelB","levelC","levelD","levelE"};

    for (int i = 1; i < 5; i++) {
        level_t *level = NULL;
        parseFile(paths[i], &ctx->sdl_ctx, &level);
        //assert(level != NULL && "Parse file didn't work");

        ctx->levels[i] = level;
    }

    return true;
}

bool addMenu(gameContext *ctx, gui_menu *menu, menu_kind kind)
{
    if (ctx->menus == NULL) {
        // Menus:
        //   - Pause
        //   - Start
        //   - Option
        //   - Level selection
        //   - "footer"
        assert(__menu_count == 5 && "Amount of menu changed");
        ctx->menus = malloc(sizeof(gui_menu *) * __menu_count); // At best, we'll have 5 menus

        if (ctx->menus == NULL) {
            nob_log(ERROR, "Failed to allocate space for %d menus", __menu_count);
            return false;
        }
        memset(ctx->menus, 0, sizeof(gui_menu *) * __menu_count);
    }

    ctx->menus[kind] = menu;
    ctx->menu_count++;
    return true;
}

bool gameLoop(gameContext *ctx, int argc, char **argv)
{
    size_t mark = temp_save();
    bool needsFpsCap = false;
    int frameCounter = 0;
    float deltaTime = 0;
    int frameRate = 0;

    Uint32 frameStart = 0;
    Uint32 last = SDL_GetTicks();

    int mouseInputFlag = 0;
    int prevMouseInput = 0;
    V2f mouseCoord = {0};

    // 0st place in taking a lot of time -> Loads every SDL contexts (i.e. SDL, SDL Image, ttf, mixer)
    if (!createCtx(&ctx->sdl_ctx)) return false; // Error handling is done in the function
    SDL_GetMouseState(&mouseCoord.x, &mouseCoord.y);
    disableVsync(ctx->sdl_ctx);

    // 1st place in taking a lot of time -> Loads the player
    if (!createPlayer(&ctx->player, (V2f){100, 120}, &ctx->sdl_ctx)) return 1;

    SDL_FRect footerBox = {0, (WINDOW_HEIGHT - 150.f), WINDOW_WIDTH, 150.f};
    boxToScale(&footerBox, ctx->sdl_ctx->screenRatio);

    SDL_FRect cWeaponBox = {(WINDOW_WIDTH / 2.f - 300.f), (WINDOW_HEIGHT - 135.f), 600.f, 120.f};
    boxToScale(&cWeaponBox, ctx->sdl_ctx->screenRatio);

    SDL_FRect styleMeterBox = {(WINDOW_WIDTH - 265.f), (WINDOW_HEIGHT - 135.f), 250.f, 120.f};
    boxToScale(&styleMeterBox, ctx->sdl_ctx->screenRatio);

    bar *hpBar = NULL;
    bar *dashBar1 = NULL;
    bar *dashBar2 = NULL;
    bar *dashBar3 = NULL;

    createPlayerStatusBar(ctx->sdl_ctx, &dashBar1, &dashBar2, &dashBar3, &hpBar);
    room_t *currRoom = NULL;
    // 2rd place in taking a lot of time -> parse the file and loads the level
    if ((currRoom = beginLevel(argc, argv, ctx)) == NULL) return false;
    level_t *currLevel = getLoadedLevel(ctx);

    if (!loadAllLevels(ctx)) return false;

    if (!addMenu(ctx, createPauseMenu(ctx->sdl_ctx), PAUSE_MENU)) return false;
    if (!addMenu(ctx, createOptionsMenu(ctx->sdl_ctx), OPTIONS_MENU)) return false;
    if (!addMenu(ctx, createHomeMenu(ctx->sdl_ctx), START_MENU)) return false;
    if (!addMenu(ctx, createLevelMenu(ctx->sdl_ctx), LEVEL_SELECTION_MENU)) return false;

    // 3rd place in taking a lot of time -> idk
    ctx->guns = initialiseGuns(ctx->sdl_ctx);
    playTrack(ctx->sdl_ctx, BACKGROUND_MUSIC);

    while (!ctx->sdl_ctx->quit) {
        temp_rewind(mark);
        SDL_SetRenderDrawColor(ctx->sdl_ctx->renderer, 0x00, 0x00, 0x00, 0xFF);

        Uint32 now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f; // seconds since last frame
        last = now;

        if (now - frameStart >= 1000) {
            frameRate = frameCounter;
            frameCounter = 0;
            frameStart = now;
        }
        if (frameRate > 60) needsFpsCap = true;

        while (SDL_PollEvent(&ctx->sdl_ctx->event)) {
            switch (ctx->sdl_ctx->event.type) {
            case SDL_EVENT_QUIT:
                ctx->sdl_ctx->quit = true;
                break;
            }
        }
        SDL_PumpEvents();

        mouseInputFlag = SDL_GetMouseState(&mouseCoord.x, &mouseCoord.y);
        basicKeyboardEvents(ctx->sdl_ctx, currLevel, ctx->player, ctx->guns);

        SDL_RenderClear(ctx->sdl_ctx->renderer);
        renderBackground(ctx->sdl_ctx);
        renderRoom(ctx->sdl_ctx, currLevel);
        da_foreach (trigger_t *, trigger, &getLoadedRoom(currLevel)->triggers) {
            renderFillRect(ctx->sdl_ctx->renderer, (*trigger)->boundingBox, (SDL_Color){0xFF, 0x7F, 0x7f, 0x7F});
        }

        if (!ctx->sdl_ctx->currMenu &&
            (mouseInputFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT) && !(prevMouseInput & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)))) {
            V2f startingPos = (V2f){getBB(ctx->player)->x + getBB(ctx->player)->w / 2.0f,
                                    getBB(ctx->player)->y + getBB(ctx->player)->h / 2.0f};
            V2f deltaPos = (V2f){mouseCoord.x - startingPos.x, mouseCoord.y - startingPos.y - 15.0f};
            float magnitude = SDL_sqrt((deltaPos.x * deltaPos.x) + (deltaPos.y * deltaPos.y));
            V2f vel = (V2f){((deltaPos.x / magnitude) * 2500), ((deltaPos.y / magnitude) * 2500)};

            shootGun(ctx->sdl_ctx, &ctx->guns->arsenal[ctx->guns->selectedGun], &ctx->bullet_arr, startingPos, vel);
        }
        // else if (mouseInputFlag & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT) && !(prevMouseInput & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)))
        // {
        //     printf("Right Clicked\n");
        // }

        renderBullets(ctx->sdl_ctx, &ctx->bullet_arr);
        prevMouseInput = mouseInputFlag;

        if (!ctx->sdl_ctx->currMenu) {
            updateTriggers(currLevel, ctx->player);
            updateBulletState(&ctx->bullet_arr, currLevel, deltaTime, ctx->player);
            updateEntities(getCurrentEntityWave(currLevel), ctx->player, getRoomObjects(currLevel), deltaTime);
            updatePlayer(ctx->player, getRoomObjects(currLevel), deltaTime);
        }

        renderPlayer(ctx->player);
        renderEntities(getCurrentEntityWave(currLevel));

        renderText_Ex(ctx->sdl_ctx, temp_sprintf("fps : %d", frameRate), BLACK, (V2f){10.0f, 10.0f});

        // Everything after the footer being rendered is rendered OVER it.
        renderPlayerStatusBar(ctx->sdl_ctx, ctx->player, dashBar1, dashBar2, dashBar3, hpBar);

        // Update and render the menu at the very end
        switch (ctx->sdl_ctx->currMenu) {
        case NONE_MENU:
            break;
        case PAUSE_MENU: {
            updateMenu(ctx->sdl_ctx, mouseCoord, mouseInputFlag, ctx->menus[ctx->sdl_ctx->currMenu], updatePauseMenu, NULL);
            renderMenu(ctx->sdl_ctx, ctx->menus[ctx->sdl_ctx->currMenu]);
        } break;
        case OPTIONS_MENU: {
            updateMenu(ctx->sdl_ctx, mouseCoord, mouseInputFlag, ctx->menus[OPTIONS_MENU], updateOptionsMenu, NULL);
            renderMenu(ctx->sdl_ctx, ctx->menus[OPTIONS_MENU]);
        } break;
        case START_MENU: {
            updateMenu(ctx->sdl_ctx, mouseCoord, mouseInputFlag, ctx->menus[START_MENU], updateHomeMenu, NULL);
            renderMenu(ctx->sdl_ctx, ctx->menus[START_MENU]);
        } break;
        case LEVEL_SELECTION_MENU: {
            updateMenu(ctx->sdl_ctx, mouseCoord, mouseInputFlag, ctx->menus[LEVEL_SELECTION_MENU], updateLevelMenu, &(ctx->loadedLevelIdx));
            renderMenu(ctx->sdl_ctx, ctx->menus[LEVEL_SELECTION_MENU]);
            renderText_Ex(ctx->sdl_ctx, temp_sprintf("choose a level :", ctx->player->velocity.x), BLACK, (V2f){94.0f, 198.0f}); // ombre
            renderText_Ex(ctx->sdl_ctx, temp_sprintf("choose a level :", ctx->player->velocity.x), WHITE, (V2f){92.0f, 196.0f});
        } break;
        default:
            break;
        }

        SDL_RenderPresent(ctx->sdl_ctx->renderer);

        frameCounter++;
        currLevel = getLoadedLevel(ctx);
        SDL_Delay(16); // 16.6667 ms ~= 60fps
    }

    destroyBar(&hpBar);
    return true;
}
