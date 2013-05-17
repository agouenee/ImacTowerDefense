#ifndef ITD_TOWER___
#define ITD_TOWER___

#include "stdlib.h"
#include "stdio.h"

typedef enum {
	ROCKET, LASER, MITRAILLETTE, HYBRIDE, EMPTY
}TowerType;

typedef struct tower {
	int posX;					// position de la tour sur la carte
	int posY;
	TowerType type;
	unsigned int puissance;		// valeurs entre 0 et 100 (puissance, portée et cadence)
	unsigned int reach;
	unsigned int cadence;
	int price;				// valeurs entre 0.0 et 500.0
	struct tower* next;
}Tower;

Tower* createTower(TowerType type, int posX, int posY);
int checkPosTower(Tower* t_first, int posX, int posY);
TowerType constructTowerType(int posX, int posY);
void constructTower(Tower* t_first);
Tower* constructTowerSelected(Tower* t_first, int posX, int posY);
void displayTowerFeatures(Tower* t);
Tower* reachTowerMonster(Tower* t, int posX, int posY);

#endif