#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "Game.h"
#include "Map.h"
#include "Tower.h"
#include "Monster.h"
#include "tools.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

// 100 ms = 1/10ème de seconde
static const Uint32 FRAMERATE_MILLISECONDS = 100;

GLuint menu;
GLuint gameOver;
GLuint gameWin;
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
	Uint32 actualTime, prevTime, elapsedTime = 0;
	// Initialisation des variables
	int posX, posY;
	int cpt = 1;
	int i = 0;

	// Tours
	int nbTowers = 0;
	int xClicked = 0, yClicked = 0, xOver = 0, yOver = 0;
	int towerTest = 0;

	Tower* t_first = NULL;
	Tower* t_last = NULL;
	Tower* t = NULL;
	Tower* t_selected = NULL;
	TowerType type = EMPTY;
	Tower* t_shoot = NULL;
	int cadence = 1;
	int degat = 0;

	// Monstres
	Monster* monsterToKill;
	Monster* monsterToRmv;

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
	// chargement image game over
	gameOver = loadTexture("images/game-over.jpg");
	// chargement image game win
	gameWin = loadTexture("images/game-win.jpg");

	// Chargement interface joueur (boutons)
	SDL_Surface* interface = IMG_Load("images/interface/buttons.png");
	if(interface == NULL) {
		fprintf(stderr, "Impossible de charger l'image buttons.png\n");
		return EXIT_FAILURE;
	}
	buttons = loadTexture("images/interface/buttons.png");

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

	// Noeuds
	Node* root = map.listNodes;
	Node* first = root;
	// Initialisation de la position des monstres
	posX = root->x;
	posY = root->y;

	// Création du premier monstre
	MonsterType monsterType = BOUTIN;
	Monster* rootMonster = createMonster(monsterType, posX, posY, root->next);
	// Création de la première liste de monstres
	MonsterList currentList;
	currentList.root = rootMonster;
	currentList.nbMonsters = 1;

	// Création du tableau des listes de monstre
	MonsterLists monsterLists;
	monsterLists.nbLists = 1;
	monsterLists.lists[monsterLists.nbLists - 1] = currentList;

	// Initialisation du jeu
	Game game;
	game.start = 0;
	game.over = 0;
	game.win = 0;

	reshape();


	// Boucle événements
	int loop = 1;
	while(loop) {
		root = first;
		actualTime = SDL_GetTicks();
		glClear(GL_COLOR_BUFFER_BIT);

		if(game.start == 0) {
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
		else if(game.over == 1) {
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, gameOver);
				glBegin(GL_QUADS);
					glTexCoord2d(0, 0); glVertex2f(0, WINDOW_HEIGHT);
					glTexCoord2d(0, 1); glVertex2f(0, 0);
					glTexCoord2d(1, 1); glVertex2f(WINDOW_WIDTH, 0);
					glTexCoord2d(1, 0); glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		else if(game.win == 1) {
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, gameWin);
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
					glColor4ub(255, 255, 255, 255); // Opacité 100%
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
			if(cpt%40 == 0) {
				// Création d'un nouveau monstre
				Monster* newMonster = createMonster(monsterType, posX, posY, root->next);

				if(cpt%150 == 0 && monsterLists.nbLists < NB_MONSTER_LIST_MAX) {
					MonsterList newList;
					rootMonster = newMonster;
					newList.root = rootMonster;
					currentList = newList;
					currentList.nbMonsters = 1;

					monsterLists.nbLists += 1;
					monsterLists.lists[monsterLists.nbLists - 1] = currentList;

				}
				else if(currentList.nbMonsters < 5) {
					// Ajout du monstre
					rootMonster = addMonster(rootMonster, newMonster);
					monsterLists.lists[monsterLists.nbLists - 1].root = rootMonster;
					currentList.nbMonsters += 1;
				}
			}
			cpt++;

			if(drawMonsters(monsterLists) == 0) {
				//game.over = 1; 
			}

			// Tours
			if(t_first != NULL) {
				// Construction de la tour
				constructTower(t_first);
				// Affichage des caractéristiques de la tour survolée
				t_selected = constructTowerSelected(t_first, xOver, yOver);
				if(t_selected != NULL) {
					displayTowerFeatures(t_selected);
				}
				int j =0;
				// Attaque des tours
				for(i = 0; i < monsterLists.nbLists; i++) {
					monsterToKill = monsterLists.lists[i].root;
					j = 0;
					while(monsterToKill != NULL) {
						// Test de la distance tour/monstre
						t_shoot = reachTowerMonster(t_first, (*monsterToKill).posX, (*monsterToKill).posY);
						// Si le monstre est à la portée de la tour
						if(t_shoot != NULL) {
							if(cadence%(*t_shoot).cadence == 0) {
								if((*monsterToKill).life > 0) {
									// Calcul du nb de points de vie enlevés (moyenne puissance tour et résistance monstre)
									degat = ((*t_shoot).puissance/100 + ((*monsterToKill).resistance/100)) / 2;
									(*monsterToKill).life -= degat;
								}
							}
							cadence++;
							//t_shoot = NULL;
						}
						// Suppression des monstres
						if((*monsterToKill).life <= 0) {
							printf("monster n %d\n",j);
							monsterToRmv = monsterToKill;
							monsterToKill = (*monsterToKill).next;
							monsterLists.lists[i].root = rmvMonster(monsterLists.lists[i].root, monsterToRmv);
							printf("%d\n", countMonsters(monsterLists.lists[i].root));
							printf("DEAD !\n");
						}
						else {
							monsterToKill = (*monsterToKill).next;
						}
						j++;
					}
				}
			}
		}

		SDL_GL_SwapBuffers();
		/* ****** */

		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				loop = 0;
				break;
			}
			// Mouvement souris (survol)
			if(type != EMPTY) {
				if(e.type == SDL_MOUSEMOTION) {
					xOver = e.motion.x;
	        		yOver = 600-e.motion.y;
					break;
				}
			}

			switch(e.type) {
				case SDL_MOUSEBUTTONDOWN:
					switch(e.button.button) {
						case SDL_BUTTON_LEFT:
							xClicked = e.button.x;
							yClicked = 600-e.button.y;
							/*printf("%d %d\n", xClicked, yClicked);*/
							// Si clic dans l'interface joueur
							if(xClicked >= 600) {
								// Sélection du type de tour à construire
								type = constructTowerType(xClicked, yClicked);
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
							game.start = 1;
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
		elapsedTime = actualTime - prevTime;
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}
		prevTime = actualTime;
	}

	// Destruction des données des images chargées
	SDL_FreeSurface(background);
	SDL_FreeSurface(interface);

	SDL_Quit();
	return EXIT_SUCCESS;
}
