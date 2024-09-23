#include "player.h"
#include <math.h>

void init(Player *p) {
    p->xCoord = 5;
    p->yCoord = 5;
    p->dx = cos(p->angle);
    p->dy = sin(p->angle);
}

void fixAngle(Player* p) {
    if(p->angle < 0) {
        p->angle += 2 * M_PI;
    } else if(p->angle > 2 * M_PI) {
        p->angle -= 2 * M_PI;
    }
    p->dx = cos(p->angle);
    p->dy = sin(p->angle);
}

void freePlayer(Player* p) {
	free(p);
}