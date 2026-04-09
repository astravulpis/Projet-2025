/**
 * @file sdl_ctx.c
 * @brief Implementation of the different SDL context function
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-03-15
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#include "sdl_ctx.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "common.h"
#include "sdl_helpers.h"

bool createCtx(sdl_ctx_t **ctx)
{
    (*ctx) = calloc(1, sizeof(sdl_ctx_t));
    if ((*ctx) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for sdl_ctx_t. Critical error!", __FILE__, __LINE__);
        return false;
    }

    if (!initCtx((*ctx))) return false;

    return true;
}

void initOptions(options *opts)
{
    opts->masterVolume = 75.0f;
    opts->musicVolume = 15.0f;
    opts->sfxVolume = 35.0f;
}

void setMasterTrackGain(sdl_ctx_t *ctx)
{
    MIX_SetMixerGain(ctx->mixer, ctx->opts.masterVolume / 100.f);
}

void setMusicTrackGain(sdl_ctx_t *ctx)
{
    MIX_SetTrackGain(ctx->tracks[BACKGROUND_MUSIC], ctx->opts.musicVolume / 100.f);
}

void setSfxTrackGain(sdl_ctx_t *ctx)
{
    for (int id = BACKGROUND_MUSIC + 1; id < TRACK_COUNT; ++id) {
        MIX_SetTrackGain(ctx->tracks[id], ctx->opts.sfxVolume / 100.f);
    }
}

bool initCtx(sdl_ctx_t *sdl_ctx)
{
    bool result = true; // true by default to allow fall-through pass the `defer` label

    SDL_WindowFlags windowFlags = 0x0; // To add a flag, use the bit-wise OR, such as:
                                       // SDL_[FLAG1] | SDL_[FLAG2] | SDL_[FLAG3]

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        nob_log(ERROR, "%s:%d: SDL failed to initialize. See: %s", __FILE__, __LINE__, SDL_GetError());
        return_defer(false);
    }

    SDL_CreateWindowAndRenderer("ULTRAC00L", WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags, &(sdl_ctx->window),
                                &(sdl_ctx->renderer));
    if (!sdl_ctx->window) {
        nob_log(ERROR, "%s:%d: SDL failed to create window and renderer. See: %s", __FILE__, __LINE__, SDL_GetError());
        return_defer(false);
    }

    if (!enableVsync(sdl_ctx)) {
        return_defer(false);
    }

    if (!SDL_SetRenderDrawBlendMode(sdl_ctx->renderer, SDL_BLENDMODE_BLEND)) {
        nob_log(ERROR, "%s:%d: SDL failed to initialize SDL_BLENDMODE_BLEND. See: %s", __FILE__, __LINE__, SDL_GetError());
        return_defer(false);
    }

    sdl_ctx->quit = false;
    sdl_ctx->bgRect = createRect(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!sdl_ctx->bgRect) {
        nob_log(ERROR, "%s:%d: Failed creating the window's background rectangle surface", __FILE__, __LINE__);
        return_defer(false);
    }

    sdl_ctx->bgTexture = NULL;

    if (!TTF_Init()) {
        nob_log(ERROR, "%s:%d: Failed to initialize SDL_ttf", __FILE__, __LINE__);
        return false;
    }

    // Calculate the screen's ratio to make everything on screen fit.
    int num_displays;
    SDL_DisplayID *displays = SDL_GetDisplays(&num_displays);
    if (displays == NULL) {
        nob_log(ERROR, "%s:%d: Failed to use SDL_GetDisplays(). See error: %s", __FILE__, __LINE__, SDL_GetError());
        return_defer(false);
    }

    const SDL_DisplayMode *screenInformation = SDL_GetCurrentDisplayMode(*displays);

    if (screenInformation == NULL) {
        nob_log(ERROR, "%s:%d: Failed to use SDL_GetCurrentDisplayMode(). See error: %s", __FILE__, __LINE__, SDL_GetError());
        return_defer(false);
    }

    sdl_ctx->screenRatio = (float)screenInformation->h / (float)WINDOW_HEIGHT;
    SDL_free(displays);

    sdl_ctx->font = loadFont("./assets/font/VCR_OSD_MONO_1.001.ttf", 28.0f * sdl_ctx->screenRatio, TTF_STYLE_NORMAL, 0);
    if (sdl_ctx->font == NULL) {
        nob_log(ERROR, "%s:%d: Failed to load font. See error: %s", __FILE__, __LINE__, SDL_GetError());
        return_defer(false);
    }

    if (!MIX_Init()) {
        nob_log(ERROR, "%s:%d: Failed to initialise MIX", __FILE__, __LINE__);
        return_defer(false);
    }

    SDL_AudioSpec spec;
    spec.freq = 44100;
    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;
    sdl_ctx->mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (!sdl_ctx->mixer) {
        nob_log(ERROR, "%s:%d: Couldn't create mixer on default device: %s", __FILE__, __LINE__, SDL_GetError());
        return_defer(false);
    }

    for (size_t i = 0; i < TRACK_COUNT; ++i) {
        sdl_ctx->tracks[i] = MIX_CreateTrack(sdl_ctx->mixer);
        if (!sdl_ctx->tracks[i]) {
            nob_log(ERROR, "%s:%d: Couldn't create a mixer track: %s", __FILE__, __LINE__, SDL_GetError());
            return_defer(false);
        }
    }

    initOptions(&sdl_ctx->opts);
    setMasterTrackGain(sdl_ctx);
    setMusicTrackGain(sdl_ctx);
    setSfxTrackGain(sdl_ctx);

    SDL_SetWindowFullscreen(sdl_ctx->window, true);

defer:
    if (result == false) {
        closeCtx(&sdl_ctx);
    }

    return result;
}

void closeCtx(sdl_ctx_t **sdl_ctx)
{
    if ((*sdl_ctx) != NULL) {
        sdl_ctx_t *c = (*sdl_ctx);

        MIX_StopAllTracks(c->mixer, 0);

        clearContextSurface(c);
        free(c->bgRect);
        c->bgRect = NULL;

        // Safe function that does nothing when given a NULL
        TTF_CloseFont(c->font);
        c->font = NULL;

        if (c->renderer != NULL) {
            SDL_DestroyRenderer(c->renderer);
            c->renderer = NULL;
        }

        if (c->window != NULL) {
            SDL_DestroyWindow(c->window);
            c->window = NULL;
        }
    }

    // Freeing a NULL is fine
    free(*sdl_ctx);

    MIX_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool enableVsync(sdl_ctx_t *sdl_ctx)
{
    bool res = true;
    sdl_ctx->vsyncActivation = true;
    if (!SDL_SetRenderVSync(sdl_ctx->renderer, 1)) {
        nob_log(ERROR, "%s:%d: Failed to activate VSync. See error: %s", __FILE__, __LINE__, SDL_GetError());
        res = false;
    }

    return res;
}

bool disableVsync(sdl_ctx_t *sdl_ctx)
{
    bool res = true;
    sdl_ctx->vsyncActivation = false;
    if (!SDL_SetRenderVSync(sdl_ctx->renderer, 0)) {
        nob_log(ERROR, "%s:%d: Failed to disable VSync. See error: %s", __FILE__, __LINE__, SDL_GetError());
        res = false;
    }

    return res;
}

void clearContextSurface(sdl_ctx_t *sdl_ctx)
{
    if (sdl_ctx->bgTexture != NULL) {
        SDL_DestroyTexture(sdl_ctx->bgTexture);
        sdl_ctx->bgTexture = NULL;
    }
}

bool loadBackgroundImage(sdl_ctx_t *sdl_ctx, const char *path)
{
    sdl_ctx->bgTexture = IMG_LoadTexture(sdl_ctx->renderer, path);
    if (sdl_ctx->bgTexture == NULL) {
        nob_log(ERROR, "%s:%d: Failed to load background image. See error: %s", __FILE__, __LINE__, SDL_GetError());
        return false;
    }

    return true;
}

void renderBackground(sdl_ctx_t *sdl_ctx)
{
    if (sdl_ctx->bgTexture != NULL) {
        renderImage(sdl_ctx, sdl_ctx->bgTexture, sdl_ctx->bgRect);
    }
}

TTF_Font *loadFont(char *path, float fontSize, int fontStyle, int outline)
{
    /* Here the different styles a font can be set to:
     *   TTF_STYLE_NORMAL
     *   TTF_STYLE_BOLD
     *   TTF_STYLE_ITALIC
     *   TTF_STYLE_UNDERLINE
     *   TTF_STYLE_STRIKETHROUGH
     */

    TTF_Font *result = NULL;

    result = TTF_OpenFont(path, fontSize);
    if (result == NULL) {
        nob_log(ERROR, "%s:%d: Failed to open font. See error: %s", __FILE__, __LINE__, SDL_GetError());
        return_defer(NULL);
    }

    TTF_SetFontStyle(result, fontStyle);

    if (!TTF_SetFontOutline(result, outline)) {
        nob_log(ERROR, "%s:%d: Failed to set font outline", __FILE__, __LINE__);
        return NULL;
    }

    // Clears any glyph loaded into memory + Sets already text rendering instances to use the font
    TTF_SetFontHinting(result, TTF_HINTING_NORMAL);

defer:
    if (result == NULL) {
        TTF_CloseFont(result);
    }
    return result;
}
