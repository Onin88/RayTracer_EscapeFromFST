#include <stdio.h>
#include <stdlib.h>
#include "map.h"

// creer une map
Map* createMap() {
	Map* m = calloc(2, sizeof(Map));

	unsigned int i, j;
	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			m->map[i][j]= EMPTY_SLOT;
		}
	}

	return m;
}

Map* defaultMap() {
	Map* m = calloc(2, sizeof(Map));

	unsigned int i, j;
	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			if(i == 0 || j == 0 || i == MAP_SIZE - 1 || j == MAP_SIZE - 1) {
				m->map[i][j]= FILLED_SLOT;
			} else {
				m->map[i][j]= EMPTY_SLOT;
			}
		}
	}

	return m;
}

// liberer l'espace memoire de la map
void freeMap(Map* map) {
	unsigned int i, j;
	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
				free(map->map[i][j]);
		}
	}
	free(map);
}

// affiche la carte
void printMap(Map* m) {
	unsigned int i, j;
	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
				printf("%d", m->map[j][i]);
		}
		printf("\n");
	}
}

int canBeMovedTo(Map* m, float x, float y) {
	return (x > 0 && x < MAP_SIZE && y > 0 && y < MAP_SIZE && m->map[(int) y][(int) x] != FILLED_SLOT) ? 1 : 0;
}

int escape(Map* m, float x, float y) {
	return m->map[(int) y][(int) x] == 2;
}

void readMapFromFile(Map* m, const char* filename) {
    FILE *fp = fopen(filename, "r");
    int counterx = 0, countery = 0;
    char ch;
    if (fp == NULL) {
        printf("File is not available \n");
    } else {
        while ((ch = fgetc(fp)) != EOF)
        {
            if(ch == '1' || ch == '0') {
                m->map[counterx][countery] = ch - '0';
                if(counterx == 15) {
                    counterx = 0;
                    countery++;
                } else {
                    counterx += 1;
                }
            }
        }
    }
    fclose(fp);
}