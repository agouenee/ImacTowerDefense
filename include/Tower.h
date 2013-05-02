#ifndef ITD_TOWER___
#define ITD_TOWER___

#include "stdlib.h"
#include "stdio.h"

typedef enum {
	ROCKET, LASER, MITRAILLETTE, HYBRIDE
}TowerType;

typedef struct tower {
	TowerType type;
	unsigned int puissance;		// valeurs entre 0 et 100 (puissance, portée et cadence)
	unsigned int reach;
	unsigned int cadence;
	float price;				// valeurs entre 0.0 et 500.0
}Tower;

Tower* createTower(TowerType type);

#endif