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
	int posX;
	int posY;
	struct monster* next;
}Monster;

void drawMonster(Node* node, int positionX, int positionY);
Monster* addMonster(Monster* monsterList, MonsterType type, int posX, int posY);

#endif