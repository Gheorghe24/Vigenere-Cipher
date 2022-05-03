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
	if (x == NULL)
		return NULL;
	if (tree->compare(elem, x->elem) == 0)
	{
		// printf("%d %d\n", *(int*)x->elem, *(int*)elem);
		return x;
	}
	else if (tree->compare(elem, x->elem) == -1)
	{
		return search(tree, x->left, elem);
	}
		return search(tree, x->right, elem);
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
	while (par != NULL && x == par->right)
	{
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
	while (par != NULL && x == par->left)
	{
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

	TreeNode *y = x->right;

	x->right = y->left;
	if (y->left != NULL)
	{
		y->left->parent = x;
	}

	if (x->parent == NULL)
	{
		tree->root = y;
	}
	else if (x->parent->left == x)
	{
		x->parent->left = y;
	}
	else
	{
		x->parent->right = y;
	}
	y->parent = x->parent;
	y->left = x;
	x->parent = y;
	updateHeight(x);
	updateHeight(y);
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

	TreeNode *x = y->left;

	if (x->right)
	{
		x->right->parent = y;
	}
	y->left = x->right;
	if (y->parent == NULL)
	{
		tree->root = x;
	}
	else if (y->parent->left == y)
	{
		y->parent->left = x;
	}
	else if (y->parent->right == y)
	{
		y->parent->right = x;
	}
	x->parent = y->parent;
	x->right = y;
	y->parent = x;
	updateHeight(y);
	updateHeight(x);
}

/* Calculeaza factorul de echilibrare pentru un nod x
 * (AVL balance factor)
 */
int avlGetBalance(TreeNode *x)
{
	if (x == NULL)
		return 0;
	if (x->left == NULL && x->right == NULL)
	{
		return 0;
	}
	if (x->left == NULL)
	{
		return (0 - x->right->height);
	}
	if (x->right == NULL)
	{
		return x->left->height;
	}
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
	void *elem = y->elem;
	TreeNode *par = y;
	while (par != NULL)
	{
		updateHeight(par);
		int balance = avlGetBalance(par);

		if (balance > 1 && tree->compare(elem, par->left->elem) == -1)
		{
			// printf("CAZ 1\n");
			avlRotateRight(tree, par);
			 break;
		}
		if (balance < -1 && tree->compare(elem, par->right->elem) == 1)
		{
			// printf("CAZ 2\n");
			avlRotateLeft(tree, par);
			 break;
		}
		if (balance > 1 && tree->compare(elem, par->left->elem) == 1)
		{
			// printf("CAZ 3\n");
			avlRotateLeft(tree, par->left);
			avlRotateRight(tree, par);
			 break;
		}
		if (balance < -1 && tree->compare(elem, par->right->elem) == -1)
		{
			// printf("CAZ 4\n");
			avlRotateRight(tree, par->right);
			avlRotateLeft(tree, par);
			 break;
		}
		// printf("\n");
		par = par->parent;
	}
}

void avlDeleteFixUp(TTree *tree, TreeNode *y)
{
	// void *elem = y->elem;
	TreeNode *par = y;
	while (par != NULL)
	{
		updateHeight(par);
		int balance = avlGetBalance(par);
		// printf("BAL %d Hei %d PAR %d ROOT %d \n", balance, par->height,*(long int*)par->elem, *(long int*)(tree)->root->elem);

		if (balance > 1 && avlGetBalance(par->left) >= 0)
		{
			// printf("CAZ 1\n");
			avlRotateRight(tree, par);
			break;
		}
		if (balance < -1 && avlGetBalance(par->right) <= 0)
		{
			// printf("CAZ 2\n");
			avlRotateLeft(tree, par);
			// break;
		}
		if (balance > 1 && avlGetBalance(par->left) < 0)
		{
			// printf("CAZ 3\n");
			avlRotateLeft(tree, par->left);
			avlRotateRight(tree, par);
			// break;
		}
		if (balance < -1 && avlGetBalance(par->right) > 0)
		{
			// printf("CAZ 4\n");
			avlRotateRight(tree, par->right);
			avlRotateLeft(tree, par);
			// break;
		}
		// printf("\n");
		par = par->parent;
	}
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

void insert(TTree *tree, void *elem, void *info)
{
	TreeNode *newnode = createTreeNode(tree, elem, info), *prev = tree->root, *curr = NULL;
	while (prev)
	{
		curr = prev;
		if (tree->compare(elem, prev->elem) == -1)
		{
			prev = prev->left;
		}
		else if (tree->compare(elem, prev->elem) == 1)
		{
			prev = prev->right;
		}
		else
		{
			break;
		}
	}
	// root e null
	if (curr == NULL)
	{
		tree->root = newnode;
		tree->root->end = tree->root;
	}
	else
	{
		if (tree->compare(elem, curr->elem) == -1)
		{
			curr->left = newnode;
			newnode->parent = curr;
			newnode->end = newnode;
		}
		else if (tree->compare(elem, curr->elem) == 1)
		{
			curr->right = newnode;
			newnode->parent = curr;
			newnode->end = newnode;
		}
		else
		{
			newnode->prev = curr->end;
			if (curr->end->next)
				curr->end->next->prev = newnode;
			newnode->next = curr->end->next;
			curr->end->next = newnode;
			curr->end = newnode;
		}

		if (maximum(tree->root) != newnode && tree->compare(elem, curr->elem) != 0)
		{
			successor(newnode)->prev = newnode;
			newnode->next = successor(newnode);
		}
		if (minimum(tree->root) != newnode && tree->compare(elem, curr->elem) != 0)
		{
			predecessor(newnode)->end->next = newnode;
			newnode->prev = predecessor(newnode)->end;
		}
	}

	tree->size++;
	updateHeight(newnode);
	avlFixUp(tree, newnode);
}

void printL(TTree *tree)
{
	TreeNode *y = maximum(tree->root);
	while (y != NULL)
	{
		printf("%d   ", *(int *)y->elem);
		y = y->prev;
	}
	printf("\n");
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

void link_node_parent(TTree *tree, TreeNode *first, TreeNode *second)
{
	if (first->parent == NULL)
	{
		tree->root = second;
	}
	else if (first == first->parent->left)
	{
		first->parent->left = second;
	}
	else
	{
		first->parent->right = second;
	}
	if (second)
	{
		second->parent = first->parent;
	}
}
/* Eliminarea unui nod din arbore
 *
 * elem: cheia nodului ce trebuie sters
 * 	! In cazul in care exista chei duplicate
 *	  se va sterge ultimul nod din lista de duplicate
 */
void delete (TTree *tree, void *elem)
{
	if (tree == NULL || tree->root == NULL)
		return;
	// printf("%d        o\n", *(int*)tree->root->right->elem);
	if (tree->root->left == NULL && tree->root->right == NULL)
	{
		// printf("dhgdhsjdhdhjkdjh\n");
		destroyTreeNode(tree, tree->root);
		tree->root = NULL;
		tree->size--;
		return;
	}
	TreeNode *defnode = search(tree, tree->root, elem);
	if (defnode == NULL)
		return;
	TreeNode *par = defnode->parent;
		// if(defnode!=maximum(tree->root))
		// if(defnode->prev)
	 	//  printf("%d elem %d end  \n", *(int*)elem, *(int*)defnode->prev->info);
	if (defnode->end != defnode)
	{
		TreeNode *aux = defnode->end;
		if (defnode->end->next)
			defnode->end->next->prev = defnode->end->prev;
		defnode->end->prev->next = defnode->end->next;
		defnode->end = defnode->end->prev;
		destroyTreeNode(tree, aux);
		tree->size--;
		return;
	}

	if(predecessor(defnode)) {
		predecessor(defnode)->end->next = defnode->next;
		if(defnode->next)
		defnode->next->prev = predecessor(defnode)->end;
	} 
	if(successor(defnode)) {
		successor(defnode)->prev = defnode->prev;
		if(defnode->prev)
		defnode->prev->end->next = successor(defnode);
	} 

	if (defnode->right == NULL && defnode->left != NULL)
	{ // are doar un copil
		link_node_parent(tree, defnode, defnode->left);
	}
	else if (defnode->left == NULL && defnode->right != NULL)
	{
		link_node_parent(tree, defnode, defnode->right);
	}
	else if (defnode->left != NULL && defnode->right != NULL)
	{
		TreeNode *succ = successor(defnode);
		link_node_parent(tree, succ, succ->right);
		link_node_parent(tree, defnode, succ);
		succ->right = defnode->right;
		succ->parent = defnode->parent;
		if (succ->right)
		{
			succ->right->parent = succ;
		}
		succ->left = defnode->left;
		if (succ->left)
			succ->left->parent = succ;
	}
	else if (defnode->left == NULL && defnode->right == NULL)
	{
		if (par->left == defnode)
		{
			par->left = NULL;
		}
		else if (par->right == defnode)
			par->right = NULL;
	}
	// printf("%d %d\n", *(int*)defnode->elem, *(int*)par->elem);
	

	destroyTreeNode(tree, defnode->end);
	tree->size--;
	if (par)
		avlDeleteFixUp(tree, par);
	// printf("%d   ", *(int*)tree->root->elem);
	// printf("%d        p\n", *(int*)tree->root->right->elem);
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

	TreeNode *aux = maximum(tree->root);
	while(aux != NULL) {	
		TreeNode *prev = aux;
		aux = aux->prev;
		destroyTreeNode(tree, prev);
	}
	if(tree) 
	free(tree);
	//TODO: Cerinta 1g
}