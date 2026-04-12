#include "game.h"
#include "bars.h"
#include "common.h"
#include "entity.h"
#include "event.h"
#include "file_parsing.h"
#include "jsonParsing.h"
#include "level.h"
#include "player.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "triggers.h"

// All declaration for internal helper functions

// Initialization helper functions
bool initGameContext(gameContext **gameCtx);
bool initAllMenus(gameContext *ctx);
bool loadAllLevels(gameContext *ctx);
room_t *beginLevel(int argc, char **argv, gameContext *ctx);

// Getter helper functions
void pollEvents(gameContext *ctx);
void getMouseState(gameContext *ctx);
level_t *getLoadedLevel(gameContext *ctx);
float getDeltaTime();
bool hasMouseLeftClicked(gameContext *ctx);
int calculateFPS();

// Render helper functions
void beginRendering(gameContext *ctx);
void renderMenus(gameContext *ctx);

// Other helper functions
void updateMenus(gameContext *ctx);

bool gameLoop(gameContext *ctx, int argc, char **argv)
{
    size_t mark = temp_save();
    bool needsFpsCap = false;
    float deltaTime = 0;
    int frameRate = 0;

    // SDL_FRect footerBox = {0, (WINDOW_HEIGHT - 150.f), WINDOW_WIDTH, 150.f};
    // boxToScale(&footerBox, ctx->sdl_ctx->screenRatio);
    //
    // SDL_FRect cWeaponBox = {(WINDOW_WIDTH / 2.f - 300.f), (WINDOW_HEIGHT - 135.f), 600.f, 120.f};
    // boxToScale(&cWeaponBox, ctx->sdl_ctx->screenRatio);
    //
    // SDL_FRect styleMeterBox = {(WINDOW_WIDTH - 265.f), (WINDOW_HEIGHT - 135.f), 250.f, 120.f};
    // boxToScale(&styleMeterBox, ctx->sdl_ctx->screenRatio);
    //
    // bar *hpBar = NULL;
    // bar *dashBar1 = NULL;
    // bar *dashBar2 = NULL;
    // bar *dashBar3 = NULL;
    //
    // createPlayerStatusBar(ctx->sdl_ctx, &dashBar1, &dashBar2, &dashBar3, &hpBar);

    while (!ctx->sdl_ctx->quit) {
        temp_rewind(mark);
        deltaTime = getDeltaTime();
        frameRate = calculateFPS();
        if (frameRate > 60) needsFpsCap = true;

        pollEvents(ctx);
        beginRendering(ctx);

        // renderRoom(ctx->sdl_ctx, currLevel);
        // rendertriggers(ctx->sdl_ctx, currLevel);
        // if (!ctx->sdl_ctx->currMenu &&
        //     (mouseInputFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT) && !(prevMouseInput & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)))) {
        //     V2f startingPos = (V2f){getBB(ctx->player)->x + getBB(ctx->player)->w / 2.0f,
        //                             getBB(ctx->player)->y + getBB(ctx->player)->h / 2.0f};
        //     V2f deltaPos = (V2f){ctx->mousePos.x - startingPos.x, ctx->mousePos.y - startingPos.y - 15.0f};
        //     float magnitude = SDL_sqrt((deltaPos.x * deltaPos.x) + (deltaPos.y * deltaPos.y));
        //     V2f vel = (V2f){((deltaPos.x / magnitude) * 2500), ((deltaPos.y / magnitude) * 2500)};
        //
        //     shootGun(ctx->sdl_ctx, &ctx->guns->arsenal[ctx->guns->selectedGun], &ctx->bullet_arr, startingPos, vel);
        // }
        // else if (mouseInputFlag & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT) && !(prevMouseInput & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)))
        // {
        //     printf("Right Clicked\n");
        // }

        // renderBullets(ctx->sdl_ctx, &ctx->bullet_arr);

        // if (!ctx->sdl_ctx->currMenu) { //updates the game elements only if we aren't in a menu
        //     updateTriggers(currLevel, ctx->player);
        //     updateBulletState(&ctx->bullet_arr, currLevel, deltaTime, ctx->player);
        //     updateEntities(getCurrentEntityWave(currLevel), ctx->player, getRoomObjects(currLevel), deltaTime);
        //     updatePlayer(ctx->player, getRoomObjects(currLevel), deltaTime);
        // }

        // renderPlayer(ctx->player);
        // renderEntities(getCurrentEntityWave(currLevel));

        renderText_Ex(ctx->sdl_ctx, temp_sprintf("fps : %d", frameRate), BLACK, (V2f){10.0f, 10.0f});

        // Everything after the footer being rendered is rendered OVER it.
        // renderPlayerStatusBar(ctx->sdl_ctx, ctx->player, dashBar1, dashBar2, dashBar3, hpBar);

        // Update and render the menu at the very end
        // il faut que le menu home s'affiche au démarrage, puis charge un niveau, pas que le niveau

        updateMenus(ctx);
        renderMenus(ctx);
        SDL_RenderPresent(ctx->sdl_ctx->renderer);

        // currLevel = getLoadedLevel(ctx);
        if (needsFpsCap) SDL_Delay(16); // 16.6667 ms ~= 60fps
    }

    // destroyPlayerStatusBar(&dashBar1, &dashBar2, &dashBar3, &hpBar);
    return true;
}

