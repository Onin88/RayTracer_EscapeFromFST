#ifndef _RENDER_H_
#define _RENDER_H_

#include "../mapping/map.h"
#include "../player/player.h"

//SCREEN DIMENSION CONSTANTS
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 512
#define WH 512 / MAP_SIZE

int initSDL();

typedef struct {

    SDL_Renderer* renderer;
    SDL_Window* win;

    SDL_Texture* sky_texture;
    SDL_Texture* ground_texture;
    SDL_Texture* wall_texture;
    
} EFST_Renderer;

EFST_Renderer* createEFST_Renderer();
void initTextures(EFST_Renderer* efstr);

void drawFilledRect(EFST_Renderer* efst_renderer, int x, int y, int w, int h);
void drawCircle(EFST_Renderer * efst_renderer, int32_t centreX, int32_t centreY, int32_t radius);
void drawMap2D(EFST_Renderer* efst_renderer, int map[MAP_SIZE][MAP_SIZE]);
void drawPlayer2D(EFST_Renderer* efst_renderer, Player * p);
void draw(EFST_Renderer* efst_renderer, Map * m, Player * p);
void renderBackground(EFST_Renderer* efst_renderer);


#endif