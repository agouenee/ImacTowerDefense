#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "tools.h"
#include "Tower.h"

// Création d'une tour
Tower* createTower(TowerType type, int posX, int posY, int budget) {
	Tower* newTower = (Tower*) malloc(sizeof(Tower));
	if(newTower == NULL) {
		fprintf(stderr, "Error allocation memory");
        exit(1);
	}

	(*newTower).posX = posX;
	(*newTower).posY = posY;

	(*newTower).type = type;
	if(type == ROCKET) {
		(*newTower).puissance = 500;
		(*newTower).reach = 130;		// Portée de 130 pixels
		(*newTower).cadence = 20;		// Un tir toutes les 2s. (20*1/10 de seconde)
		(*newTower).price = 200;		// Prix de 200€
	}
	else if(type == LASER) {
		(*newTower).puissance = 400;
		(*newTower).reach = 100;		// Portée de 100 pixels	
		(*newTower).cadence = 5;		// Un tir toutes les 0.5s. (5*1/10 de seconde)
		(*newTower).price = 300;		// Prix de 300€
	}
	else if(type == MITRAILLETTE) {
		(*newTower).puissance = 200;
		(*newTower).reach = 80;			// Portée de 80 pixels
		(*newTower).cadence = 3;		// Un tir toutes les 0.3s. (3*1/10 de seconde)
		(*newTower).price = 100;		// Prix de 100€
	}
	else if(type == HYBRIDE) {
		(*newTower).puissance = 300;	
		(*newTower).reach = 200;		// Portée de 200 pixels
		(*newTower).cadence = 10;		// Un tir toutes les 1s. (10*1/10 de seconde)
		(*newTower).price = 500;		// Prix de 500€
	}

	(*newTower).next = NULL;

	// Vérification du prix de la tour par rapport au budget
	if(budget >= (*newTower).price) {
		return newTower;
	}
	else {
		return NULL;
	}
}

// Suppression d'une tour
Tower* rmvTower(Tower* t_first, Tower* t) {
	if(t_first == NULL) {
		fprintf(stderr, "pointer is NULL in rmvTower function \n");
		exit(1);
	}

	Tower* root = t_first;
	Tower* t_rmv;

	if(t == t_first) {
		t_rmv = t;
		if((*t_first).next != NULL) {
			t_first = (*t_first).next;
			free(t_rmv);
			return t_first;
		}
		else {
			free(t_rmv);
			return NULL;
		}
	}
	while(t_first != NULL) {
		if((*t_first).next == t) {
			t_rmv = (*t_first).next;
			if((*t_rmv).next != NULL) {
				(*t_first).next = (*t_rmv).next;
			}
			else {
				(*t_first).next = NULL;
				break;
			}
			free(t_rmv);
		}
		t_first = (*t_first).next;
	}

	return root;
}

// Vérification de la validité de l'emplacement de la tour
int checkPosTower(Tower* t_first, int posX, int posY) {
	// Est-ce que la tour est sur une zone constructible ?
	unsigned char pick_col[3];
	glReadPixels(posX, posY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pick_col);
	if(pick_col[0] != 120 && pick_col[1] && pick_col[2]) {
		fprintf(stderr, "zone non constructible\n");
		return 0;
	}

	if(t_first != NULL) {
		Tower* currTower = t_first;
		while(currTower != NULL) {
			// Est-ce que la tour est sur une autre ?
			if(posX >= ((*currTower).posX + 40) || posX <= ((*currTower).posX - 40) || posY >= ((*currTower).posY + 50) || posY <= ((*currTower).posY - 50)) {
				currTower = (*currTower).next;
			}
			else {
				fprintf(stderr, "il y a déjà une tour ici\n");
				return 0;
			}
		}
	}

	return 1;
}

// Détermination du type de tour à construire
TowerType constructTowerType(int posX, int posY) {
	TowerType type = EMPTY;
	// Si clic sur bouton "ROCKET"
	if(posX >= 642 && posX <= 705 && posY >= 150 && posY <= 165) {
		printf("ROCKET !\n");
		type = ROCKET;
	}
	// Si clic sur bouton "MITRAILLETTE"
	if(posX >= 790 && posX <= 854 && posY >= 150 && posY <= 165) {
		printf("MITRAILLETTE !\n");
		type = MITRAILLETTE;
	}
	// Si clic sur bouton "LASER"
	if(posX >= 642 && posX <= 705 && posY >= 12 && posY <= 31) {
		printf("LASER !\n");
		type = LASER;
	}
	// Si clic sur bouton "HYBRIDE"
	if(posX >= 790 && posX <= 854 && posY >= 12 && posY <= 31) {
		printf("HYBRIDE !\n");
		type = HYBRIDE;
	}

	return type;
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
			glColor4ub(255, 255, 255, 255); // Opacité 100%
			glTexCoord2d(0, 0); glVertex2f((*currTower).posX - tourImg->w * 0.5, (*currTower).posY + tourImg->h * 0.5);
			glTexCoord2d(0, 1); glVertex2f((*currTower).posX - tourImg->w * 0.5, (*currTower).posY - tourImg->h * 0.5);
			glTexCoord2d(1, 1); glVertex2f((*currTower).posX + tourImg->w * 0.5, (*currTower).posY - tourImg->h * 0.5);
			glTexCoord2d(1, 0); glVertex2f((*currTower).posX + tourImg->w * 0.5, (*currTower).posY + tourImg->h * 0.5);
		glEnd();
	    
	    glBindTexture(GL_TEXTURE_2D, 0);
	    glDisable(GL_BLEND);
	    glDisable(GL_TEXTURE_2D);

	    currTower = (*currTower).next;
    }

    glDeleteTextures(1, &tourTexture);
    SDL_FreeSurface(tourImg);
}