level_t *getLoadedLevel(gameContext *ctx)
{
    return ctx->levels[ctx->loadedLevelIdx];
}

// Technically a debug function?
// Really necessary if you past arguments to the main function
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
    char *paths[5] = {"levelA", "levelB", "levelC", "levelD", "levelE"};

    for (size_t i = ctx->level_count; i < ARRAY_LEN(paths); i++) {
        level_t *level = NULL;
        parseFile(paths[i], &ctx->sdl_ctx, &level);
        // assert(level != NULL && "Parse file didn't work");

        ctx->levels[i] = level;
        ctx->level_count++;
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

// Add more menus here
bool initAllMenus(gameContext *ctx)
{
    if (!addMenu(ctx, createPauseMenu(ctx->sdl_ctx), PAUSE_MENU)) return false;
    if (!addMenu(ctx, createOptionsMenu(ctx->sdl_ctx), OPTIONS_MENU)) return false;
    if (!addMenu(ctx, createHomeMenu(ctx->sdl_ctx), START_MENU)) return false;
    if (!addMenu(ctx, createLevelMenu(ctx->sdl_ctx), LEVEL_SELECTION_MENU)) return false;
    return true;
}

float getDeltaTime()
{
    static Uint32 last = 0;
    Uint32 now = SDL_GetTicks();
    float deltaTime = (now - last) / 1000.0f; // seconds since last frame
    last = now;
    return deltaTime;
}

int calculateFPS()
{
    static Uint32 last = 0;
    Uint32 now = SDL_GetTicks();
    static int frameCounter = 0;
    int frameRate = 0;
    if (now - last >= 1000) {
        frameRate = frameCounter;
        frameCounter = 0;
        last = now;
    }
    frameCounter++;
    return frameRate;
}

void getMouseState(gameContext *ctx)
{
    ctx->mouse.currState = SDL_GetMouseState(&ctx->mouse.position.x, &ctx->mouse.position.y);
}

bool hasMouseLeftClicked(gameContext *ctx)
{
    getMouseState(ctx);
    bool result =
        (ctx->mouse.currState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT) && !(ctx->mouse.prevState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)));
    ctx->mouse.prevState = ctx->mouse.currState;
    return result;
}

void pollEvents(gameContext *ctx)
{
    while (SDL_PollEvent(&ctx->sdl_ctx->event)) {
        switch (ctx->sdl_ctx->event.type) {
        case SDL_EVENT_QUIT:
            ctx->sdl_ctx->quit = true;
            break;
        }
    }
    SDL_PumpEvents();
    getMouseState(ctx);
    basicKeyboardEvents(ctx->sdl_ctx, ctx->guns);
}

