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
  

  // Initialisation SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_OPENGL);
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
