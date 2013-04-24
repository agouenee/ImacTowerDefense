#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Map.h"

// Vérification de la validité de la carte itd et création de la map
int createMap(FILE* itd_file, Map* map) {
	// Première ligne : @ITD 1
	char code[5];
	int version;
	// Code @ITD
	fgets(code, 5, itd_file);
	if(strcmp(code, "@ITD") != 0) {
		fprintf(stderr, "Error itd file code [@ITD code]\n");
		return 0;
	}
	// Numéro de la version
	fscanf(itd_file, "%d\n", &version);
	if(version < 1) {
		fprintf(stderr, "Error itd file code [version]\n");
       	return 0;
    }
   	
    // Autres lignes du fichier
    char keyword[10];
    int val1 = 0;
    int val2 = 0;
    int val3 = 0;
    // Mot clé "carte"
    fgets(keyword, 7, itd_file);
    if(strcmp(keyword, "carte ") != 0) {
		fprintf(stderr, "Error itd file keyword [carte]\n");
		return 0;
	}
	// Valeur : nom du fichier PPM
    char filename[30];
    fgets(filename, 30, itd_file);
    //printf("%s", filename);               GERER L'ABSENCE DE POINT !
	char* extension = strchr(filename, '.');
	if(strcmp(extension, ".ppm\n") != 0) {
		fprintf(stderr, "Error itd file keyword value [carte]\n");
       	return 0;
	}
    else {
        (*map).image = filename;
    }
    // Mot clé "energie"
    fgets(keyword, 9, itd_file);
    if(strcmp(keyword, "energie ") != 0) {
        fprintf(stderr, "Error itd file keyword [energie]\n");
        return 0;
    }
    // Valeur energie
    fscanf(itd_file, "%d\n", &val1);
    if(val1 < 0) {
        fprintf(stderr, "Error itd file keyword value [energie]\n");
        return 0;
    }
    else {
        (*map).energie = val1;
    }
    // Mot clé "chemin"
    fgets(keyword, 8, itd_file);
    if(strcmp(keyword, "chemin ") != 0) {
        fprintf(stderr, "Error itd file keyword [chemin]\n");
        return 0;
    }
    // Couleur chemin
    fscanf(itd_file, "%d %d %d\n", &val1, &val2, &val3);
    if(val1 < 0 || val1 > 255 || val2 < 0 || val2 > 255 || val3 < 0 || val3 > 255) {
        fprintf(stderr, "Error itd file keyword value [chemin]\n");
        return 0;
    }
    else {
        (*map).pathColor = ColorRGB(val1, val2, val3);
    }
    // Mot clé "noeud"
    fgets(keyword, 7, itd_file);
    if(strcmp(keyword, "noeud ") != 0) {
        fprintf(stderr, "Error itd file keyword [noeud]\n");
        return 0;
    }
    // Couleur noeuds
    fscanf(itd_file, "%d %d %d\n", &val1, &val2, &val3);
    if(val1 < 0 || val1 > 255 || val2 < 0 || val2 > 255 || val3 < 0 || val3 > 255) {
        fprintf(stderr, "Error itd file keyword value [noeud]\n");
        return 0;
    }
    else {
        (*map).nodeColor = ColorRGB(val1, val2, val3);
    }
    // Mot clé "construct"
    fgets(keyword, 11, itd_file);
    if(strcmp(keyword, "construct ") != 0) {
        fprintf(stderr, "Error itd file keyword [construct]\n");
        return 0;
    }
    // Couleur zones constructibles
    fscanf(itd_file, "%d %d %d\n", &val1, &val2, &val3);
    if(val1 < 0 || val1 > 255 || val2 < 0 || val2 > 255 || val3 < 0 || val3 > 255) {
        fprintf(stderr, "Error itd file keyword value [construct]\n");
        return 0;
    }
    else {
        (*map).buildingAreaColor = ColorRGB(val1, val2, val3);
    }
    // Mot clé "in"
    fgets(keyword, 4, itd_file);
    if(strcmp(keyword, "in ") != 0) {
        fprintf(stderr, "Error itd file keyword [in]\n");
        return 0;
    }
    // Couleur zone d'entrée
    fscanf(itd_file, "%d %d %d\n", &val1, &val2, &val3);
    if(val1 < 0 || val1 > 255 || val2 < 0 || val2 > 255 || val3 < 0 || val3 > 255) {
        fprintf(stderr, "Error itd file keyword value [in]\n");
        return 0;
    }
    else {
        (*map).inColor = ColorRGB(val1, val2, val3);
    }
     // Mot clé "out"
    fgets(keyword, 5, itd_file);
    if(strcmp(keyword, "out ") != 0) {
        fprintf(stderr, "Error itd file keyword [out]\n");
        return 0;
    }
    // Couleur zone de sortie
    fscanf(itd_file, "%d %d %d\n", &val1, &val2, &val3);
    if(val1 < 0 || val1 > 255 || val2 < 0 || val2 > 255 || val3 < 0 || val3 > 255) {
        fprintf(stderr, "Error itd file keyword value [out]\n");
        return 0;
    }
    else {
        (*map).outColor = ColorRGB(val1, val2, val3);
    }
    // Nombre de noeuds
    fscanf(itd_file, "%d\n", &val1);
    // Nombre de lignes restantes
    unsigned int nbLignes = 1;
    int c;
    while((c = fgetc(itd_file)) != EOF) {
        if(c == '\n') {
            nbLignes++;
        }
    }
    if(val1 != nbLignes) {
        fprintf(stderr, "Error itd file number of nodes\n");
        return 0;
    }
    else {
        (*map).nbNodes = val1;
    }

    // Vérification de la correspondance des coordonnées de chaque noeud à des pixels de l'image
    // Suppression du caractère '\n' de trop à la fin du nom du fichier 
    int i = 0;
    while(i < strlen(filename)) {
        if(filename[i] == '\n') {
            filename[i] = '\0';
        }
        i++;
    }
    // Chargement de l'image pour avoir accès à ses dimensions
    char file[256] = "images/";
    strcat(file, filename);

    SDL_Surface* image = IMG_Load(file);
    if(image == NULL) {
        fprintf(stderr, "impossible de charger l'image %s\n", file);
        return 0;
    }
    
    //fscanf(itd_file, "%d %d\n", &val1, &val2);
    //printf("%d %d\n", val1, val2);

    //printf("height: %d\n width: %d\n", image->h, image->w);

    return 1;
}

// Chargement et ouverture de l'image (IM_1)
void loadMap(char* itd_fileName) {
    // Ouverture fichier (r: lecture seule)
    FILE* itd_file = fopen(itd_fileName, "r");
    if(itd_file == NULL) {
        // Erreur ouverture fichier
        fprintf(stderr, "Error opening itd file : %s\n", itd_fileName);
        exit(1);
    }
    else {
        Map map;
        // Vérification de la validité du fichier et création de la map
        if(createMap(itd_file, &map)) {
            // Carte valide
            printf("Map créée\n");
        }
    }
}
