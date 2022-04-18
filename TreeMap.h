#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <stdlib.h>

/*
 * Reprezentarea unui nod din arbore
 */
typedef struct node{
	void* elem; 			// elementul/cheia unui nod
	void* info;				// informatia/valoarea unui nod
	struct node *parent;	// parintele unui nod
	struct node *left;  	// copilul stang
	struct node *right; 	// copilul drept
	struct node* next; 		// pointer catre urmatorul nod din lista de duplicate
	struct node* prev; 		// pointer catre nodul anterior din lista de duplicate
	struct node* end; 		// pointer catre finalul listei de duplicate pentru
							// nodul curent
	long height;			// inaltimea nodului in arbore
}TreeNode;

/*
 * Reprezentarea multi-dictionarului
 */
typedef struct TTree{
	TreeNode *root;					// radacina arborelui
	void* (*createElement)(void*);  // metoda pentru crearea unui element
	void (*destroyElement)(void*);	// metoda pentru stergerea unui element
	void* (*createInfo)(void*); 	// metoda pentru crearea informatiei (valorii)
	void (*destroyInfo)(void*); 	// metoda pentru stergerea informatiei
	int (*compare)(void*, void*); 	// metoda pentru compararea a doua noduri
	long size;						// numarul de noduri din arbore
}TTree;


TTree* createTree(void* (*createElement)(void*),
				  void (*destroyElement)(void*),
				  void* (*createInfo)(void*),
				  void (*destroyInfo)(void*),
				  int compare(void*, void*));

int isEmpty(TTree* tree);
TreeNode* search(TTree* tree, TreeNode* x, void* elem);
TreeNode* minimum(TreeNode* x);
TreeNode* maximum(TreeNode* x);
TreeNode* successor(TreeNode* x);
TreeNode* predecessor(TreeNode* x);
void updateHeight(TreeNode* x);
void avlRotateLeft(TTree* tree, TreeNode* x);
void avlRotateRight(TTree* tree, TreeNode* y);
int avlGetBalance(TreeNode *x);
void avlFixUp(TTree* tree, TreeNode* y);
TreeNode* createTreeNode(TTree *tree, void* value, void* info);
void destroyTreeNode(TTree *tree, TreeNode* node);
void insert(TTree* tree, void* elem, void* info);
void delete(TTree* tree, void* elem);
void destroyTree(TTree* tree);

#endif /* TREEMAP_H_ */