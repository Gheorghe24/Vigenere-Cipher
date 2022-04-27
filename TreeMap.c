#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreeMap.h"

#define MAX(a, b) (((a) >= (b)) ? (a) : (b))

/* Creeaza un arbore cu o serie de metode asociate
 *
 * return: arborele creat
 */
TTree *createTree(void *(*createElement)(void *),
				  void (*destroyElement)(void *),
				  void *(*createInfo)(void *),
				  void (*destroyInfo)(void *),
				  int compare(void *, void *))
{
	TTree *newTree = (TTree *)malloc(sizeof(TTree));
	newTree->createElement = createElement;
	newTree->createInfo = createInfo;
	newTree->destroyElement = destroyElement;
	newTree->destroyInfo = destroyInfo;
	newTree->compare = compare;
	newTree->size = 0;
	newTree->root = NULL;
	return newTree;
}

/* Verifica daca un arbore este gol (vid)
 * 		1 - daca arborele este gol
 * 		0 - in caz contrar
 */
int isEmpty(TTree *tree)
{
	return (tree->root == NULL);
}

/* Cauta un anumit element in interiorul unui arbore
 *
 * tree: structura cu metodele asociate arborelui
 *   !folosit pentru a putea compara elementul curent cu cel cautat
 *
 * x: radacina arborelui curent (in care se face cautarea)
 * elem: elementul ce trebuie cautat
 */
TreeNode *search(TTree *tree, TreeNode *x, void *elem)
{
	if (x == NULL || tree->compare(x->elem, elem) == 0)
	{
		return x;
	}
	if (tree->compare(x->elem, elem) == -1)
	{
		return search(tree, x->right, elem);
	}
	return search(tree, x->left, elem);
}

/* Gaseste nodul cu elementul minim dintr-un arbore
 * avand radacina in x
 */
TreeNode *minimum(TreeNode *x)
{
	while (x->left != NULL)
		x = x->left;
	return x;
}

/* Gaseste nodul cu elementul maxim dintr-un arbore
 * avand radacina in x
 */
TreeNode *maximum(TreeNode *x)
{
	while (x->right != NULL)
		x = x->right;
	return x;
}

/* Functie pentru gasirea succesorului unui nod
 * (succesorul in inordine)
 */
TreeNode *successor(TreeNode *x)
{	
	if (x->right != NULL)
		return minimum(x->right);
	TreeNode *par = x->parent;
	// step 2 of the above algorithm
	while (par!=NULL && x == par->right) {
		x = par;
		par = par->parent;
	}
	return par;
}

/* Functie pentru gasirea predecesorului unui nod
 * (predecesorul in inordine)
 */
TreeNode *predecessor(TreeNode *x)
{
	if (x->left != NULL)
		return maximum(x->left);
	TreeNode *par = x->parent;
	// step 2 of the above algorithm
	while (par!=NULL && x == par->left) {
		x = par;
		par = par->parent;
	}
	return par;
}

/* Actualizeaza inaltimea unui nod din arbore
 */
