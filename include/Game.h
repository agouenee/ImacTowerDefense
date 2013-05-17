#ifndef ITD_GAME___
#define ITD_GAME___

typedef struct game {
	int start;
	int win;
	int over;
	int budget; 
}Game;


void displayBudget(SDL_Surface* figuresIMG, GLuint figures, int budget);
int posFigure(int i);
void displayFigure(SDL_Surface* figuresIMG, int figure, int pos);

#endif