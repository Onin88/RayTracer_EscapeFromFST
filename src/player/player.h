#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../mapping/map.h"

typedef struct {
	float xCoord, yCoord, dx, dy, angle;
} Player;

void init(Player *p);
void freePlayer(Player* p);

void fixAngle(Player* p);


#endif