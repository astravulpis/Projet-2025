#include "buttons.h"
#include "common.h"
#include "sdl_helpers.h"
#include "buttons.h"

void pauseMenu (sdl_ctx_t *sdl_ctx, button **resume, button **options, button **quit){
    SDL_FRect *boxResume = createRect((WINDOW_WIDTH / 2 - 150) * sdl_ctx->screenRatio, (WINDOW_HEIGHT / 2 - 37.5 - 90) * sdl_ctx->screenRatio, 300.0f * sdl_ctx->screenRatio, 75.0f * sdl_ctx->screenRatio);
    SDL_FRect *boxOptions = createRect((WINDOW_WIDTH / 2 - 150) * sdl_ctx->screenRatio, (WINDOW_HEIGHT / 2 - 37.5) * sdl_ctx->screenRatio, 300.0f * sdl_ctx->screenRatio, 75.0f * sdl_ctx->screenRatio);
    SDL_FRect *boxQuit = createRect((WINDOW_WIDTH / 2 - 150) * sdl_ctx->screenRatio, (WINDOW_HEIGHT / 2 - 37.5 + 90) * sdl_ctx->screenRatio, 300.0f * sdl_ctx->screenRatio, 75.0f * sdl_ctx->screenRatio);

    SDL_Color baseColor = {0, 0, 255, 255};
    SDL_Color hoverColor = {255, 10, 100, 255};
    SDL_Color clickColor = {200, 10, 100, 255};

    //textes des boutons du menu pause
    char *textResume = "resume";
    char *textOptions = "options";
    char *textQuit = "quit";

    button *resumeButton = initButton(boxResume, textResume, &baseColor, &hoverColor, &clickColor);
    button *optionsButton = initButton(boxOptions, textOptions, &baseColor, &hoverColor, &clickColor);
    button *quitButton = initButton(boxQuit, textQuit, &baseColor, &hoverColor, &clickColor);

    *resume = resumeButton;
    *options = optionsButton;
    *quit = quitButton;
}

void update_and_renderPauseMenu(sdl_ctx_t *sdl_ctx, SDL_FPoint *mouseCoord, int mouseInputFlag, button *resume, button *options, button *quit){
    SDL_SetRenderDrawColor(sdl_ctx->renderer, 60, 60, 60, 120);
    SDL_RenderFillRect(sdl_ctx->renderer, &(SDL_FRect){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT});
    SDL_SetRenderDrawColor(sdl_ctx->renderer, 0, 0, 0, 255); 
    
    updateButtonState(resume, *mouseCoord, mouseInputFlag);
    updateButtonState(options, *mouseCoord, mouseInputFlag);
    updateButtonState(quit, *mouseCoord, mouseInputFlag);
    
    buttonRender(sdl_ctx, resume);
    buttonRender(sdl_ctx, options);
    buttonRender(sdl_ctx, quit);

    if (quit->leftClicked == true)
        sdl_ctx->quit = true;
    if (resume->leftClicked == true)
        sdl_ctx->pause = false;
}

void destroyPauseMenu (button **resume, button **options, button **quit) {
    destroyButton(resume);
    destroyButton(options);
    destroyButton(quit);
}