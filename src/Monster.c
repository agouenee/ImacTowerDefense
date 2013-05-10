#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Node.h"
#include "tools.h"
#include "Monster.h"

int countMonsters(Monster* root) {
	int cpt = 1;
	while((*root).next != NULL) {
		cpt++;
		root = (*root).next;
	}
	return cpt;
}
Monster* createMonster(MonsterType type, int posX, int posY, Node* nextNode) {
	if(nextNode == NULL) {
		fprintf(stderr, "pointer is NULL in createMonster function \n");
		exit(1);
	}

	Monster* newMonster = (Monster*)malloc(sizeof(Monster));
	if(newMonster == NULL) {
		fprintf(stderr, "Error allocation memory \n");
		exit(1);
	}

	if(type == BOUTIN) {
		(*newMonster).life = 10;
		(*newMonster).resistance = 10;
		(*newMonster).speed = 1;
	}
	else if(type == BARJOT) {
		(*newMonster).life = 20;
		(*newMonster).resistance = 20;
		(*newMonster).speed = 2;
	}
	else {
		fprintf(stderr, "Unknown monster type \n");
		exit(1);
	}
	(*newMonster).posX = posX;
	(*newMonster).posY = posY;
	(*newMonster).nextNode = nextNode;
	(*newMonster).next = NULL;

	return newMonster;
}

Monster* addMonster(Monster* monsterList, Monster* addMonster) {
	if(monsterList == NULL || addMonster == NULL) {
		fprintf(stderr, "pointer is NULL in addMonster function \n");
		exit(1);
	}	

	(*addMonster).next = monsterList;

	return addMonster;
}
Monster* rmvMonster(Monster* monsterList, Monster* monster) {
	if(monsterList == NULL) {
		fprintf(stderr, "pointer is NULL in rmvMonster function \n");
		exit(1);
	}
	Monster* root = monsterList;
	Monster* rmvMonster;

	if(monsterList == monster) {
		rmvMonster = monsterList;
		if((*monsterList).next != NULL) { 
			monsterList = (*monsterList).next;
			free(rmvMonster);
			return monsterList;
		}
		else {
			free(rmvMonster);
			return NULL;
		}
	}
	while((*monsterList).next != NULL) {
		if((*monsterList).next == monster) {
			rmvMonster = (*monsterList).next;
			if((*rmvMonster).next != NULL) {
				(*monsterList).next = (*rmvMonster).next;
			}
			else {
				(*monsterList).next = NULL;
			}
			free(rmvMonster);
		}
		monsterList = (*monsterList).next;
	}

	return root;
}
int drawMonsters(Monster* root) {
	SDL_Surface* boutin = IMG_Load("images/boutin.png");
	if(boutin == NULL) {
		fprintf(stderr, "impossible de charger l'image boutin.png \n");
		exit(1);
	}
	GLuint texture = loadTexture("images/boutin.png");

	while(root != NULL) {
		if(drawMonster(root, boutin, texture) == 0) {
			return 0;
		}
		root = (*root).next;
	}

	SDL_FreeSurface(boutin);

	return 1;
}
int drawMonster(Monster* monster, SDL_Surface* boutin, GLuint texture) {
	if(monster->nextNode != NULL) {
		if(monster->nextNode->y == monster->posY) {
			if(monster->nextNode->x > monster->posX) {
				monster->posX += monster->speed;
			}
			else {
				monster->posX -= monster->speed;
			}   
		}
		else {
			if(monster->nextNode->y > monster->posY) {
				monster->posY += monster->speed;
			}
			else {
				monster->posY -= monster->speed;
			}
		}
  		if(monster->posX == monster->nextNode->x && monster->posY == monster->nextNode->y) {
			monster->nextNode = monster->nextNode->next;
  		}

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, texture);

		glBegin(GL_QUADS);
		glTexCoord2d(0, 1); glVertex2d(monster->posX - boutin->w * 0.5, 600 - monster->posY - boutin->h * 0.5);
		glTexCoord2d(0, 0); glVertex2d(monster->posX - boutin->w * 0.5, 600 - monster->posY + boutin->h * 0.5);
		glTexCoord2d(1, 0); glVertex2d(monster->posX + boutin->w * 0.5, 600 - monster->posY + boutin->h * 0.5);
		glTexCoord2d(1, 1); glVertex2d(monster->posX + boutin->w * 0.5, 600 - monster->posY - boutin->h * 0.5);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		return 1;
	}
	return 0;
}