void beginRendering(gameContext *ctx)
{
    SDL_SetRenderDrawColor(ctx->sdl_ctx->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(ctx->sdl_ctx->renderer);
    renderBackground(ctx->sdl_ctx);
}

void updateMenus(gameContext *ctx)
{
    switch (ctx->sdl_ctx->currMenu) {
    case NONE_MENU:
        break;
    case LEVEL_SELECTION_MENU: {
        updateMenu(ctx->sdl_ctx, ctx->mouse.position, ctx->mouse.currState, ctx->menus[LEVEL_SELECTION_MENU], updateLevelMenu,
                   .loadedLevelIdx = &(ctx->loadedLevelIdx));
    } break;
    case PAUSE_MENU: {
        updateMenu(ctx->sdl_ctx, ctx->mouse.position, ctx->mouse.currState, ctx->menus[PAUSE_MENU], updatePauseMenu);
    } break;
    case OPTIONS_MENU: {
        updateMenu(ctx->sdl_ctx, ctx->mouse.position, ctx->mouse.currState, ctx->menus[OPTIONS_MENU], updateOptionsMenu);
    } break;
    case START_MENU: {
        updateMenu(ctx->sdl_ctx, ctx->mouse.position, ctx->mouse.currState, ctx->menus[START_MENU], updateHomeMenu);
    } break;
    default:
        break;
    }
}

void renderMenus(gameContext *ctx)
{
    switch (ctx->sdl_ctx->currMenu) {
    case NONE_MENU:
        break;
    case PAUSE_MENU: // Fallthrough
    case OPTIONS_MENU: // Fallthrough
    case START_MENU: {
        renderMenu(ctx->sdl_ctx, ctx->menus[ctx->sdl_ctx->currMenu]);
    } break;
    case LEVEL_SELECTION_MENU: {
        renderMenu(ctx->sdl_ctx, ctx->menus[LEVEL_SELECTION_MENU]);
        renderText_Ex(ctx->sdl_ctx, "choose a level :", BLACK, (V2f){94.0f, 198.0f}); // ombre
        renderText_Ex(ctx->sdl_ctx, "choose a level :", WHITE, (V2f){92.0f, 196.0f});
    } break;
    default:
        break;
    }
}

bool initGameContext(gameContext **gameCtx)
{
    *gameCtx = calloc(1, sizeof(gameContext));
    if ((*gameCtx) != NULL) {
        gameContext *ctx = *gameCtx;
        // 0st place in taking a lot of time -> Loads every SDL contexts (i.e. SDL, SDL Image, ttf, mixer)
        if (!createCtx(&ctx->sdl_ctx)) return false;
        getMouseState(ctx);
        initAllMenus(ctx);

        // 1st place in taking a lot of time -> Loads the player
        // if (!createPlayer(&ctx->player, (V2f){100, 120}, &ctx->sdl_ctx)) return 1;

        // room_t *currRoom = NULL;
        // 2rd place in taking a lot of time -> parse the file and loads the level
        // if ((currRoom = beginLevel(argc, argv, ctx)) == NULL) return false;
        // level_t *currLevel = getLoadedLevel(ctx);

        // 3rd place in taking a lot of time -> idk
        // ctx->guns = initialiseGuns(ctx->sdl_ctx);
        // playTrack(ctx->sdl_ctx, BACKGROUND_MUSIC);

        // if (!loadAllLevels(ctx)) return false;
    } else {
        nob_log(ERROR, "Failed to initialize game context");
        return false;
    }

    return true;
}

void closeGame(gameContext **ctx)
{
    if ((*ctx) != NULL) {
        // printf("value of the player's score at the end of the runtime: %f\n", ctx->player->score);
        // writeJSON((*ctx)->player);
        //
        // if ((*ctx)->levels != NULL) {
        //     for (size_t i = 0; i < (*ctx)->level_count; ++i) {
        //         destroyLevel(&(*ctx)->levels[i]);
        //     }
        // }
        // free((*ctx)->levels);
        // (*ctx)->levels = NULL;
        //
        if ((*ctx)->menus != NULL) {
            for (menu_kind kind = 0; kind < __menu_count; ++kind) {
                if ((*ctx)->menus[kind] != NULL) {
                    destroyMenu(&(*ctx)->menus[kind]);
                }
            }
        }
        free((*ctx)->menus);
        (*ctx)->menus = NULL;
        //
        // destroyGuns(&(*ctx)->guns);
        // deleteBullets(&(*ctx)->bullet_arr);
        // destroyPlayer(&(*ctx)->player);
        closeCtx(&(*ctx)->sdl_ctx);
    }
    free(*ctx);
    *ctx = NULL;
}
