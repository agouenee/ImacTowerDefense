#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "Map.h"
#include "tools.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 10;

GLuint texture;

void reshape() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0., 600., 0., 600.);
}

int main(int argc, char** argv) {
   // Initialisation des variables
   int positionX, positionY;

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

   SDL_Surface* boutin = IMG_Load("images/boutin.png");
   if(boutin == NULL) {
      fprintf(stderr, "impossible de charger l'image boutin.png \n");
      return EXIT_FAILURE;
   }
   texture = loadTexture("images/boutin.png");

   // Chargement de la map
      // Chargement de l'image ppm
   char* filename = "images/map-test.ppm";
   SDL_Surface* image = IMG_Load(filename);
   if(image == NULL) {
      fprintf(stderr, "impossible de charger l'image %s\n", filename);
      return EXIT_FAILURE;
   }
      // Chargement du fichier itd
   Map map = loadMap("data/map-test.itd");
   Node* root = map.listNodes;
   Node* node = root;
   Node* first = root;
   // Initialisation de la position des monstres
   positionX = node->x;
   positionY = node->y;
   reshape();

   // Boucle événements
   glClear(GL_COLOR_BUFFER_BIT);
   int loop = 1;
   while(loop) {
      root = first;
      Uint32 startTime = SDL_GetTicks();
      glClear(GL_COLOR_BUFFER_BIT);

      // Dessin du chemin
      glBegin(GL_LINES);
      glColor3ub(255, 255, 255);

      while(root->next != NULL) {
         glVertex2d(root->x, 600-root->y);
         glVertex2d(root->next->x, 600-root->next->y);

         root = root->next;
      }

      glEnd();

      if(node->next->y == positionY) {
         if(node->next->x > positionX) {
            positionX += 1;
         }
         else {
            positionX -= 1;
         }   
      }
      else {
         if(node->next->y > positionY) {
            positionY += 1;
         }
         else {
            positionY -= 1;
         }
      }

      if(positionX == node->next->x && positionY == node->next->y) {
         node = node->next;
      }

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

   SDL_Quit();
   return EXIT_SUCCESS;
}