// Sélection d'une tour construite
Tower* constructTowerSelected(Tower* t_first, int posX, int posY) {
	if(t_first != NULL) {
		Tower* currTower = t_first;
		while(currTower != NULL) {
			// Est-ce que la souris survole une tour ?
			if(posX >= ((*currTower).posX - 30) && posX <= ((*currTower).posX + 30) && posY >= ((*currTower).posY - 30) && posY <= ((*currTower).posY + 30)) {
				return currTower;
			}
			else {
				currTower = (*currTower).next;
			}
		}
	}

	return NULL;
}

// Affichage des caractéristiques de la tour survolée
void displayTowerFeatures(Tower* t) {
	SDL_Surface* featuresImg;
	GLuint featuresTexture;

	if(t != NULL) {
		// Chargement des caractéristiqes de la tour en fonction de son type
		if((*t).type == ROCKET) {
			featuresImg = IMG_Load("images/interface/rocket.png");
		    if(featuresImg == NULL) {
		        fprintf(stderr, "impossible de charger l'image interface/rocket.png \n");
		        exit(1);
		    }
		    featuresTexture = loadTexture("images/interface/rocket.png");
		}
		else if((*t).type == LASER) {
			featuresImg = IMG_Load("images/interface/laser.png");
		    if(featuresImg == NULL) {
		        fprintf(stderr, "impossible de charger l'image interface/laser.png \n");
		        exit(1);
		    }
		    featuresTexture = loadTexture("images/interface/laser.png");		
		}
		else if((*t).type == MITRAILLETTE) {
			featuresImg = IMG_Load("images/interface/mitraillette.png");
		    if(featuresImg == NULL) {
		        fprintf(stderr, "impossible de charger l'image interface/mitraillette.png \n");
		        exit(1);
		    }
		    featuresTexture = loadTexture("images/interface/mitraillette.png");
		}
		else if((*t).type == HYBRIDE) {
			featuresImg = IMG_Load("images/interface/hybride.png");
		    if(featuresImg == NULL) {
		        fprintf(stderr, "impossible de charger l'image interface/hybride.png \n");
		        exit(1);
		    }
		    featuresTexture = loadTexture("images/interface/hybride.png");
		}
		
		// Affichage de la texture sur la carte
		glEnable(GL_TEXTURE_2D);
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glBindTexture(GL_TEXTURE_2D, featuresTexture);
	    
		glBegin(GL_QUADS);
			glColor4ub(255, 255, 255, 255); // Opacité 100%
			glTexCoord2d(0, 0); glVertex2f((*t).posX - featuresImg->w * 0.5 + featuresImg->w / 2.5, (*t).posY + featuresImg->h * 0.5 + featuresImg->h / 2);
			glTexCoord2d(0, 1); glVertex2f((*t).posX - featuresImg->w * 0.5 + featuresImg->w / 2.5, (*t).posY - featuresImg->h * 0.5 + featuresImg->h / 2);
			glTexCoord2d(1, 1); glVertex2f((*t).posX + featuresImg->w * 0.5 + featuresImg->w / 2.5, (*t).posY - featuresImg->h * 0.5 + featuresImg->h / 2);
			glTexCoord2d(1, 0); glVertex2f((*t).posX + featuresImg->w * 0.5 + featuresImg->w / 2.5, (*t).posY + featuresImg->h * 0.5 + featuresImg->h / 2);
		glEnd();
	    
	    glBindTexture(GL_TEXTURE_2D, 0);
	    glDisable(GL_BLEND);
	    glDisable(GL_TEXTURE_2D);
    }

    glDeleteTextures(1, &featuresTexture);
    SDL_FreeSurface(featuresImg);
}

// Détection des monstres par les tours
/*Tower* reachTowerMonster(Tower* t, int posX, int posY) {
	if(t != NULL) {
		Tower* currTower = t;
		while(currTower != NULL) {
			// Calcul de la distance entre le monstre et la tour
			float distance = sqrt((((*currTower).posX - posX)*((*currTower).posX - posX)) + (((*currTower).posY - (600-posY))*((*currTower).posY - (600-posY))));
			// Si la distance est inférieure ou égale à la portée de la tour
			if(distance <= (*currTower).reach) {
				// Retour la puissance de la tour
				return currTower;
			}
			currTower = (*currTower).next;
		}
	}
	
	return NULL;
}*/
