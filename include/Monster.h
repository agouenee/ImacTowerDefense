#ifndef ITD_MONSTER___
#define ITD_MONSTER___

#define NB_MONSTER_LIST_MAX 20

typedef enum {
	BOUTIN, BARJOT
}MonsterType;

typedef struct monster {
	MonsterType type;
	int life;
	unsigned int resistance;
	int move;
	int speedDelay;
	int posX;
	int posY;
	struct monster* next;
	struct node* nextNode;
}Monster;

typedef struct monsterList {
	struct monster* root;
	int nbMonsters;
}MonsterList;

typedef struct monsterLists {
	MonsterList lists[NB_MONSTER_LIST_MAX];
	int nbLists;
}MonsterLists;


int countMonsters(Monster* root);
Monster* createMonster(MonsterType type, int posX, int posY, Node* nextNode);
Monster* addMonster(Monster* monsterList, Monster* addMonster);
Monster* rmvMonster(Monster* monsterList, Monster* monster);
int drawMonsters(Monster* root);
int drawMonster(Monster* monster, SDL_Surface* boutin, GLuint texture);

#endif