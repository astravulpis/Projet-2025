#include "bars.h"
#include "common.h"
#include "sdl_helpers.h"

bool createBar(bar **h, SDL_FRect rect, SDL_Color bgColor, SDL_Color fillColor, SDL_Color cursorColor, float maxHp,
               float barPadding)
{
    *h = calloc(1, sizeof(bar));
    if ((*h) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for  bar", __FILE__, __LINE__);
        return false;
    }

    (*h)->BarBox = createRect_Ex(rect);
    (*h)->bgColor = bgColor;
    (*h)->fillColor = fillColor;
    (*h)->cursorColor = cursorColor;

    (*h)->maxHp = maxHp;
    (*h)->barPadding = barPadding;

    return true;
}

void destroyBar(bar **h)
{
    free((*h)->BarBox);
    (*h)->BarBox = NULL;
    free(*h);
    h = NULL;
}

void barRender(sdl_ctx_t *sdl_ctx, bar *h, float hpValue, int s_intensity, int l_intensity,
               int ls_opacity) // s = shadow et l = ligth
{
    int textWidth = 0;
    int textHeight = 0;

    // Compare hpValue a la valeur maximum de la barre, copie la valeur das hp, la rabaisse si trop grande, la met a 0 si
    // négative cela ne se fait que sur la jauge, pas le pourcentage affiché (cela me parraît utile d'avoir la vrai valeur qui
    // est mise en paramètre pour un potentiel debug)
    float hp = hpValue;
    if (hpValue > h->maxHp) hp = h->maxHp;
    else if (hpValue < 0)
        hp = 0;

    char *hpText = temp_sprintf("%.1f%%", hpValue);
    TTF_GetStringSize(sdl_ctx->font, hpText, 0, &textWidth, &textHeight);

    // Calculates the center of the life point information text
    float XCentering = ((h->BarBox)->w / 2.0f) - textWidth / 2.0f;
    float YCentering = ((h->BarBox)->h / 2.0f) - textHeight / 2.0f;
    V2f barTextPos = {(h->BarBox)->x + XCentering, (h->BarBox)->y + YCentering};
    V2f barTextPosShadow = {(h->BarBox)->x + XCentering + 2 * sdl_ctx->screenRatio,
                            (h->BarBox)->y + YCentering + 1 * sdl_ctx->screenRatio};

    // Calcul de fillBox via barPadding (normalement plus petite que h->BarBox, sinon il y'a un problème XD), largeur
    // calculées en fonction de hp, représente la jauge de vie
    SDL_FRect fillBox = {(h->BarBox)->x + ((h->barPadding / 2) * sdl_ctx->screenRatio),
                         (h->BarBox)->y + ((h->barPadding / 2) * sdl_ctx->screenRatio),
                         ((h->BarBox)->w - (h->barPadding * sdl_ctx->screenRatio)) / h->maxHp * hp,
                         (h->BarBox)->h - (h->barPadding * sdl_ctx->screenRatio)};

    // Calcul de emptyFillBox, qui représente l'espace que ne prend pas fillBox
    SDL_FRect emptyFillBox = {fillBox.x + fillBox.w, fillBox.y,
                              ((h->BarBox)->w - (h->barPadding * sdl_ctx->screenRatio)) / h->maxHp * (h->maxHp - hp),
                              fillBox.h};
    SDL_Color emptyBgColor = {(h->bgColor.r + 20) < 255 ? (h->bgColor.r + 20) : 255,
                              (h->bgColor.g + 20) < 255 ? (h->bgColor.g + 20) : 255,
                              (h->bgColor.b + 20) < 255 ? (h->bgColor.b + 20) : 255, 200};

    // rectangles et couleur de rendu pour effets d'ombres sur la barre de vie
    SDL_FRect horizontal_shadowFillBox = {fillBox.x, fillBox.y + fillBox.h - (h->barPadding * sdl_ctx->screenRatio), fillBox.w,
                                          h->barPadding * sdl_ctx->screenRatio};
    SDL_FRect vertical_shadowFillBox = {fillBox.x, fillBox.y, h->barPadding * sdl_ctx->screenRatio, fillBox.h};
    SDL_Color shadowColor = {
        ((h->fillColor.r - s_intensity) >= 0) && ((h->fillColor.r - s_intensity) <= 255) ? (h->fillColor.r - s_intensity) : 0,
        ((h->fillColor.g - s_intensity) >= 0) && ((h->fillColor.g - s_intensity) <= 255) ? (h->fillColor.g - s_intensity) : 0,
        ((h->fillColor.b - s_intensity) >= 0) && ((h->fillColor.b - s_intensity) <= 255) ? (h->fillColor.b - s_intensity) : 0,
        (ls_opacity >= 0 && ls_opacity <= 255) ? ls_opacity
                                               : 255}; // si l'opacité n'est pas bonne, alors l'ombre n'aura aucune transparence

    // rectangles et couleur de rendu pour effets de lumières sur la barre de vie
    SDL_FRect horizontal_lightFillBox = {fillBox.x, fillBox.y, fillBox.w, h->barPadding * sdl_ctx->screenRatio};
    SDL_FRect vertical_lightFillBox = {fillBox.x + fillBox.w - (h->barPadding * sdl_ctx->screenRatio), fillBox.y,
                                       h->barPadding * sdl_ctx->screenRatio, fillBox.h};
    SDL_Color lightColor = {
        ((h->fillColor.r + l_intensity) >= 0) && ((h->fillColor.r + l_intensity) <= 255) ? (h->fillColor.r + l_intensity) : 255,
        ((h->fillColor.g + l_intensity) >= 0) && ((h->fillColor.g + l_intensity) <= 255) ? (h->fillColor.g + l_intensity) : 255,
        ((h->fillColor.b + l_intensity) >= 0) && ((h->fillColor.b + l_intensity) <= 255) ? (h->fillColor.b + l_intensity) : 255,
        (ls_opacity >= 0 && ls_opacity <= 255) ? (ls_opacity * (hp / h->maxHp))
                                               : 255}; // si l'opacité n'est pas bonne, alors l'ombre n'aura aucune transparence

    // initialisations des boxs et couleurs pour le curseur et ses ombres/ lumières
    SDL_FRect cursorBox = {fillBox.x + fillBox.w - (h->barPadding * sdl_ctx->screenRatio / 2),
                           h->BarBox->y + (6 * sdl_ctx->screenRatio) / 2, h->barPadding * sdl_ctx->screenRatio / 2,
                           h->BarBox->h - (6 * sdl_ctx->screenRatio)};

    SDL_FRect cursorShadowBox = {cursorBox.x, cursorBox.y, cursorBox.w / 3, cursorBox.h};
    SDL_Color cursorShadowColor = {50, 50, 50, 100}; // valeurs en dur, je vais peut être revenir dessus, mais il me semble que
                                                     // les barres de vies sont déja assez personnalisables

    SDL_FRect cursorLightBox = {cursorBox.x + cursorBox.w - (cursorBox.w / 3), cursorBox.y, cursorBox.w / 3, cursorBox.h};
    SDL_Color cursorLightColor = {205, 205, 205, 100}; // valeurs en dur, je vais peut être revenir dessus, mais il me semble
                                                       // que les barres de vies sont déja assez personnalisables

    // rendu de la barre et de son arrière plan
    renderFillRect(sdl_ctx->renderer, h->BarBox, h->bgColor);
    renderFillRect(sdl_ctx->renderer, &fillBox, h->fillColor);
    renderFillRect(sdl_ctx->renderer, &emptyFillBox, emptyBgColor);

    // rendu des ombres et lumières sur la barre de vie
    renderFillRect(sdl_ctx->renderer, &horizontal_shadowFillBox, shadowColor);
    renderFillRect(sdl_ctx->renderer, &vertical_shadowFillBox, shadowColor);
    renderFillRect(sdl_ctx->renderer, &horizontal_lightFillBox, lightColor);
    renderFillRect(sdl_ctx->renderer, &vertical_lightFillBox, lightColor);

    // rendu du curseur et ses ombres/lumières
    renderFillRect(sdl_ctx->renderer, &cursorBox, h->cursorColor);
    renderFillRect(sdl_ctx->renderer, &cursorShadowBox, cursorShadowColor);
    renderFillRect(sdl_ctx->renderer, &cursorLightBox, cursorLightColor);

    // rendu du texte, avec un effet d'ombre qui imite a la perfection le raytracing
    renderText_Ex(sdl_ctx, hpText, BLACK, barTextPosShadow);
    renderText_Ex(sdl_ctx, hpText, WHITE, barTextPos);
}
