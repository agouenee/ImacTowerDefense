#ifndef ITD_MAP___
#define ITD_MAP___

#include "Color.h"
#include "Node.h"

typedef struct map {
	char* image;
	unsigned int energie; 	// Nb unité d'énergie produit par les centrales (wtf?!) 
	Color pathColor;
	Color nodeColor;
	Color buildingAreaColor;
	Color inColor;
	Color outColor;
	unsigned int nbNodes;
	Node* listNodes;
}Map;

int verifMap(FILE* itd_file);
void loadMap(char* itd_fileName);

#endif