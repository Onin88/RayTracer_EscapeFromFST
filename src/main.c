#include <stdio.h>
#include <SDL2/SDL.h>

#include "render/render.h"
#include "player/player.h"
#include "mapping/map.h"

#define MOUSE_SENSITIVITY 1.1

/*
 *
 *	EFST Main program
 *
 */

// debug purpose only
void debugging(char msg[]) {
	printf("[EFST-DEBUG] %s\n", msg);
}

// event handler
int handle_events(SDL_Event *e, Player *p, SDL_Window *win, Map* m) {
	Uint8 *keystates = calloc(1, sizeof(keystates));
    while (SDL_PollEvent(e))
    {
        if (e->type == SDL_KEYDOWN)
        {
            switch (e->key.keysym.sym)
            {
            case SDLK_ESCAPE:
                SDL_Quit();
                return 1;
            case SDLK_q:
                if(!canBeMovedTo(m, p->xCoord - p->dx * 0.2, p->yCoord - p->dy * 0.2)) break;
                p->angle += M_PI / 2;
                fixAngle(p);
                p->xCoord -= p->dx * 0.2;
                p->yCoord -= p->dy * 0.2;
                p->angle -= M_PI / 2;
                fixAngle(p);
                break;
            case SDLK_d:
                if(!canBeMovedTo(m, p->xCoord + p->dx * 0.2, p->yCoord + p->dy * 0.2)) break;
                p->angle += M_PI / 2;
                fixAngle(p);
                p->xCoord += p->dx * 0.2;
                p->yCoord += p->dy * 0.2;
                p->angle -= M_PI / 2;
                fixAngle(p);
                break;
            case SDLK_z:
                if(!canBeMovedTo(m, p->xCoord + p->dx * 0.2, p->yCoord + p->dy * 0.2)) break;
                p->xCoord += p->dx * 0.2;
                p->yCoord += p->dy * 0.2;
                break;
            case SDLK_s:
                if(!canBeMovedTo(m, p->xCoord - p->dx * 0.2, p->yCoord - p->dy * 0.2)) break;
                p->xCoord -= p->dx * 0.2;
                p->yCoord -= p->dy * 0.2;
                break;
            }
        }
        if(e->type == SDL_MOUSEMOTION) {
			float xOff;
			if(e->motion.x > SCREEN_WIDTH / 2) {
				xOff = 0.01;
			} else if(e->motion.x < SCREEN_WIDTH / 2) {
				xOff = -0.01;
			} else {
				xOff = 0;
			}
            p->angle += xOff;
            fixAngle(p);
			SDL_WarpMouseInWindow(SDL_GetWindowFromID(e->motion.windowID), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        }
      
    }
    return 0;
}

// Main
int main(int argc, char* argv[]) {

	debugging("start");
	initSDL();
    EFST_Renderer* efstr = createEFST_Renderer();
    initTextures(efstr);

	Map* map = calloc(2, sizeof(Map));
    readMapFromFile(map, "assets/map.txt");
	Player* p = calloc(1, sizeof(Player));
    init(p);

	SDL_Event* event = calloc(1, sizeof(SDL_Event));
	int gameState = 0;
    SDL_ShowCursor(SDL_DISABLE);

	while(gameState == 0) {
        if(escape(map, p->xCoord, p->yCoord)) {
            gameState = 0;
            SDL_Quit();
            printf("[EFST-GAME] YOU WIN");
        }
		renderBackground(efstr);
		gameState = handle_events(event, p, efstr->win, map);
		draw(efstr, map, p);
		SDL_RenderPresent(efstr->renderer);
	}

	SDL_Quit();
	return 0;
}