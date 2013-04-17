#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define WIDTH 800
#define HEIGHT 600

void reshape() {
  glViewport(0, 0, WIDTH, HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-100., 100., -100., 100.);
}

int main(int argc, char** argv) {
   // Initialisation des variables
   char* filename = "../images/map-test.ppm";
   /* SDL_Rect positonMap;
   positionMap.x = 0;
   positionMap.y = 0; */


   // Initialisation SDL
   SDL_Init(SDL_INIT_VIDEO);
   SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_OPENGL);
   SDL_WM_SetCaption("ImacTowerDefense", NULL);

   SDL_Surface* image = IMG_Load(filename);
   if(image == NULL) {
      fprintf(stderr, "impossible de charger l'image %s\n", filename);
      return EXIT_FAILURE;
   }

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


   reshape();


   // Boucle événements
   int loop = 1;
   while(loop) {
      //Uint32 startTime = SDL_GetTicks();

      // Dessin


      SDL_GL_SwapBuffers();
      /* ****** */

      SDL_Event e;
      while(SDL_PollEvent(&e)) {
         if(e.type == SDL_QUIT) {
            loop = 0;
            break;
         }

         switch(e.type) {
            /* Clic souris */
            case SDL_MOUSEBUTTONDOWN:
               switch(e.button.button) {
                  /* Clic gauche */
                  case SDL_BUTTON_LEFT:
                     break;

                  /* Clic droit */
                  case SDL_BUTTON_RIGHT:
                     break;
<<<<<<< HEAD

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
   }

   SDL_Quit();
   return EXIT_SUCCESS;
}
=======

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
   }

   SDL_Quit();
   return EXIT_SUCCESS;
}
>>>>>>> 80c31c815daf2913f7adc5c04ba335234bc2e24b
