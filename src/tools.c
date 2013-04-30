#include "tools.h"

GLuint loadTexture(char* filename) {
	GLuint textureId;
	GLenum format;

	SDL_Surface* image = IMG_Load(filename);
	if(image == NULL) {
      fprintf(stderr, "impossible de charger l'image %s\n", filename);
      exit(1);
   }

   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_2D, textureId);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   	switch(image->format->BytesPerPixel) {
	    case 1:
	    format = GL_RED;
	    break;
	    case 3:
	    format = GL_RGB;
	    break;
	    case 4:
	    format = GL_RGBA;
	    break;
	    default:
	      /* On ne traite pas les autres cas */
	    fprintf(stderr, "Format des pixels de l’image %s non pris en charge\n", filename);
	    exit(1);
 	}

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format,
    GL_UNSIGNED_BYTE, image->pixels);

   glBindTexture(GL_TEXTURE_2D, 0);
   SDL_FreeSurface(image);

   return textureId;
}