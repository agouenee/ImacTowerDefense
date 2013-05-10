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
	struct node* nextNode;
}Monster;

typedef struct monsterList {
	struct monster* root;
	int nbMonster;
}MonsterList;


int countMonsters(Monster* root);
Monster* createMonster(MonsterType type, int posX, int posY, Node* nextNode);
Monster* addMonster(Monster* monsterList, Monster* addMonster);
Monster* rmvMonster(Monster* monsterList, Monster* monster);
int drawMonsters(Monster* root);
int drawMonster(Monster* monster, SDL_Surface* boutin, GLuint texture);

#endif