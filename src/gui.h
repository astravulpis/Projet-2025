#ifndef GUI_H_
#define GUI_H_

#include "common.h"

void pauseMenu (button **resume, button **options, button **quit);
void destroyPauseMenu (button **resume, button **options, button **quit);
void update_and_renderPauseMenu(sdl_ctx_t *sdl_ctx, SDL_FPoint *mouseCoord, int mouseInputFlag, button *resume, button *options, button *quit);

#endif // GUI_H_