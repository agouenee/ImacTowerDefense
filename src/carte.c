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
    // Mot clé "carte"
    fgets(keyword, 6, itd_file);
    if(strcmp(keyword, "carte") != 0) {
		fprintf(stderr, "Error itd file keyword [carte]\n");
		return 1;
	}
	// Valeur : nom du fichier PPM 		GERER LES ESPACES !
	char filename[20];
	fscanf(itd_file, "%s\n", &filename);
	char *extension = strchr(filename, '.');
	if(strcmp(extension, ".ppm") != 0) {
		fprintf(stderr, "Error itd file keyword value [carte]\n");
       	return 1;
	}
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
