/* Window render management */

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <limits.h>

#include "render.h"
#include "../mapping/map.h"
#include "../player/player.h"
#include "../maths.h"

int initSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL ERROR(Init): %s\n", SDL_GetError());
		return -1;
	}
	return 0;
}

EFST_Renderer* createEFST_Renderer() {
	EFST_Renderer* efstr = calloc(1, sizeof(EFST_Renderer));
	efstr->win = SDL_CreateWindow("Escape From FST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);;
	efstr->renderer = SDL_CreateRenderer(efstr->win, -1, SDL_RENDERER_SOFTWARE);
	return efstr;
}

void initTextures(EFST_Renderer* efstr) {

	//texture from files
	SDL_Texture* sky = SDL_LoadBMP("textures/sky.bmp");
	SDL_Texture* ground = SDL_LoadBMP("textures/ground.bmp");
	SDL_Texture* wall = SDL_LoadBMP("textures/wall.bmp");

	//surfaces from textures
	efstr->sky_texture = SDL_CreateTextureFromSurface(efstr->renderer, sky);
    efstr->ground_texture = SDL_CreateTextureFromSurface(efstr->renderer, ground);
	efstr->wall_texture = SDL_CreateTextureFromSurface(efstr->renderer, wall);
    
	//free mem
	SDL_FreeSurface(sky);
	SDL_FreeSurface(ground);
}

void drawFilledRect(EFST_Renderer* efstr, int x, int y, int w, int h) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_RenderFillRect(efstr->renderer, &rect);
}


void drawCircle(EFST_Renderer * efstr, int32_t centreX, int32_t centreY, int32_t radius) {
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(efstr->renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(efstr->renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(efstr->renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(efstr->renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(efstr->renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(efstr->renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(efstr->renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(efstr->renderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}


void drawMap2D(EFST_Renderer* efstr, int map[MAP_SIZE][MAP_SIZE]) {
	SDL_SetRenderDrawColor(efstr->renderer, 255, 255, 255, 0xFF);
	for(unsigned int i = 0; i < MAP_SIZE; i++) {
		for(unsigned int j = 0; j < MAP_SIZE; j++) {
			if(map[j][i] == FILLED_SLOT) {
				SDL_SetRenderDrawColor(efstr->renderer, 255/2, 255/2, 255/2, 0xFF);
			} else {
				SDL_SetRenderDrawColor(efstr->renderer, 255, 255, 255, 0xFF);
			}
			drawFilledRect(efstr, (WH * i), (WH * j), WH ,WH);
		}
	}
}

void drawPlayer2D(EFST_Renderer* efstr, Player * p) {
	SDL_SetRenderDrawColor(efstr->renderer, 0, 255, 0, 0xFF);
	drawCircle(efstr, WH * p->xCoord + WH / 2, WH * p->yCoord + WH / 2, WH / 4);
	SDL_RenderDrawLine(efstr->renderer, WH * p->xCoord + (WH / 2), WH * p->yCoord + (WH / 2), WH * (p->xCoord + p->dx) + (WH / 2), WH * (p->yCoord + p->dy) + (WH / 2));
}

void drawRay(EFST_Renderer* efstr, Map* m, Player* p) {
	const float fov = M_PI / 3;

	for(size_t i = 0; i < SCREEN_WIDTH; i++) {
		float angle = p->angle - fov / 2 + fov * i / (float) SCREEN_WIDTH;
		SDL_SetRenderDrawColor(efstr->renderer, 50, 50, 50, 0xFF);
		float c = 0;
		float x, y;
		for (; c<20; c+=.05) {
			x = p->xCoord + c*cos(angle);
			y = p->yCoord + c*sin(angle);
			if (m->map[(int)y][(int)x] != EMPTY_SLOT) break;
		}
		int pX = WH * p->xCoord + WH / 2;
		int pY = WH * p->yCoord + WH / 2;
		SDL_RenderDrawLine(efstr->renderer, pX, pY, WH * x, WH * y);

	}

}

void drawMap3D(EFST_Renderer* efstr, Map* m, Player* p) {
	const float fov = M_PI / 3;
	SDL_SetRenderTarget(efstr->renderer,  efstr->wall_texture);

	for(size_t i = 0; i < SCREEN_WIDTH; i++) {
		float angle = p->angle - fov / 2 + fov * i / (float) SCREEN_WIDTH;
		
		float c = 0;
		float x, y;
		for (; c<20; c+=.05) {
			x = p->xCoord + c*cos(angle);
			y = p->yCoord + c*sin(angle);
			SDL_SetRenderDrawColor(efstr->renderer, 80, 90, 95, 0xFF);
			if (m->map[(int)y][(int)x] == FILLED_SLOT) {
				size_t column_height = SCREEN_HEIGHT / c * cos(angle - p->angle);
				/*SDL_Rect srcRect;
				SDL_Rect dstRect;
  				srcRect.x = 0;
				srcRect.y = 0;
				srcRect.w = INT_MAX;
				srcRect.h = INT_MAX;
				dstRect.x = i;
				dstRect.y = SCREEN_HEIGHT / 2 - column_height / 2;
				dstRect.w = 10;
				dstRect.h = column_height;
				SDL_RenderCopy(efstr->renderer, efstr->wall_texture, &srcRect, &dstRect);*/
				SDL_RenderDrawLine(efstr->renderer, i, SCREEN_HEIGHT / 2 - column_height, i, SCREEN_HEIGHT / 2 + column_height / 2);
			}
		}
	}

}

void draw(EFST_Renderer* efstr, Map * m, Player * p) {
	drawMap3D(efstr, m, p);
}

void drawGround(EFST_Renderer* efstr){
  SDL_Rect srcRect;
  SDL_Rect destRect;
  SDL_Rect degradeDestRect;

  srcRect.x = 0;
  srcRect.y = 0; 
  srcRect.w = INT_MAX;
  srcRect.h = INT_MAX;

  destRect.x = 0;
  destRect.y = SCREEN_HEIGHT/2; 
  destRect.w = SCREEN_WIDTH;  
  destRect.h = SCREEN_HEIGHT/2;

  SDL_RenderCopy(efstr->renderer, efstr->ground_texture, &srcRect, &destRect);
}

void drawSky(EFST_Renderer* efstr){
  SDL_Rect srcRect;
  SDL_Rect destRect;
  
  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.w = INT_MAX;
  srcRect.h = INT_MAX;

  destRect.x = 0;
  destRect.y = 0;
  destRect.w = SCREEN_WIDTH;
  destRect.h = SCREEN_HEIGHT/2;

  SDL_RenderCopy(efstr->renderer, efstr->sky_texture, &srcRect, &destRect);
}

void renderBackground(EFST_Renderer* efstr) {
	drawSky(efstr);
	drawGround(efstr);
}