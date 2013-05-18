#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "tools.h"
#include "Game.h"

// Affichage du budget
void displayBudget(SDL_Surface* figuresIMG, GLuint figures, int budget) {
	glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, figures);

		// Test : budget de 18 954
		int pos = 0;
		int i = 1;
		int figure = budget/i%10;
		while(i <= 100000) {
			// Détermination de la position du chiffre
			pos = posFigure(i);
			
			// Détermination du chiffre à afficher
			displayFigure(figuresIMG, figure, pos);

			i *= 10;
			figure = budget/i%10;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

// Détermination de la position du chiffre (unité, dizaine, centaine, etc.)
int posFigure(int i) {
	int pos = 0;
	switch(i) {
		case 1:
			pos = 190;
			break;
		case 10:
			pos = 170;
			break;
		case 100:
			pos = 150;
			break;
		case 1000:
			pos = 130;
			break;
		case 10000:
			pos = 110;
			break;
		case 100000:
			pos = 90;
			break;
		default:
			/*fprintf(stderr, "Wrong position\n");
			exit(1);*/
			break;
	}
	return pos;
}

// Détermination du chiffre à afficher
void displayFigure(SDL_Surface* figuresIMG, int figure, int pos) {
	switch(figure) {
		case 0:
			// 0
			glBegin(GL_QUADS);
				//glColor4ub(255, 255, 255, 255); // Opacité 100%
				glTexCoord2d(0, 0); glVertex2f(pos + 580, 304 + figuresIMG->h * 0.5);
				glTexCoord2d(0, 1); glVertex2f(pos + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.1, 1); glVertex2f(pos + 29 + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.1, 0); glVertex2f(pos + 29 + 580, 304 + figuresIMG->h * 0.5);
			glEnd();
			break;

		case 1:
			// 1
			glBegin(GL_QUADS);
				//glColor4ub(255, 255, 255, 255); // Opacité 100%
				glTexCoord2d(0.1, 0); glVertex2f(pos + 580, 304 + figuresIMG->h * 0.5);
				glTexCoord2d(0.1, 1); glVertex2f(pos + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.2, 1); glVertex2f(pos + 29 + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.2, 0); glVertex2f(pos + 29 + 580, 304 + figuresIMG->h * 0.5);
			glEnd();
			break;

		case 2:
			// 2
			glBegin(GL_QUADS);
				//glColor4ub(255, 255, 255, 255); // Opacité 100%
				glTexCoord2d(0.2, 0); glVertex2f(pos + 580, 304 + figuresIMG->h * 0.5);
				glTexCoord2d(0.2, 1); glVertex2f(pos + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.3, 1); glVertex2f(pos + 29 + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.3, 0); glVertex2f(pos + 29 + 580, 304 + figuresIMG->h * 0.5);
			glEnd();		
			break;

		case 3:
			// 3
			glBegin(GL_QUADS);
				//glColor4ub(255, 255, 255, 255); // Opacité 100%
				glTexCoord2d(0.3, 0); glVertex2f(pos + 580, 304 + figuresIMG->h * 0.5);
				glTexCoord2d(0.3, 1); glVertex2f(pos + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.4, 1); glVertex2f(pos + 29 + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.4, 0); glVertex2f(pos + 29 + 580, 304 + figuresIMG->h * 0.5);
			glEnd();			
			break;

		case 4:
			// 4
			glBegin(GL_QUADS);
				//glColor4ub(255, 255, 255, 255); // Opacité 100%
				glTexCoord2d(0.4, 0); glVertex2f(pos + 580, 304 + figuresIMG->h * 0.5);
				glTexCoord2d(0.4, 1); glVertex2f(pos + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.5, 1); glVertex2f(pos + 29 + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.5, 0); glVertex2f(pos + 29 + 580, 304 + figuresIMG->h * 0.5);
			glEnd();
			break;

		case 5:
			// 5
			glBegin(GL_QUADS);
				//glColor4ub(255, 255, 255, 255); // Opacité 100%
				glTexCoord2d(0.5, 0); glVertex2f(pos + 580, 304 + figuresIMG->h * 0.5);
				glTexCoord2d(0.5, 1); glVertex2f(pos + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.6, 1); glVertex2f(pos + 29 + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.6, 0); glVertex2f(pos + 29 + 580, 304 + figuresIMG->h * 0.5);
			glEnd();
			break;

		case 6:
			// 6
			glBegin(GL_QUADS);
				//glColor4ub(255, 255, 255, 255); // Opacité 100%
				glTexCoord2d(0.6, 0); glVertex2f(pos + 580, 304 + figuresIMG->h * 0.5);
				glTexCoord2d(0.6, 1); glVertex2f(pos + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.7, 1); glVertex2f(pos + 29 + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.7, 0); glVertex2f(pos + 29 + 580, 304 + figuresIMG->h * 0.5);
			glEnd();
			break;

		case 7:
			// 7
			glBegin(GL_QUADS);
				//glColor4ub(255, 255, 255, 255); // Opacité 100%
				glTexCoord2d(0.7, 0); glVertex2f(pos + 580, 304 + figuresIMG->h * 0.5);
				glTexCoord2d(0.7, 1); glVertex2f(pos + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.8, 1); glVertex2f(pos + 29 + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.8, 0); glVertex2f(pos + 29 + 580, 304 + figuresIMG->h * 0.5);
			glEnd();			
			break;

		case 8:
			// 8
			glBegin(GL_QUADS);
				//glColor4ub(255, 255, 255, 255); // Opacité 100%
				glTexCoord2d(0.8, 0); glVertex2f(pos + 580, 304 + figuresIMG->h * 0.5);
				glTexCoord2d(0.8, 1); glVertex2f(pos + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.9, 1); glVertex2f(pos + 29 + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(0.9, 0); glVertex2f(pos + 29 + 580, 304 + figuresIMG->h * 0.5);
			glEnd();
			break;

		case 9:
			// 9
			glBegin(GL_QUADS);
				//glColor4ub(255, 255, 255, 255); // Opacité 100%
				glTexCoord2d(0.9, 0); glVertex2f(pos + 580, 304 + figuresIMG->h * 0.5);
				glTexCoord2d(0.9, 1); glVertex2f(pos + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(1, 1); glVertex2f(pos + 29 + 580, 304 - figuresIMG->h * 0.5);
				glTexCoord2d(1, 0); glVertex2f(pos + 29 + 580, 304 + figuresIMG->h * 0.5);
			glEnd();
			break;

		default:
			break;
	}
}