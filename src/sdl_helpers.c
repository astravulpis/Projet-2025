#include "sdl_helpers.h"
#include "common.h"
#include <stdlib.h>

sdl_ctx_t *init_all(void)
{
    sdl_ctx_t *ctx = NULL;
    SDL_WindowFlags windowFlags = 0x0; // Pour ajouter des flags, utiliser le `OR` binaire, soit `|`. e.g:
                                       // SDL_[FLAG1] | SDL_[FLAG2] | SDL_[FLAG3]

    ctx = malloc(sizeof(sdl_ctx_t));
    if (!ctx) return NULL;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        nob_log(ERROR, "%s:%d: SDL failed to initialize. See: %s", __FILE__, __LINE__, SDL_GetError());
        return NULL;
    }

    SDL_CreateWindowAndRenderer("ULTRAC00L", WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags, &(ctx->window), &(ctx->renderer));
    if (!ctx->window) {
        nob_log(ERROR, "%s:%d: SDL failed to create window and renderer. See: %s", __FILE__, __LINE__, SDL_GetError());
        close_SDL(ctx);
        return NULL;
    }

    if (!enableVsync(ctx)) {
        nob_log(ERROR, "%s:%d: SDL failed to create window and renderer. See error backtrace above", __FILE__, __LINE__);
        close_SDL(ctx);
        return NULL;
    }

    ctx->quit = false;
    ctx->bgRect = createRect(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!ctx->bgRect) {
        nob_log(ERROR, "%s:%d: Failed creating the window's background rectangle surface", __FILE__, __LINE__);
        close_SDL(ctx);
        return NULL;
    }

    ctx->bgTexture = NULL;

    // initialisation de la prise en charge des fonts
    if (!TTF_Init()) {
        nob_log(ERROR, "%s:%d: Failed to initialize SDL_ttf", __FILE__, __LINE__);
        close_SDL(ctx);
        return NULL;
    }

    return ctx;
}

void close_SDL(sdl_ctx_t *sdl_ctx)
{
    if (sdl_ctx) { // If it isn't NULL
        SDL_DestroyWindow(sdl_ctx->window);
        SDL_DestroyRenderer(sdl_ctx->renderer);

        sdl_ctx->window = NULL;
        sdl_ctx->renderer = NULL;

        clearContextSurface(sdl_ctx);
        free(sdl_ctx->bgRect);
        sdl_ctx->bgRect = NULL;
    }

    // Freeing a NULL is fine
    free(sdl_ctx);

    SDL_Quit();
}

SDL_FRect *createRect(float x, float y, float width, float height)
{
    SDL_FRect *res = NULL;
    if ((res = malloc(sizeof(SDL_FRect))) == NULL) {
        nob_log(ERROR, "%s:%d: failed to create a rect. Please buy more ram", __FILE__, __LINE__);
        return NULL;
    }

    if (width <= 0) width = 1;
    if (height <= 0) height = 1;

    res->x = x;
    res->y = y;
    res->w = width;
    res->h = height;

    return res;
}

bool enableVsync(sdl_ctx_t *sdl_ctx)
{
    bool res = true;
    sdl_ctx->vsyncActivation = true;
    // Activation du Vsync pour avoir un contrôle du framerate et éviter une surcharge du pc
    if (!SDL_SetRenderVSync(sdl_ctx->renderer, 1)) {
        nob_log(ERROR, "%s:%d: Impossible d'activer la VSync. Voir: %s", __FILE__, __LINE__, SDL_GetError());
        res = false;
    }

    return res;
}

bool disableVsync(sdl_ctx_t *sdl_ctx)
{
    bool res = true;
    sdl_ctx->vsyncActivation = false;
    // Activation du Vsync pour avoir un contrôle du framerate et éviter une surcharge du pc
    if (!SDL_SetRenderVSync(sdl_ctx->renderer, 0)) {
        nob_log(ERROR, "%s:%d: Impossible de desactiver la VSync. Voir: %s", __FILE__, __LINE__, SDL_GetError());
        res = false;
    }

    return res;
}

void clearContextSurface(sdl_ctx_t *sdl_ctx)
{
    SDL_DestroyTexture(sdl_ctx->bgTexture);
    sdl_ctx->bgTexture = NULL;
}

void renduImage(sdl_ctx_t *sdl_ctx, SDL_Texture *textureImg, SDL_FRect *rect)
{
    assert(textureImg != NULL);
    SDL_RenderTexture(sdl_ctx->renderer, textureImg, NULL, rect);
}

SDL_Texture *chargerImage(sdl_ctx_t *sdl_ctx, char *chemin)
{
    // chargement de l'image dans une surface, si cela échoue on retourne directement NULL
    // format pris en charge BMP, PNG,

    char extension[10];
    SDL_Surface *surfaceImg = NULL;

    sscanf(chemin, "%*[^.]%s", extension);
    // nob_log(INFO, "Extension : %s", extension);

    // Appel de la fonction adapté a l'extension de l'image
    if (strcmp(".bmp", extension) == 0) {
        surfaceImg = SDL_LoadBMP(chemin);
    } else if (strcmp(".png", extension) == 0) {
        surfaceImg = SDL_LoadPNG(chemin);
    }

    if (surfaceImg == NULL) return NULL;

    SDL_Texture *textureImg = SDL_CreateTextureFromSurface(sdl_ctx->renderer, surfaceImg);
    SDL_DestroySurface(surfaceImg);

    return textureImg;
}

void loadBackgroundImage(sdl_ctx_t *sdl_ctx, char *chemin)
{
    sdl_ctx->bgTexture = chargerImage(sdl_ctx, chemin);
}

void renderBackground(sdl_ctx_t *sdl_ctx)
{
    if (sdl_ctx->bgTexture != NULL) {
        renduImage(sdl_ctx, sdl_ctx->bgTexture, sdl_ctx->bgRect);
    }
}

// void printTextSdl(sdl_ctx_t *sdl_ctx, char *text, ...)
// {
// }

TTF_Font *loadFont(char *path, float size, int fontStyle, int outline)
{
    /* Voicic les différents style d'écriture de police
    TTF_STYLE_NORMAL (is zero)
    TTF_STYLE_BOLD
    TTF_STYLE_ITALIC
    TTF_STYLE_UNDERLINE
    TTF_STYLE_STRIKETHROUGH
    */

    TTF_Font *font = TTF_OpenFont(path, size);
    if (font == NULL) return NULL;

    // paramétrage de la la police a l'aide de fontsyle et outline
    // ne retourne rien
    TTF_SetFontStyle(font, fontStyle);

    if (!TTF_SetFontOutline(font, outline)) {
        nob_log(ERROR, "%s:%d: Failed to set font outline", __FILE__, __LINE__);
        TTF_CloseFont(font);
        return NULL;
    }

    // ne retourne rien
    TTF_SetFontHinting(font, TTF_HINTING_NORMAL);

    return font;
}

void print_Sdl_Text(sdl_ctx_t *sdl_ctx, char *text, TTF_Font *font, SDL_Color fontColor, SDL_FRect *drawingBox)
{
    SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, 0, fontColor, 0);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(sdl_ctx->renderer, textSurface);

    // empếche l'application de crash, dans le cas ou c'est NULL
    if (textTexture == NULL) return;

    SDL_RenderTexture(sdl_ctx->renderer, textTexture, NULL, drawingBox);

    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
