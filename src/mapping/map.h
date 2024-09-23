#ifndef _MAP_H_
#define _MAP_H_

#define MAP_SIZE 16
#define EMPTY_SLOT 0
#define FILLED_SLOT 1
#define EXIT_SLOT 2

typedef struct {
	int map[MAP_SIZE][MAP_SIZE];
	int players;
} Map;

Map* createMap();
Map* defaultMap();
void freeMap(Map* map);
void printMap(Map* map);
int canBeMovedTo(Map* m, float x, float y);
void readMapFromFile(Map* m, const char* filename);
int escape(Map* m, float x, float y);

#endif