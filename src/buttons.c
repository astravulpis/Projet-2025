#include "buttons.h"
#include "common.h"
#include "sdl_helpers.h"

button *initButton(SDL_FRect *b_box, char *b_text, SDL_Color *b_baseC, SDL_Color *b_hoverC, SDL_Color *b_clickC){
    //création du boutton
    button *b = malloc(sizeof(button));

    //contenu et taille du bouton
    //pas de copie des chaines, juste un pointeur vers la chaine
    //(comme ça au peut modifier le contenu dans le main)
    b->buttonText = b_text;
    //b->buttonhoveredText = b_hoveredText;
    b->buttonBox = b_box;

    //couleurs du boutton
    b->baseColor = b_baseC;
    b->hoverColor = b_hoverC;
    b->clickColor = b_clickC;

    //variables d'état du bouton
    b->hovered = false;
    b->leftClicked = false;
    b->rightClicked = false;

    return b;
}

void destroyButton(button **b){
    free((*b)->buttonBox);
    free(*b);
    b = NULL;
}

void updateButtonState(button *b, SDL_FPoint mouseCoord, int mouseFlag){
    if (SDL_PointInRectFloat(&mouseCoord, b->buttonBox)) {

        b->hovered = true;

        //clici gauche
        if (mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT))
            b->leftClicked = true;
        else
            b->leftClicked = false;

        //clic droit
        if (mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT))
            b->rightClicked = true;
        else            
            b->rightClicked = false;

    }  
    else {//par sécurité on remet tout a False
        b->hovered = false;
        b->leftClicked = false;
        b->rightClicked = false;
    }
}

void buttonRender(sdl_ctx_t *sdl_ctx, button *b){//rend le texte et l'arrière plan du bouton
    if (b->hovered){
        SDL_Color *tempC;

        if (b->leftClicked || b->rightClicked)
            tempC = b->clickColor;
        else
            tempC = b->hoverColor;

        //rendu de la'arrière plan du bouton
        SDL_SetRenderDrawColor(sdl_ctx->renderer, tempC->r, tempC->g, tempC->b, tempC->a);
        SDL_RenderFillRect(sdl_ctx->renderer, b->buttonBox);
        SDL_SetRenderDrawColor(sdl_ctx->renderer, 0, 0, 0, 255);//on remet en noir

        //claculs pour le centrage du texte dans le bouton
        size_t pixelWidth = 0;
        int buttonTextWidth = 0;
        int buttonTextHeight = 0;

        TTF_MeasureString(sdl_ctx->font, temp_sprintf("%s", b->buttonText), 0, 0, NULL, &pixelWidth);
        TTF_GetStringSize(sdl_ctx->font, b->buttonText, pixelWidth, &buttonTextWidth, &buttonTextHeight);

        float XCentering = ((b->buttonBox)->w /2) - buttonTextWidth / 2;
        float YCentering = ((b->buttonBox)->h /2) - buttonTextHeight / 2;
     
        V2f tempPos = {(b->buttonBox)->x + XCentering, (b->buttonBox)->y + YCentering};
        renderText_Ex(sdl_ctx, temp_sprintf("%s", b->buttonText), WHITE, tempPos);
    }
    else {
        SDL_Color *tempC = b->baseColor;

        //rendu de la'arrière plan du bouton
        SDL_SetRenderDrawColor(sdl_ctx->renderer, tempC->r, tempC->g, tempC->b, tempC->a);
        SDL_RenderFillRect(sdl_ctx->renderer, b->buttonBox);
        SDL_SetRenderDrawColor(sdl_ctx->renderer, 0, 0, 0, 255);//on remet en noir

        //rendu du texte du bouton
        size_t pixelWidth = 0;
        int buttonTextWidth = 0;
        int buttonTextHeight = 0;

        TTF_MeasureString(sdl_ctx->font, temp_sprintf("%s", b->buttonText), 0, 0, NULL, &pixelWidth);
        TTF_GetStringSize(sdl_ctx->font, b->buttonText, pixelWidth, &buttonTextWidth, &buttonTextHeight);

        float XCentering = ((b->buttonBox)->w /2) - buttonTextWidth / 2;
        float YCentering = ((b->buttonBox)->h /2) - buttonTextHeight / 2;
     
        V2f tempPos = {(b->buttonBox)->x + XCentering, (b->buttonBox)->y + YCentering};
        renderText_Ex(sdl_ctx, temp_sprintf("%s", b->buttonText), WHITE, tempPos);
    }
}