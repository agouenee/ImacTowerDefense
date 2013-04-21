#ifndef ITD_MONSTER___
#define ITD_MONSTER___

typedef enum {
	// A définir
	SLOW_MONSTER, FAST_MONSTER
}MonsterType;

typedef struct {
	MonsterType type;
	unsigned int life;
	unsigned int resistance;
	float speed;
}Monster;

#endif