#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Vérification de la validité de la carte itd
int verifMap(FILE* itd_file) {
	// Première ligne : @ITD 1
	char code[5];
	int version;
	// Code @ITD
	fgets(code, 5, itd_file);
	if(strcmp(code, "@ITD") != 0) {
		fprintf(stderr, "Error itd file code [@ITD code]\n");
		return 1;
	}
	// Numéro de la version
	fscanf(itd_file, "%d\n", &version);
	if(version < 1) {
		fprintf(stderr, "Error itd file code [version]\n");
       	return 1;
    }
   	
    // Autres lignes du fichier
    char keyword[10];
    int val1 = 0;
    int val2 = 0;
    int val3 = 0;
    // Mot clé "carte"
    fgets(keyword, 6, itd_file);
    if(strcmp(keyword, "carte") != 0) {
		fprintf(stderr, "Error itd file keyword [carte]\n");
		return 1;
	}
	// Valeur : nom du fichier PPM 		GERER LES ESPACES !
	char* filename = (char*) malloc(sizeof(char)*20);
    if(filename == NULL) {
        fprintf(stderr, "Error allocation memory\n");
        exit(1);
    }
	fscanf(itd_file, "%s\n", filename);
	char *extension = strchr(filename, '.');
	if(strcmp(extension, ".ppm") != 0) {
		fprintf(stderr, "Error itd file keyword value [carte]\n");
       	return 1;
	}
    // Mot clé "energie"
    fgets(keyword, 8, itd_file);
    if(strcmp(keyword, "energie") != 0) {
        fprintf(stderr, "Error itd file keyword [energie]\n");
        return 1;
    }
    // Valeur energie
    fscanf(itd_file, "%d\n", &val1);
    if(val1 < 0) {
        fprintf(stderr, "Error itd file keyword value [energie]\n");
        return 1;
    }
    // Mot clé "chemin"
    fgets(keyword, 7, itd_file);
    if(strcmp(keyword, "chemin") != 0) {
        fprintf(stderr, "Error itd file keyword [chemin]\n");
        return 1;
    }
    // Couleur chemin
    fscanf(itd_file, "%d %d %d\n", &val1, &val2, &val3);
    if(val1 < 0 || val1 > 255 || val2 < 0 || val2 > 255 || val3 < 0 || val3 > 255) {
        fprintf(stderr, "Error itd file keyword value [chemin]\n");
        return 1;
    }
    // Mot clé "noeud"
    fgets(keyword, 6, itd_file);
    if(strcmp(keyword, "noeud") != 0) {
        fprintf(stderr, "Error itd file keyword [noeud]\n");
        return 1;
    }
    // Couleur noeuds
    fscanf(itd_file, "%d %d %d\n", &val1, &val2, &val3);
    if(val1 < 0 || val1 > 255 || val2 < 0 || val2 > 255 || val3 < 0 || val3 > 255) {
        fprintf(stderr, "Error itd file keyword value [noeud]\n");
        return 1;
    }
    // Mot clé "construct"
    fgets(keyword, 10, itd_file);
    if(strcmp(keyword, "construct") != 0) {
        fprintf(stderr, "Error itd file keyword [construct]\n");
        return 1;
    }
    // Couleur zones constructibles
    fscanf(itd_file, "%d %d %d\n", &val1, &val2, &val3);
    if(val1 < 0 || val1 > 255 || val2 < 0 || val2 > 255 || val3 < 0 || val3 > 255) {
        fprintf(stderr, "Error itd file keyword value [construct]\n");
        return 1;
    }
    // Mot clé "in"
    fgets(keyword, 3, itd_file);
    if(strcmp(keyword, "in") != 0) {
        fprintf(stderr, "Error itd file keyword [in]\n");
        return 1;
    }
    // Couleur zone d'entrée
    fscanf(itd_file, "%d %d %d\n", &val1, &val2, &val3);
    if(val1 < 0 || val1 > 255 || val2 < 0 || val2 > 255 || val3 < 0 || val3 > 255) {
        fprintf(stderr, "Error itd file keyword value [in]\n");
        return 1;
    }
     // Mot clé "out"
    fgets(keyword, 4, itd_file);
    if(strcmp(keyword, "out") != 0) {
        fprintf(stderr, "Error itd file keyword [out]\n");
        return 1;
    }
    // Couleur zone de sortie
    fscanf(itd_file, "%d %d %d\n", &val1, &val2, &val3);
    if(val1 < 0 || val1 > 255 || val2 < 0 || val2 > 255 || val3 < 0 || val3 > 255) {
        fprintf(stderr, "Error itd file keyword value [out]\n");
        return 1;
    }
    // Nombre de noeuds
    /* pour compter le nb de lignes :
    while(fgets(ligne, 200, fichier) != NULL){
        count++;
    } */
    //printf("%s\n", keyword);
    //printf("%d %d %d\n", val1, val2, val3);
    return 0;
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
        // Vérification de la validité du fichier
        int test = verifMap(itd_file);
        if(test == 0) {
            /* Carte valide */
        }
    }
}
