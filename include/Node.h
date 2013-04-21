#ifndef ITD_NODE___
#define ITD_NODE___

typedef struct node {
	float x;
	float y;
	struct node* next;
}Node;
typedef Node* Path;

#endif