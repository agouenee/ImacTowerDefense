#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
GLuint figures;
GLuint mapBackground;
GLuint pauseBackground;
GLuint helpBackground;
GLuint texture;

void reshape() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0., 900., 0., 600.);
}

int main(int argc, char** argv) {
	// Initialisation des variables
	int posX, posY;
	int cpt = 1;
	int i = 0;
	Uint32 actualTime, prevTime, elapsedTime = 0;
	srand(time(NULL));

	// Tours
	int nbTowers = 0;
	int xClicked = 0, yClicked = 0, xClickedRight = 0, yClickedRight = 0, xOver = 0, yOver = 0;
	int towerTest = 0;

	Tower* t_first = NULL;
	Tower* t_last = NULL;
	Tower* t = NULL;
	Tower* t_selected = NULL;
	Tower* t_rmv = NULL;
	TowerType type = EMPTY;
	Tower* t_shoot = NULL;
	int cadence = 1;
	int degat = 0;

	// Monstres
	Monster* monsterToKill;
	Monster* monsterToRmv;
	int monsterTypeInt = 0;

	if(argc <= 1) {
		fprintf(stderr, "Veuillez indiquer la carte .itd à charger\n");
		return EXIT_FAILURE;
	}

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
	// Texture chiffres budget
	SDL_Surface* figuresIMG = IMG_Load("images/interface/figures.png");
	if(figuresIMG == NULL) {
		fprintf(stderr, "Impossible de charger l'image figures.png\n");
		return EXIT_FAILURE;
	}
	figures = loadTexture("images/interface/figures.png");
	// Chargement écran pause
	SDL_Surface* pause = IMG_Load("images/interface/pause.png");
	if(pause == NULL) {
		fprintf(stderr, "Impossible de charger l'image pause.png\n");
		return EXIT_FAILURE;
	}
	pauseBackground = loadTexture("images/interface/pause.png");
	// Chargement écran aide
	SDL_Surface* help = IMG_Load("images/interface/help.png");
	if(help == NULL) {
		fprintf(stderr, "Impossible de charger l'image help.png\n");
		return EXIT_FAILURE;
	}
	helpBackground = loadTexture("images/interface/help.png");

	// Chargement carte itd
	printf("%d\n", argc);
	char itdFile[256] = "data/"; strcat(itdFile, argv[1]); /* argv[1] = 1er argument passé au programme à son exécution */
	Map map = loadMap(itdFile);

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
	MonsterList* currentList = createMonsterList();
	(*currentList).root = rootMonster;
	(*currentList).nbMonsters = 1;
	(*currentList).nbMonstersSend = 1;

	// Création du tableau des listes de monstre
	MonsterLists monsterLists;
	monsterLists.nbLists = 1;
	monsterLists.lists[monsterLists.nbLists - 1] = currentList;

	// Initialisation du jeu
	Game game;
	game.start = 0;
	game.pause = 0;
	game.help = 0;
	game.over = 0;
	game.win = 0;
	game.budget = 400;
	game.nbListsSend = 1;
	game.nbListsKilled = 0;

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

			// Budget
			displayBudget(figuresIMG, figures, game.budget);

			// Vagues de monstres éliminées
			displayWave(figuresIMG, figures, game.nbListsSend);

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

			// Ecran jeu en pause
			if(game.pause == 1) {
				glEnable(GL_TEXTURE_2D);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glBindTexture(GL_TEXTURE_2D, pauseBackground);

					glBegin(GL_QUADS);
						glTexCoord2d(0, 0); glVertex2f(0, pause->h);
						glTexCoord2d(0, 1); glVertex2f(0, 0);
						glTexCoord2d(1, 1); glVertex2f(pause->w, 0);
						glTexCoord2d(1, 0); glVertex2f(pause->w, pause->h);
					glEnd();

					glBindTexture(GL_TEXTURE_2D, 0);
					glDisable(GL_BLEND);					
				glDisable(GL_TEXTURE_2D);
			}

			// Ecran jeu aide
			if(game.help == 1) {
				glEnable(GL_TEXTURE_2D);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glBindTexture(GL_TEXTURE_2D, helpBackground);

					glBegin(GL_QUADS);
						glTexCoord2d(0, 0); glVertex2f(0, help->h);
						glTexCoord2d(0, 1); glVertex2f(0, 0);
						glTexCoord2d(1, 1); glVertex2f(help->w, 0);
						glTexCoord2d(1, 0); glVertex2f(help->w, help->h);
					glEnd();

					glBindTexture(GL_TEXTURE_2D, 0);
					glDisable(GL_BLEND);					
				glDisable(GL_TEXTURE_2D);
			}

		if(game.pause == 0) {
			// Monstres
			if(cpt%40 == 0) {
				// Création d'un nouveau monstre
				// Type aléatoire
				monsterTypeInt = rand()%2;
				if(monsterTypeInt == 0) {
					monsterType = BOUTIN;
				}
				else {
					monsterType = BARJOT;
				}
				Monster* newMonster = createMonster(monsterType, posX, posY, root->next);
				// Nouvelle liste de monstre
				if(cpt%150 == 0 && game.nbListsSend < NB_MONSTER_LIST_MAX) {
					MonsterList* newList = createMonsterList();
					rootMonster = newMonster;
					(*newList).root = rootMonster;
					(*newList).nbMonsters = 1;
					(*newList).nbMonstersSend = 1;
					currentList = newList;

					monsterLists.nbLists += 1;
					game.nbListsSend += 1;
					monsterLists.lists[monsterLists.nbLists - 1] = currentList;
				}
				else if((*currentList).nbMonstersSend < 5) {
					// Ajout du monstre à la liste actuelle
					rootMonster = addMonster(rootMonster, newMonster);
					monsterLists.lists[monsterLists.nbLists - 1]->root = rootMonster;
					(*currentList).nbMonsters += 1;
					(*currentList).nbMonstersSend += 1;
				}
			}
			cpt++;
		
			// Affichage des monstres
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

				// Attaque des tours
				Tower* currTower = t_first;
				while(currTower != NULL) {
					// Récupération de la liste de monstres à la portée de la tour
					MonsterToReach* m = reachTowerMonster(currTower, monsterLists);
					if(m != NULL) {
						// Détermination du monstre le plus proche de la tour
						MonsterToReach* closest = m;
						while(m != NULL) {
							if((*m).distance < (*closest).distance) {
								closest = m;
							}
							m = (*m).next;
						}
						// Tir de la tour en fonction de sa cadence
						if(cadence%(*currTower).cadence == 0) {
							// Si le monstre le plus proche de la tour n'est pas encore décédé (=D)
							if((*closest).monster->life > 0) {
								// Calcul du nb de points de vie enlevés (moyenne puissance tour et résistance monstre)
								degat = ((*currTower).puissance/100 + ((*closest).monster->resistance/100)) / 2;
								(*closest).monster->life -= degat;
							}
							// Suppression des monstres
							if((*closest).monster->life <= 0) {
								monsterLists.lists[(*closest).listNum]->nbMonsters -= 1;
								monsterToRmv = (*closest).monster;
								if(monsterLists.lists[(*closest).listNum]->nbMonsters == 0) {
									rmvMonsterList(&monsterLists, (*closest).listNum);
									game.nbListsKilled += 1;
								}
								else {
									//monsterToKill = (*monsterToKill).next;
									monsterLists.lists[(*closest).listNum]->root = rmvMonster(monsterLists.lists[(*closest).listNum]->root, monsterToRmv);
								}
								printf("DEAD !\n");
								// Gain d'argent en fonction de la vague du monstre
								game.budget += game.nbListsSend * 5;
							}
						}
						cadence++;
					}
					currTower = (*currTower).next;
				}
				if(game.nbListsKilled == NB_MONSTER_LIST_MAX) {
					game.win = 1;
				}

				/*int j =0;
				// Attaque des tours
				for(i = 0; i < monsterLists.nbLists; i++) {
					monsterToKill = monsterLists.lists[i]->root;
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
							monsterLists.lists[i]->nbMonsters -= 1;
							monsterToRmv = monsterToKill;
							if(monsterLists.lists[i]->nbMonsters == 0) {
								rmvMonsterList(&monsterLists, i);
							}
							else {
								monsterToKill = (*monsterToKill).next;
								monsterLists.lists[i]->root = rmvMonster(monsterLists.lists[i]->root, monsterToRmv);
							}
							printf("DEAD !\n");
						}
						else {
							monsterToKill = (*monsterToKill).next;
						}
						j++;
					}
				}*/

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
					// Clic gauche
					case SDL_BUTTON_LEFT:
						xClicked = e.button.x;
						yClicked = 600-e.button.y;
						//printf("%d %d\n", xClicked, yClicked);
						// Si clic dans l'interface joueur
						if(xClicked >= 600) {
							// Clic sur pause/play
							if(xClicked >= 720 && xClicked <= 745 && yClicked >= 516 && yClicked <= 542) {
								game.pause = 1;
								printf("Jeu en pause\n");
							}
							else if(xClicked >= 768 && xClicked <= 790 && yClicked >= 516 && yClicked <= 542) {
								game.pause = 0;
							}
							// Sélection du type de tour à construire
							if(game.pause == 0) {
								type = constructTowerType(xClicked, yClicked);									
							}
						}
						// Si clic sur la carte
						else {
							if(type != EMPTY && game.pause == 0) {
								// Création de la première tour
								if(nbTowers == 0) {
									// Vérification de la position
									towerTest = checkPosTower(t_first, xClicked, yClicked);
									if(towerTest == 1) {
										t_first = createTower(type, xClicked, yClicked, game.budget);
										// Vérification du prix
										if(t_first != NULL) {
											t_last = t_first;
											game.budget -= (*t_first).price;
											nbTowers++;
										}
										else {
											printf("Pas assez de budget !\n");
										}
									}
								}
								// Autres tours
								else if(nbTowers >= 1) {
									// Vérification de la position
									towerTest = checkPosTower(t_first, xClicked, yClicked);
									if(towerTest == 1) {
										t = createTower(type, xClicked, yClicked, game.budget);
										// Vérification du prix
										if(t != NULL) {
											(*t_last).next = t;
											t_last = t;
											game.budget -= (*t).price;
											nbTowers++;												
										}
										else {
											printf("Pas assez de budget !\n");
										}
									}
								}
							}
							else if(game.pause == 0) {
								printf("Sélectionner une tour à construire !\n");
							}
						}
						break;

					// Clic droit
					case SDL_BUTTON_RIGHT:
						xClickedRight = e.button.x;
						yClickedRight = 600-e.button.y;
						// Si clic sur la carte
						if(xClicked < 600) {
							// Si au moins une tour a été construite
							if(t_first != NULL && game.pause == 0) {
								// Sélection de la tour à supprimer
								t_rmv = constructTowerSelected(t_first, xClickedRight, yClickedRight);
								// Suppression d'une tour
								if(t_rmv != NULL) {
									t_first = rmvTower(t_first, t_rmv);
									// Mise à jour du t_last (dernière tour de la liste)
									if(t_first != NULL) {
										Tower* new_last = t_first;
										while((*new_last).next != NULL) {
											new_last = (*new_last).next;
										}
										t_last = new_last;
									}
									game.budget += (*t_rmv).price;
									nbTowers--;
								}
							}
						}
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
					case 'h' :
						if(game.help == 0) {
							game.help = 1;
							game.pause = 1;
						}
						else {
							game.help = 0;
							game.pause = 0;
						}
						break;
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

	// Libération mémoire
	free(t_first);
	free(t_last);
	free(t);
	/*free(t_selected);
	//free(t_rmv);
	free(t_shoot);*/
	
	/*free(monsterToKill);
	free(monsterToRmv);*/

	/*free(root);
	free(first);*/

	free(rootMonster);
	free(currentList);

	// Suppression des textures
	glDeleteTextures(1, &menu);
	glDeleteTextures(1, &gameOver);
	glDeleteTextures(1, &gameWin);
	glDeleteTextures(1, &buttons);
	glDeleteTextures(1, &figures);
	glDeleteTextures(1, &mapBackground);
	glDeleteTextures(1, &pauseBackground);
	glDeleteTextures(1, &helpBackground);
	glDeleteTextures(1, &texture);
	// Destruction des données des images chargées
	SDL_FreeSurface(background);
	SDL_FreeSurface(interface);
	SDL_FreeSurface(figuresIMG);
	SDL_FreeSurface(pause);
	SDL_FreeSurface(help);

	SDL_Quit();
	return EXIT_SUCCESS;
}
