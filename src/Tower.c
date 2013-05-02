#include "Tower.h"

// Création d'une tour
Tower* createTower(TowerType type) {
	Tower* newTower = (Tower*)malloc(sizeof(Tower));
	if(newTower == NULL) {
		fprintf(stderr, "Error allocation memory");
        exit(1);
	}

	(*newTower).type = type;
	if(type == ROCKET) {
		(*newTower).puissance = 100;
		(*newTower).reach = 60;			// Portée de 60 pixels
		(*newTower).cadence = 100;		// Un tir toutes les 10s.
		(*newTower).price = 200.0;		// Prix de 200€
		printf("ROCKET: %d\n", type);
	}
	else if(type == LASER) {
		(*newTower).puissance = 60;
		(*newTower).reach = 40;			// Portée de 40 pixels	
		(*newTower).cadence = 10;		// Un tir toutes les 1s.
		(*newTower).price = 300.0;		// Prix de 300€
		printf("LASER: %d\n", type);
	}
	else if(type == MITRAILLETTE) {
		(*newTower).puissance = 30;
		(*newTower).reach = 30;			// Portée de 30 pixels
		(*newTower).cadence = 30;		// Un tir toutes les 3s.
		(*newTower).price = 100.0;		// Prix de 100€
		printf("MITRAILLETTE: %d\n", type);
	}
	else { // HYBRIDE
		(*newTower).puissance = 40;	
		(*newTower).reach = 100;		// Portée de 100 pixels
		(*newTower).cadence = 50;		// Un tir toutes les 5s.
		(*newTower).price = 500.0;		// Prix de 500€
		printf("HYBRIDE: %d\n", type);
	}

	return newTower;
}