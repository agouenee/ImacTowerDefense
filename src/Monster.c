#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Node.h"
#include "tools.h"
#include "Monster.h"

Monster* addMonster(Monster* monsterList, MonsterType type, int posX, int posY) {
	if(monsterList == NULL) {
		fprintf(stderr, "pointer is NULL in addMonster function");
        exit(1);
	}	

	Monster* newMonster = (Monster*) malloc(sizeof(Monster));
	if(newMonster == NULL) {
		fprintf(stderr, "Error allocation memory");
        exit(1);
	}

	(*newMonster).posX = posX;
	(*newMonster).posY = posY;
	(*newMonster).type = type;

	// VALEURS A MODIFIER
	if(type == BOUTIN) {
		(*newMonster).life = 10;
		(*newMonster).resistance = 10;
		(*newMonster).speed = 10;
	}
	else if(type == BARJOT) {
		(*newMonster).life = 20;
		(*newMonster).resistance = 20;
		(*newMonster).speed = 20;
	}
	else {
		fprintf(stderr, "Unknown monster type");
		free(newMonster);
        exit(1);
	}

	(*newMonster).next = monsterList;

	return newMonster;
}
void drawMonster(Node* node, int positionX, int positionY) {
	SDL_Surface* boutin = IMG_Load("images/boutin.png");
   	if(boutin == NULL) {
      	fprintf(stderr, "impossible de charger l'image boutin.png \n");
      	exit(1);
   	}
   	GLuint texture = loadTexture("images/boutin.png");

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glTexCoord2d(0, 1); glVertex2d(positionX - boutin->w * 0.5, 600 - positionY - boutin->h * 0.5);
    glTexCoord2d(0, 0); glVertex2d(positionX - boutin->w * 0.5, 600 - positionY + boutin->h * 0.5);
    glTexCoord2d(1, 0); glVertex2d(positionX + boutin->w * 0.5, 600 - positionY + boutin->h * 0.5);
    glTexCoord2d(1, 1); glVertex2d(positionX + boutin->w * 0.5, 600 - positionY - boutin->h * 0.5);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}