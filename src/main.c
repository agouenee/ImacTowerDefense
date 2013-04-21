#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "Map.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void reshape() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-100., 100., -100., 100.);
}

int main(int argc, char** argv) {
   // Initialisation des variables
   char* filename = "images/map-test.ppm";
   /* SDL_Rect positonMap;
   positionMap.x = 0;
   positionMap.y = 0; */


   // Initialisation SDL
   SDL_Init(SDL_INIT_VIDEO);
   SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_OPENGL);
   SDL_WM_SetCaption("ImacTowerDefense", NULL);

   SDL_Surface* image = IMG_Load(filename);
   if(image == NULL) {
      fprintf(stderr, "impossible de charger l'image %s\n", filename);
      return EXIT_FAILURE;
   }
   SDL_FreeSurface(image);
   SDL_Quit();

   // switch(image->format->BytesPerPixel) {
   //    case 1:
   //       format = GL_RED;
   //       break;
   //    case 3:
   //       format = GL_RGB;
   //       break;
   //    case 4:
   //       format = GL_RGBA;
   //       break;
   //    default:
   //       /* On ne traite pas les autres cas */
   //       fprintf(stderr, "Format des pixels de l’image %s non pris en charge\n", filename);
   //       return EXIT_FAILURE;
   // }

   // Chargement carte
   loadMap("data/map-test.itd");


   reshape();


   // Boucle événements
   int loop = 1;
   while(loop) {
      Uint32 startTime = SDL_GetTicks();

      // Dessin
      //SDL_GL_SwapBuffers();
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
