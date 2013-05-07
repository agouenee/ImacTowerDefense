#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "Map.h"
#include "Tower.h"
#include "Monster.h"
#include "tools.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

// 1 image pour 100 ms (0.1 secondes) = 10 images pour 1000 ms
static const Uint32 FRAMERATE_MILLISECONDS = 10 / 1000;

GLuint menu;
GLuint buttons;
GLuint mapBackground;
GLuint texture;

void reshape() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0., 900., 0., 600.);
}

int main(int argc, char** argv) {
	// Initialisation des variables
	int game = 0;
	int posX, posY;
	int cpt = 0;

	int nbTowers = 0;
	int xClicked = 0, yClicked = 0;
	int towerTest = 0;

	Tower* t_first = NULL;
	Tower* t_last = NULL;
	Tower* t = NULL;
	TowerType type = EMPTY;

	int nbMonsters = 0;

	// Initialisation SDL
	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
		return EXIT_FAILURE;
	}
	SDL_Surface* screen = NULL;
	if(NULL == (screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_OPENGL | SDL_RESIZABLE))) {
		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
		return EXIT_FAILURE;
	}

	SDL_WM_SetCaption("ImacTowerDefense", NULL);

	// Chargement menu
	menu = loadTexture("images/menu.jpg");

	// Chargement interface joueur (boutons)
	SDL_Surface* interface = IMG_Load("images/buttons.png");
	if(interface == NULL) {
		fprintf(stderr, "Impossible de charger l'image buttons.png\n");
		return EXIT_FAILURE;
	}
	buttons = loadTexture("images/buttons.png");

	// Chargement carte itd
	Map map = loadMap("data/map-test.itd");
	// Chargement carte ppm
	char fileName[256] = "images/"; strcat(fileName, map.image);
	SDL_Surface* background = IMG_Load(fileName);
	if(background == NULL) {
		fprintf(stderr, "Impossible de charger l'image %s\n", fileName);
		return EXIT_FAILURE;
	}
	mapBackground = loadTexture(fileName);

	// Monstres
	SDL_Surface* boutin = IMG_Load("images/boutin.png");
	if(boutin == NULL) {
		fprintf(stderr, "impossible de charger l'image boutin.png \n");
		return EXIT_FAILURE;
	}
	texture = loadTexture("images/boutin.png");
	// Noeuds
	Node* root = map.listNodes;
	Node* node = root;
	Node* first = root;
	// Initialisation de la position des monstres
	posX = node->x;
	posY = node->y;

	MonsterType monsterType = BOUTIN;
	Monster* rootMonster = createMonster(monsterType, posX, posY, node->next);

	reshape();


	// Boucle événements
	int loop = 1;
	while(loop) {
		root = first;
		Uint32 startTime = SDL_GetTicks();
		glClear(GL_COLOR_BUFFER_BIT);

		if(game == 0) {
			// Menu
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, menu);
				glBegin(GL_QUADS);
					glTexCoord2d(0, 0); glVertex2f(0, WINDOW_HEIGHT);
					glTexCoord2d(0, 1); glVertex2f(0, 0);
					glTexCoord2d(1, 1); glVertex2f(WINDOW_WIDTH, 0);
					glTexCoord2d(1, 0); glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		else {
			// Interface joueur (boutons)
			glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBindTexture(GL_TEXTURE_2D, buttons);

				glBegin(GL_QUADS);
					glTexCoord2d(0, 0); glVertex2f(600, interface->h);
					glTexCoord2d(0, 1); glVertex2f(600, 0);
					glTexCoord2d(1, 1); glVertex2f(interface->w + 600, 0);
					glTexCoord2d(1, 0); glVertex2f(interface->w + 600, interface->h);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);

			// Carte
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, mapBackground);
				glBegin(GL_QUADS);
					glTexCoord2d(0, 0); glVertex2f(0, background->h);
					glTexCoord2d(0, 1); glVertex2f(0, 0);
					glTexCoord2d(1, 1); glVertex2f(background->w, 0);
					glTexCoord2d(1, 0); glVertex2f(background->w, background->h);
				glEnd();
			glDisable(GL_TEXTURE_2D);
			
			// Dessin du chemin
			glColor3ub(map.pathColor.r, map.pathColor.g, map.pathColor.b);
			drawPath(root);

			// Monstres
			if(cpt%50 == 0 && nbMonsters < 3) {
				Monster* newMonster = createMonster(monsterType, posX, posY, root->next);
				rootMonster = addMonster(rootMonster, newMonster);
				nbMonsters++;
			}
			cpt++;
			drawMonsters(rootMonster);
		}

		// Tours
		if(t_first != NULL) {
			constructTower(t_first);
		}

		SDL_GL_SwapBuffers();
		/* ****** */

		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				loop = 0;
				break;
			}
			switch(e.type) {
				case SDL_MOUSEBUTTONDOWN:
					switch(e.button.button) {
						case SDL_BUTTON_LEFT:
							xClicked = e.button.x;
							yClicked = 600-e.button.y;
							/*printf("%d %d\n", xClicked, yClicked);*/
							// Si clic dans l'interface joueur)
							if(xClicked >= 600) {
								// Si clic sur bouton "ROCKET"
								if(xClicked >= 606 && xClicked <= 747 && yClicked >= 43 && yClicked <= 73) {
									printf("ROCKET !\n");
									type = ROCKET;
								}
								// Si clic sur bouton "MITRAILLETTE"
								if(xClicked >= 753 && xClicked <= 893 && yClicked >= 43 && yClicked <= 73) {
									printf("MITRAILLETTE !\n");
									type = MITRAILLETTE;
								}
								// Si clic sur bouton "LASER"
								if(xClicked >= 606 && xClicked <= 747 && yClicked >= 7 && yClicked <= 37) {
									printf("LASER !\n");
									type = LASER;
								}
								// Si clic sur bouton "HYBRIDE"
								if(xClicked >= 753 && xClicked <= 893 && yClicked >= 7 && yClicked <= 37) {
									printf("HYBRIDE !\n");
									type = HYBRIDE;
								}
							}
							// Si clic sur la carte
							else {
								if(type != EMPTY) {
									// Création de la première tour
									if(nbTowers == 0) {
										// Vérification de la position
										towerTest = checkPosTower(t_first, xClicked, yClicked);
										if(towerTest == 1) {
											t_first = createTower(type, xClicked, yClicked);
											t_last = t_first;
											nbTowers++;
										}
									}
									// Autres tours
									else if(nbTowers >= 1) {
										// Vérification de la position
										towerTest = checkPosTower(t_first, xClicked, yClicked);
										if(towerTest == 1) {
											t = createTower(type, xClicked, yClicked);
											(*t_last).next = t;
											t_last = t;
											nbTowers++;
										}
									}      
								}
								else {
									printf("Sélectionner une tour à construire !\n");
								}
							}
							break;
						case SDL_BUTTON_RIGHT:
							break;
						default:
							break;
					}
					break;
				/*case SDL_VIDEORESIZE:
				WIDTH = e.resize.w;
				HEIGHT = e.resize.h;
				setVideoMode();
				break;*/
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym) {
						case 's' :
							game = 1;
							break;
						case 'q' :
							loop = 0;
							break;
						case SDLK_ESCAPE: 
							loop = 0;
							break;
						default: 
							break;
					}
					break;
				default:
					break;
			}
		}
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}
	}

	// Destruction des données des images chargées
	SDL_FreeSurface(background);
	SDL_FreeSurface(boutin);

	SDL_Quit();
	return EXIT_SUCCESS;
}