void updateHeight(TreeNode *x)
{

	int leftHeight = 0;
	int rightHeight = 0;

	if (x != NULL)
	{
		if (x->left != NULL)
			leftHeight = x->left->height;
		if (x->right != NULL)
			rightHeight = x->right->height;
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}

void update_All(TreeNode *x) {
	if (!x)
		return;
	update_All(x->left);
	update_All(x->right);
	updateHeight(x);
}

/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la stanga
 * a subarborelui care are varful in x
 *
 *   (x)	 Rotatie    (y)
 *	 / \     stanga     / \
 *  a  (y)    ---->   (x)  c
 *	   / \			  / \
 *    b   c          a   b
 */
void avlRotateLeft(TTree *tree, TreeNode *x)
{
	if (tree == NULL || x == NULL)
		return;
	TreeNode* y1 = x->right;
	x->right = y1->left;
	y1->left = x;
	tree->root = y1;
	updateHeight(x);
	updateHeight(y1);
	
	//update_All(y1);
	// TreeNode *y = x->right;
	// TreeNode *second = y->left;
	// if(second) {
	// 	second->parent = x;
	// 	x->right = second;
	// } else {
	// 	x->right = NULL;
	// }
	
	// y->left = x;
	// if(x->parent == NULL){
	// 	tree->root = y;
	// } else {
	// 	if(x->parent->left == x) { 
	// 		x->parent->left = y;
	// 	} else if(x->parent->right == x) { 
	// 		x->parent->right = y;
	// 	}

	// 	y->parent = x->parent;
	// }
	// x->parent = y;
	// // updateHeight(x);
	// // updateHeight(y);
	// update_All(tree->root);
	
}

/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la dreapta
 * a subarborelui care are varful in y
 *
 *     (y)	 Rotatie    (x)
 *	   / \   dreapta    / \
 *   (x)  c   ---->    a  (y)
 *	 / \			      / \
 *  a   b                b   c
 */
void avlRotateRight(TTree *tree, TreeNode *y)
{
	if (tree == NULL || y == NULL)
		return;
	TreeNode* y1 = y->left;
	y->left = y1->right;
	y1->right = y;
	tree->root = y1;
	updateHeight(y);
	updateHeight(y1);
	
	// TreeNode *x = y->left;
	// TreeNode *second = x->right;

	// if(second) {
	// 	second->parent = y;
	// 	y->left = second;
	// } else {
	// 	y->left = NULL;
	// }
		
	// x->right = y;
	// if(y->parent == NULL){
	// 	tree->root = x;
	// } else {
	// 	if(y->parent->left == y) { 
	// 		y->parent->left = x;
	// 	} else if(y->parent->right == y) { 
	// 		y->parent->right = x;
	// 	}

	// 	x->parent = y->parent;
	// }
	// y->parent = x;

	// update_All(tree->root);

}

/* Calculeaza factorul de echilibrare pentru un nod x
 * (AVL balance factor)
 */
int avlGetBalance(TreeNode *x)
{
	if (x == NULL)
		return 0;
	return x->left->height - x->right->height;
}

/* Functie pentru reechilibrarea unui arbore AVL
 * in urma unei inserari prin rotatii simple sau duble
 * si recalcularea inaltimii fiecarui nod intalnit parcurgand
 * arborele de jos in sus, spre radacina
 *
 */
void avlFixUp(TTree *tree, TreeNode *y)
{
}

/* Functie pentru crearea unui nod
 *
 * value: valoarea/cheia din cadrul arborelui
 * info: informatia/valoarea din dictionar
 */
TreeNode *createTreeNode(TTree *tree, void *value, void *info)
{

	if (tree == NULL)
		return NULL;

	// Alocarea memoriei
	TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

	// Setarea elementului si a informatiei
	node->elem = tree->createElement(value);
	node->info = tree->createInfo(info);

	// Initializarea legaturilor din cadrul arborelui
	node->parent = node->right = node->left = NULL;

	// Initializarea legaturilor din cadrul listei dublu inlantuite
	node->next = node->prev = node->end = NULL;

	// Inaltimea unui nod nou este 1
	// Inaltimea lui NULL este 0
	node->height = 1;

	return node;
}

/* Inserarea unul nou nod in cadrul multi-dictionarului
 * ! In urma adaugarii arborele trebuie sa fie echilibrat
 *
 */
void insert1(TTree *tree, void *elem, void *info)
{
	// if(tree->root == NULL){
	// //	return createTree(elem,info);
	// 	tree->root = createTreeNode(tree, elem, info);
	// 	tree->root->end = tree->root;
	// 	return;
	// }
	// if(tree->compare(info,tree->root->info) == -1){
	// 	insert(tree, tree->root->left, info);
	// } else if(tree->compare(info,tree->root->info) == -1) {
	// 	insert(tree, tree->root->right, info);
	// } 

	// updateHeight(tree->root);

	// int balance = avlGetBalance(tree->root);

	// if(balance > 1 && tree->compare(info,tree->root->info) == -1) {
	// 	avlRotateRight(tree, tree->root);
	// }
	// if(balance < -1 && tree->compare(info,tree->root->info) == 1) {
	// 	avlRotateLeft(tree, tree->root);
	// }
	// if(balance > 1 && tree->compare(info,tree->root->info) == 1) {
	// 	avlRotateLeft(tree, tree->root->left);
	// 	avlRotateRight(tree, tree->root);
	// }
	// if(balance < -1 && tree->compare(info,tree->root->info) == -1) {
	// 	avlRotateRight(tree, tree->root->right);
	// 	avlRotateLeft(tree, tree->root);
	// }

}

void insert(TTree *tree, void *elem, void *info)
{
	TreeNode *newnode = createTreeNode(tree, elem, info), *prev = tree->root, *curr = NULL;
	while (prev)
	{
		curr = prev;
		if(tree->compare(elem,prev->elem) == -1) {
			prev = prev->left;
		}
		else {
			prev = prev->right;
		}
	}
	//root e null
	if(curr == NULL) {
		tree->root = newnode;
		tree->root->end = tree->root;
	} else if(tree->compare(elem,curr->elem) == -1){
		curr->left = curr;
	} else if(tree->compare(elem,curr->elem) == 1){
		curr->right = curr;
	} else {
		newnode->prev = curr->end;
		curr->end = newnode;
	}
	updateHeight(tree->root);

	int balance = avlGetBalance(tree->root);

	if(balance > 1 && tree->compare(elem,tree->root->elem) == -1) {
		avlRotateRight(tree, tree->root);
	}
	if(balance < -1 && tree->compare(elem,tree->root->elem) == 1) {
		avlRotateLeft(tree, tree->root);
	}
	if(balance > 1 && tree->compare(elem,tree->root->elem) == 1) {
		avlRotateLeft(tree, tree->root->left);
		avlRotateRight(tree, tree->root);
	}
	if(balance < -1 && tree->compare(elem,tree->root->elem) == -1) {
		avlRotateRight(tree, tree->root->right);
		avlRotateLeft(tree, tree->root);
	}

}
/* Eliminarea unui nod dintr-un arbore
 *
 * ! tree trebuie folosit pentru eliberarea
 *   campurilor `elem` si `info`
 * */
void destroyTreeNode(TTree *tree, TreeNode *node)
{

	// Verificarea argumentelor functiei
	if (tree == NULL || node == NULL)
		return;

	// Folosirea metodelor arborelui
	// pentru de-alocarea campurilor nodului
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// Eliberarea memoriei nodului
	free(node);
}

/* Eliminarea unui nod din arbore
 *
 * elem: cheia nodului ce trebuie sters
 * 	! In cazul in care exista chei duplicate
 *	  se va sterge ultimul nod din lista de duplicate
 */
void delete (TTree *tree, void *elem)
{
}

/* Eliberarea memoriei unui arbore
 */
void destroyTree(TTree *tree)
{

	/* Se poate folosi lista dublu intalntuita
	 * pentru eliberarea memoriei
	 */

	if (tree == NULL || tree->root == NULL)
		return;
	free(tree);
}