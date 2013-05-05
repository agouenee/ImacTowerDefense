#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "tools.h"
#include "Tower.h"

// Création d'une tour
Tower* createTower(TowerType type, int posX, int posY) {
	Tower* newTower = (Tower*) malloc(sizeof(Tower));
	if(newTower == NULL) {
		fprintf(stderr, "Error allocation memory");
        exit(1);
	}

	(*newTower).posX = posX;
	(*newTower).posY = posY;

	(*newTower).type = type;
	if(type == ROCKET) {
		(*newTower).puissance = 100;
		(*newTower).reach = 60;			// Portée de 60 pixels
		(*newTower).cadence = 100;		// Un tir toutes les 10s.
		(*newTower).price = 200.0;		// Prix de 200€
	}
	else if(type == LASER) {
		(*newTower).puissance = 60;
		(*newTower).reach = 40;			// Portée de 40 pixels	
		(*newTower).cadence = 10;		// Un tir toutes les 1s.
		(*newTower).price = 300.0;		// Prix de 300€
	}
	else if(type == MITRAILLETTE) {
		(*newTower).puissance = 30;
		(*newTower).reach = 30;			// Portée de 30 pixels
		(*newTower).cadence = 30;		// Un tir toutes les 3s.
		(*newTower).price = 100.0;		// Prix de 100€
	}
	else if(type == HYBRIDE) {
		(*newTower).puissance = 40;	
		(*newTower).reach = 100;		// Portée de 100 pixels
		(*newTower).cadence = 50;		// Un tir toutes les 5s.
		(*newTower).price = 500.0;		// Prix de 500€
	}
	/*else { // EMPTY
		fprintf(stderr, "type EMPTY \n");
        return NULL;
	}*/

	(*newTower).next = NULL;

	return newTower;
}

// Vérification de la validité de l'emplacement de la tour
int checkPosTower(Tower* t_first, int posX, int posY) {
	Tower* currTower = t_first;
	while(currTower != NULL) {
		if(posX >= ((*currTower).posX + 30) || posX <= ((*currTower).posX - 30) || posY >= ((*currTower).posY + 30) || posY <= ((*currTower).posY - 30)) {
			currTower = (*currTower).next;
		}
		else {
			fprintf(stderr, "il y a déjà une tour ici\n");
			return 0;
		}
	}
	return 1;
}

// Construction (affichage) des tours sur la carte
void constructTower(Tower* t_first) {
	SDL_Surface* tourImg;
	GLuint tourTexture;
	Tower* currTower = t_first;

	while(currTower != NULL) {
		// Chargement de l'image de la tour en fonction de son type
		if((*currTower).type == ROCKET) {
			tourImg = IMG_Load("images/towers/rocket.png");
		    if(tourImg == NULL) {
		        fprintf(stderr, "impossible de charger l'image rocket.png \n");
		        exit(1);
		    }
		    tourTexture = loadTexture("images/towers/rocket.png");
		}
		else if((*currTower).type == LASER) {
			tourImg = IMG_Load("images/towers/laser.png");
		    if(tourImg == NULL) {
		        fprintf(stderr, "impossible de charger l'image laser.png \n");
		        exit(1);
		    }
		    tourTexture = loadTexture("images/towers/laser.png");		
		}
		else if((*currTower).type == MITRAILLETTE) {
			tourImg = IMG_Load("images/towers/mitraillette.png");
		    if(tourImg == NULL) {
		        fprintf(stderr, "impossible de charger l'image mitraillette.png \n");
		        exit(1);
		    }
		    tourTexture = loadTexture("images/towers/mitraillette.png");
		}
		else if((*currTower).type == HYBRIDE) {
			tourImg = IMG_Load("images/towers/hybride.png");
		    if(tourImg == NULL) {
		        fprintf(stderr, "impossible de charger l'image hybride.png \n");
		        exit(1);
		    }
		    tourTexture = loadTexture("images/towers/hybride.png");
		}
		
		// Affichage de la texture sur la carte
		glEnable(GL_TEXTURE_2D);
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glBindTexture(GL_TEXTURE_2D, tourTexture);
	    
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2f((*currTower).posX - tourImg->w * 0.5, 600 - (*currTower).posY - tourImg->h * 0.5);
			glTexCoord2d(0, 1); glVertex2f((*currTower).posX - tourImg->w * 0.5, 600 - (*currTower).posY + tourImg->h * 0.5);
			glTexCoord2d(1, 1); glVertex2f((*currTower).posX + tourImg->w * 0.5, 600 - (*currTower).posY + tourImg->h * 0.5);
			glTexCoord2d(1, 0); glVertex2f((*currTower).posX + tourImg->w * 0.5, 600 - (*currTower).posY - tourImg->h * 0.5);
		glEnd();
	    
	    glBindTexture(GL_TEXTURE_2D, 0);
	    glDisable(GL_BLEND);
	    glDisable(GL_TEXTURE_2D);

	    currTower = (*currTower).next;
    }
}
