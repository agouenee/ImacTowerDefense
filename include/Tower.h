#ifndef ITD_TOWER___
#define ITD_TOWER___

typedef enum {
	ROCKET, LASER, MITRAILLETTE, HYBRIDE
}TowerType;

typedef struct tower {
	TowerType type;
	unsigned int puissance;
	unsigned int reach;
	unsigned int cadence;
	float price;
}Tower;

#endif