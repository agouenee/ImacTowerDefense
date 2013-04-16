#ifndef ITD_MONSTRE___
#define LDR_MONSTRE___

typedef enum {
	// A définir
	MONSTRE1, MONSTRE2
}MonstreType;

typedef struct {
	MonstreType type;
	unsigned int vie;
	unsigned int resistance;
	unsigned int vitesse;
}Tour;

#endif