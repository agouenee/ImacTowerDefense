#ifndef ITD_MONSTER___
#define ITD_MONSTER___

typedef enum {
	// A définir
	BOUTIN, BARJOT
}MonsterType;

typedef struct monster {
	MonsterType type;
	unsigned int life;
	unsigned int resistance;
	float speed;
}Monster;

#endif