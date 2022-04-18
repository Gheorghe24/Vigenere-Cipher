#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreeMap.h"
#include "Cipher.h"

#define ASSERT(f, cond, msg) if (!(cond)) { failed(f, msg); return; } else passed(f, msg);


void failed(FILE *f, char* msg) {
	fprintf(f, "%s ...... failed\n", msg);
	fclose(f);
}


void passed(FILE *f, char* msg) {
	fprintf(f, "%s ...... passed\n", msg);
}


void* createLong(void* value) {
	long *l = malloc(sizeof(long));
	*l = *((long*) (value));
	return l;
}


void destroyLong(void* value) {
	free((long*)value);
}


int compareLong(void* a, void* b) {
	if(*((long*)a) < *((long*)b)) return -1;
	if(*((long*)a) > *((long*)b)) return  1;
	return 0;
}


void* createStrElement(void* str){
	char* elem = (char*)malloc(ELEMENT_TREE_LENGTH + 1);
	strncpy(elem, (char*)str, ELEMENT_TREE_LENGTH);
	elem[ELEMENT_TREE_LENGTH] = '\0';
	return elem;
}


void destroyStrElement(void* elem){
	free((char*)elem);
}


void* createIndexInfo(void* index){
	int *idx = (int*) malloc(sizeof(int));
	*idx = *((int*) index);
	return idx;
}


void destroyIndexInfo(void* index){
	free(index);
}


int compareStr(void* str1, void* str2) {

	if (strncmp((char*)str1,(char*)str2, ELEMENT_TREE_LENGTH) > 0)
		return 1;
	else if (strncmp((char*) str1,(char*) str2, ELEMENT_TREE_LENGTH) < 0)
		return -1;
	return 0;
}


void print_dot_aux(TreeNode* node, FILE* f, int type) {

	if (node->left) {
		if (type == 1)
			fprintf(f, "    %d -> %d;\n", *((int*)node->elem), *((int*)node->left->elem));
		else
			fprintf(f, "    %s -> %s;\n", (char*)node->elem, (char*)node->left->elem);

		print_dot_aux(node->left, f, type);
	}

	if (node->right) {
		if (type == 1)
			fprintf(f, "    %d -> %d;\n", *((int*)node->elem), *((int*)node->right->elem));
		else
			fprintf(f, "    %s -> %s;\n", (char*)node->elem, (char*)node->right->elem);

		print_dot_aux(node->right, f, type);
	}
}


void print_dot(TreeNode* root, FILE* f, int type) {

	fprintf(f, "digraph BST {\n");
	if (type == 1)
		fprintf(f, "    node [fontname=\"Arial\", shape=circle, style=filled, fillcolor=green];\n");
	else
		fprintf(f, "    node [fontname=\"Arial\", shape=circle, style=filled, fillcolor=yellow];\n");

	if (!root)
		fprintf(f, "\n");
	else if (!root->right && !root->left) {
		if (type == 1)
			fprintf(f, "    %d;\n", *((int*)root->elem));
		else
			fprintf(f, "    %s;\n", ((char*)root->elem));

	} else
		print_dot_aux(root, f, type);

	fprintf(f, "}\n");
}


void create_simple_tree(TTree **tree) {

	long values[] = {5, 3, 2, 4, 7, 6, 8};

	TreeNode *node = createTreeNode((*tree), values, values);

	if (node == NULL) {
		return;
	}

	(*tree)->root = node;

	node = createTreeNode((*tree), values + 1, values + 1);
	(*tree)->root->left = node;
	node->parent = (*tree)->root;

	node = createTreeNode((*tree), values + 2, values + 2);
	(*tree)->root->left->left = node;
	node->parent = (*tree)->root->left;

	node = createTreeNode((*tree), values + 3, values + 3);
	(*tree)->root->left->right = node;
	node->parent = (*tree)->root->left;

	node = createTreeNode((*tree), values + 4, values + 4);
	(*tree)->root->right = node;
	node->parent = (*tree)->root;

	node = createTreeNode((*tree), values + 5, values + 5);
	(*tree)->root->right->left = node;
	node->parent = (*tree)->root->right;

	node = createTreeNode((*tree), values + 6, values + 6);
	(*tree)->root->right->right = node;
	node->parent = (*tree)->root->right;
}


void test_init(TTree** tree) {

	FILE *f = fopen("outputs/output_init.out", "w");

	if (f == NULL) {
		printf("Error opening file!\n");
		return;
	}

	// Test create and isEmpty
	(*tree) = createTree(createLong, destroyLong,
						 createLong, destroyLong, compareLong);

	ASSERT(f, (*tree) != NULL, "CreateTree-01");
	ASSERT(f, (*tree)->createElement != NULL, "CreateTree-02");
	ASSERT(f, (*tree)->createInfo != NULL, "CreateTree-03");
	ASSERT(f, (*tree)->destroyElement != NULL, "CreateTree-04");
	ASSERT(f, (*tree)->destroyInfo != NULL, "CreateTree-05");
	ASSERT(f, (*tree)->compare != NULL, "CreateTree-06");
	ASSERT(f, (*tree)->root == NULL, "CreateTree-07");
	ASSERT(f, (*tree)->size == 0, "CreateTree-08");
	ASSERT(f, isEmpty((*tree)) == 1, "IsEmpty-01");

	fprintf(f, "\nAll tests for Init passed!\n");

	fclose(f);
}


void test_search(TTree** tree) {

	FILE *f = fopen("outputs/output_search.out", "w");

	if (f == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if (*tree == NULL) {
		fprintf(f, "Empty tree passed!\n");
		fclose(f);
		return;
	}

	long value = 4;
	ASSERT(f, (search((*tree), (*tree)->root, &value) != NULL) == 1, "Search-01");

	value = 10;
	ASSERT(f, (search((*tree), (*tree)->root, &value) != NULL) == 0, "Search-02");

	value = 5;
	ASSERT(f, search((*tree), (*tree)->root, &value)->left == (*tree)->root->left, "Search-04");
	ASSERT(f, search((*tree), (*tree)->root, &value)->right == (*tree)->root->right, "Search-05");

	fprintf(f, "\nAll tests for Search passed!\n");
	fclose(f);
}



void test_minmax(TTree** tree) {

	FILE *f = fopen("outputs/output_minmax.out", "w");

	if (f == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if (*tree == NULL || (*tree)->root == NULL) {
		fprintf(f, "Empty tree passed!\n");
		fclose(f);
		return;
	}

	ASSERT(f, minimum((*tree)->root) != NULL, "MinMax-01");
	ASSERT(f, *((long*)minimum((*tree)->root)->elem) == 2l, "MinMax-02");
	ASSERT(f, minimum((*tree)->root)->left == NULL, "MinMax-03");
	ASSERT(f, *((long*)minimum((*tree)->root->right)->elem) == 6l, "MinMax-04");

	ASSERT(f, maximum((*tree)->root) != NULL, "MinMax-05");
	ASSERT(f, *((long*)maximum((*tree)->root)->elem) == 8l, "MinMax-06");
	ASSERT(f, maximum((*tree)->root)->right == NULL, "MinMax-07");
	ASSERT(f, *((long*)maximum((*tree)->root->left)->elem) == 4l, "MinMax-08");

	fprintf(f, "\nAll tests for Min Max passed!\n");
	fclose(f);
}


void test_succ_pred(TTree** tree) {
	FILE *f = fopen("outputs/output_succ_pred.out", "w");
	long value;

	if (f == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if (*tree == NULL || (*tree)->root == NULL) {
		fprintf(f, "Empty tree passed!\n");
		fclose(f);
		return;
	}

	// Successor Tests
	ASSERT(f, successor((*tree)->root) != NULL, "Successor-01 ");

	ASSERT(f, successor(maximum((*tree)->root)) == NULL,
		"Successor-02 ");

	value = 3;
	ASSERT(f, *((long*)successor(search((*tree),
		(*tree)->root, &value))->elem) == 4l, "Successor-03 ");

	value = 4;
	ASSERT(f, *((long*)successor(search((*tree),
		(*tree)->root, &value))->elem) == 5l, "Successor-04 ");

	// Predecessor Tests
	ASSERT(f, predecessor((*tree)->root) != NULL, "Predecesor-01");

	ASSERT(f, predecessor(minimum((*tree)->root)) == NULL,
		"Predecesor-02");

	value = 3;
	ASSERT(f, *((long*)predecessor(search((*tree),
		(*tree)->root, &value))->elem) == 2l, "Predecesor-03");

	value = 6;
	ASSERT(f, *((long*)predecessor(search((*tree),
		(*tree)->root, &value))->elem) == 5l, "Predecesor-04");

	fprintf(f, "\nAll tests for Successor Predecesor passed!\n");
	fclose(f);
}


void test_rotations(TTree** tree) {

	FILE *f = fopen("outputs/output_rotations.out", "w");

	if (f == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if (*tree == NULL) {
		fprintf(f, "Empty tree passed!\n");
		fclose(f);
		return;
	}

	avlRotateLeft((*tree), (*tree)->root);

	ASSERT(f, *((long*)((*tree)->root)->elem) == 7l, "Rotate-01");
	ASSERT(f, *((long*)((*tree)->root->left)->elem) == 5l, "Rotate-02");
	ASSERT(f, *((long*)((*tree)->root->right)->elem) == 8l, "Rotate-03");
	ASSERT(f, *((long*)((*tree)->root->left->left)->elem) == 3l, "Rotate-04");
	ASSERT(f, *((long*)((*tree)->root->left->right)->elem) == 6l, "Rotate-05");


	avlRotateRight((*tree), (*tree)->root);
	ASSERT(f, *((long*)((*tree)->root)->elem) == 5l, "Rotate-06");
	ASSERT(f, *((long*)((*tree)->root->left)->elem) == 3l, "Rotate-07");
	ASSERT(f, *((long*)((*tree)->root->right)->elem) == 7l, "Rotate-08");
	ASSERT(f, *((long*)((*tree)->root->right->left)->elem) == 6l, "Rotate-09");
	ASSERT(f, *((long*)((*tree)->root->right->right)->elem) == 8l, "Rotate-10");

	fprintf(f, "\nAll tests for Rotations passed!\n");
	fclose(f);
}


void test_insert(TTree** tree) {
	FILE *f = fopen("outputs/output_insert.out", "w");

	if (f == NULL) {
		printf("Error opening file!\n");
		return;
	}

	(*tree) = createTree(createLong, destroyLong,
						 createLong, destroyLong, compareLong);

	if (*tree == NULL) {
		fprintf(f, "Empty tree passed!\n");
		fclose(f);
		return;
	}

	long values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

	// Test root
	insert(*(tree), values + 2, values + 2);
	ASSERT(f, isEmpty(*tree) != 1, "Insert-01");
	ASSERT(f, (*tree)->root->left == NULL, "Insert-02");
	ASSERT(f, (*tree)->root->right == NULL, "Insert-03");

	// Test Left Rotate
	insert((*tree), values + 3, values + 3);
	insert((*tree), values + 4, values + 4);
	ASSERT(f, *((long*)(*tree)->root->left->elem) == 2l, "Insert-04");
	ASSERT(f, *((long*)(*tree)->root->right->elem) == 4l, "Insert-05");

	// Test Right Rotate
	insert((*tree), values + 1, values + 1);
	insert((*tree), values, values);
	ASSERT(f, *((long*)(*tree)->root->left->elem) == 1l, "Insert-06");
	ASSERT(f, *((long*)(*tree)->root->left->left->elem) == 0l, "Insert-07");
	ASSERT(f, *((long*)(*tree)->root->left->right->elem) == 2l, "Insert-08");


	// Test Left Rotate
	insert((*tree), values + 5, values + 5);
	insert((*tree), values + 6, values + 6);
	ASSERT(f, *((long*)(*tree)->root->right->elem) == 5l,"Insert-09");
	ASSERT(f, *((long*)(*tree)->root->right->left->elem) == 4l,"Insert-10");
	ASSERT(f, *((long*)(*tree)->root->right->right->elem) == 6l,"Insert-11");


	// Test Zig-Zag case
	insert((*tree), values + 8, values + 8);
	insert((*tree), values + 7, values + 7);
	ASSERT(f, *((long*)(*tree)->root->right->right->elem) == 7l,"Insert-12");
	ASSERT(f, *((long*)(*tree)->root->right->right->left->elem) == 6l,"Insert-13");
	ASSERT(f, *((long*)(*tree)->root->right->right->right->elem) == 8l,"Insert-14");

	fprintf(f, "\nAll tests for Insert passed!\n");
	fclose(f);
}


void test_delete(TTree** tree) {

	FILE *f = fopen("outputs/output_delete.out", "w");

	if (f == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if (*tree == NULL || (*tree)->root == NULL) {
		fprintf(f, "Empty tree passed!\n");
		fclose(f);
		return;
	}

	// Test single node delete no child
	long value = 6;
	delete((*tree),&value);
	ASSERT(f, *((long*)(*tree)->root->elem) == 3l, "Delete-01");
	ASSERT(f, (*tree)->root->right->right->left == NULL, "Delete-02");

	// Test left re-balance
	value = 4;
	delete((*tree),&value);
	ASSERT(f, *((long*)(*tree)->root->right->elem) == 7l, "Delete-03");
	ASSERT(f, *((long*)(*tree)->root->right->left->elem) == 5l, "Delete-04");
	ASSERT(f, *((long*)(*tree)->root->right->right->elem) == 8l, "Delete-05");

	// Test splice successor
	value = 3;
	delete((*tree),&value);
	ASSERT(f, *((long*)(*tree)->root->elem) == 5l, "Delete-06");
	ASSERT(f, *((long*)(*tree)->root->right->elem) == 7l, "Delete-07");
	ASSERT(f, *((long*)(*tree)->root->right->right->elem) == 8l, "Delete-08");
	ASSERT(f, (*tree)->root->right->left == NULL, "Delete-09");

	// Test delete node with 1 child
	value = 7;
	delete((*tree),&value);
	ASSERT(f, *((long*)(*tree)->root->right->elem) == 8l, "Delete-10");
	ASSERT(f, (*tree)->root->right->height == 1, "Delete-11");

	// Test right re-balance
	value = 8;
	delete((*tree),&value);
	ASSERT(f, *((long*)(*tree)->root->elem) == 1l, "Delete-12");
	ASSERT(f, *((long*)(*tree)->root->left->elem) == 0l, "Delete-13");
	ASSERT(f, *((long*)(*tree)->root->right->elem) == 5l, "Delete-14");
	ASSERT(f, *((long*)(*tree)->root->right->left->elem) == 2l, "Delete-15");

	//Test zig-zag re-balance
	value = 0;
	delete((*tree),&value);
	ASSERT(f, *((long*)(*tree)->root->elem) == 2l, "Delete-16");
	ASSERT(f, *((long*)(*tree)->root->left->elem) == 1l, "Delete-17");
	ASSERT(f, *((long*)(*tree)->root->right->elem) == 5l, "Delete-18");

	// Test empty tree
	value = 2;
	delete((*tree),&value);
	value = 1;
	delete((*tree),&value);
	value = 5;
	delete((*tree),&value);
	ASSERT(f, (*tree)->root == NULL, "Delete-19");
	ASSERT(f, (*tree)->size == 0, "Delete-20");

	fprintf(f, "\nAll tests for Delete passed!\n");
	fclose(f);
}


void test_list_insert(TTree **tree) {

	FILE *f = fopen("outputs/output_list_insert.out", "w");

	if (f == NULL) {
		printf("Error opening file!\n");
		return;
	}

	long values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	for(int i = 0; i < sizeof(values)/sizeof(values[0]); i++)
		insert(*(tree), values + i, values + i);

	if (*tree == NULL || (*tree)->root == NULL) {
		fprintf(f, "Empty tree passed!\n");
		fclose(f);
		return;
	}

	ASSERT(f, minimum((*tree)->root)->prev == NULL, "ListInsert-01");
	ASSERT(f, maximum((*tree)->root)->end->next == NULL, "ListInsert-02");
	ASSERT(f, *((long*)(*tree)->root->elem) == 3, "ListInsert-03");
	ASSERT(f, (*tree)->root == (*tree)->root->end, "ListInsert-04");
	ASSERT(f, (*tree)->root->prev == predecessor((*tree)->root), "ListInsert-05");
	ASSERT(f, (*tree)->root->next == successor((*tree)->root), "ListInsert-06");

	insert(*(tree), values + 3, values + 4);
	ASSERT(f, (*tree)->root != (*tree)->root->end, "ListInsert-07");
	ASSERT(f, (*tree)->root->next == (*tree)->root->end, "ListInsert-08");
	ASSERT(f, (*tree)->root->prev == predecessor((*tree)->root), "ListInsert-09");
	ASSERT(f, (*tree)->root->end->next == successor((*tree)->root), "ListInsert-10");

	insert(*(tree), values + 3, values + 5);
	ASSERT(f, *((long*)(*tree)->root->elem) == *((long*)(*tree)->root->next->elem), "ListInsert-11");
	ASSERT(f, *((long*)(*tree)->root->elem) == *((long*)(*tree)->root->next->next->elem), "ListInsert-12");
	ASSERT(f, (*tree)->root->end == (*tree)->root->next->next, "ListInsert-13");
	ASSERT(f, (*tree)->root->next->next == successor((*tree)->root)->prev, "ListInsert-14");
	ASSERT(f, (*tree)->root == predecessor((*tree)->root)->next, "ListInsert-15");

	insert(*(tree),values, values);
	ASSERT(f, *((long*)minimum((*tree)->root)->elem) == 0 , "ListInsert-16");
	ASSERT(f, *((long*)minimum((*tree)->root)->next->elem) == 0 , "ListInsert-17");
	ASSERT(f, *((long*)minimum((*tree)->root)->end->next->elem) == 1 , "ListInsert-18");
	ASSERT(f, minimum((*tree)->root)->prev == NULL , "ListInsert-19");
	ASSERT(f, minimum((*tree)->root)->prev == NULL , "ListInsert-20");
	ASSERT(f, minimum((*tree)->root)->next->next == minimum((*tree)->root)->end->next , "ListInsert-21");

	insert(*(tree), values + 8, values + 8);
	ASSERT(f, *((long*)maximum((*tree)->root)->elem) == 8, "ListInsert-22");
	ASSERT(f, *((long*)maximum((*tree)->root)->next->elem) == 8, "ListInsert-23");
	ASSERT(f, *((long*)maximum((*tree)->root)->prev->elem) == 7l, "ListInsert-24");
	ASSERT(f, maximum((*tree)->root)->end->next == NULL , "ListInsert-25");
	ASSERT(f, maximum((*tree)->root)->next->next == NULL , "ListInsert-26");
	ASSERT(f, maximum((*tree)->root)->next->next == maximum((*tree)->root)->end->next , "ListInsert-27");

	fprintf(f, "\nAll tests for List Insert passed!\n");
	fclose(f);
}


void test_list_delete(TTree **tree) {

	FILE *f = fopen("outputs/output_list_delete.out", "w");

	if (f == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if (*tree == NULL || (*tree)->root == NULL) {
		fprintf(f, "Empty tree passed!\n");
		fclose(f);
		return;
	}

	long value = 3;
	delete((*tree), &value);
	ASSERT(f, *((long*)(*tree)->root->elem) == 3l, "ListDelete-01");
	ASSERT(f, *((long*)(*tree)->root->next->elem) == 3l, "ListDelete-02");
	ASSERT(f, *((long*)(*tree)->root->info) == 3l, "ListDelete-03");
	ASSERT(f, *((long*)(*tree)->root->end->info) == 4l, "ListDelete-04");

	ASSERT(f, *((long*)(*tree)->root->end->elem) == 3l, "ListDelete-05");
	ASSERT(f, (*tree)->root->end->elem == (*tree)->root->next->elem, "ListDelete-06");
	ASSERT(f, *((long*)(*tree)->root->end->next->elem) == 4l, "ListDelete-07");
	ASSERT(f, *((long*)(*tree)->root->prev->elem) == 2l, "ListDelete-08");

	delete((*tree), &value);
	ASSERT(f, *((long*)(*tree)->root->elem) == 3l, "ListDelete-09");
	ASSERT(f, *((long*)(*tree)->root->next->elem) == 4l, "ListDelete-10");
	ASSERT(f, *((long*)(*tree)->root->end->elem) == 3l, "ListDelete-11");
	ASSERT(f, (*tree)->root->end->next == (*tree)->root->next, "ListDelete-12");
	ASSERT(f, *((long*)(*tree)->root->end->next->elem) == 4l, "ListDelete-13");
	ASSERT(f, *((long*)(*tree)->root->prev->elem) == 2l, "ListDelete-14");

	delete((*tree), &value);
	ASSERT(f, *((long*)(*tree)->root->elem) == 4l, "ListDelete-15");
	ASSERT(f, *((long*)(*tree)->root->next->elem) == 5l, "ListDelete-16");
	ASSERT(f, *((long*)(*tree)->root->end->elem) == 4l, "ListDelete-17");
	ASSERT(f, (*tree)->root->end->next == (*tree)->root->next, "ListDelete-18");
	ASSERT(f, *((long*)(*tree)->root->end->next->elem) == 5l, "ListDelete-19");
	ASSERT(f, *((long*)(*tree)->root->prev->elem) == 2l, "ListDelete-20");

	value = 4;
	delete((*tree), &value);
	ASSERT(f, *((long*)(*tree)->root->elem) == 5l, "ListDelete-21");
	ASSERT(f, *((long*)(*tree)->root->next->elem) == 6l, "ListDelete-22");
	ASSERT(f, *((long*)(*tree)->root->end->elem) == 5l, "ListDelete-23");
	ASSERT(f, (*tree)->root->end->next == (*tree)->root->next, "ListDelete-24");
	ASSERT(f, *((long*)(*tree)->root->end->next->elem) == 6l, "ListDelete-25");
	ASSERT(f, *((long*)(*tree)->root->prev->elem) == 2l, "ListDelete-26");

	value = 8;
	delete((*tree), &value);
	ASSERT(f, *((long*)maximum((*tree)->root)->elem) == 8, "ListDelete-27");
	ASSERT(f, *((long*)maximum((*tree)->root)->end->elem) == 8, "ListDelete-28");
	ASSERT(f, *((long*)maximum((*tree)->root)->prev->elem) == 7l, "ListDelete-29");
	ASSERT(f, maximum((*tree)->root)->end->next == NULL , "ListDelete-30");
	ASSERT(f, maximum((*tree)->root)->next == NULL , "ListDelete-31");

	delete((*tree), &value);
	ASSERT(f, *((long*)maximum((*tree)->root)->elem) == 7, "ListDelete-32");
	ASSERT(f, *((long*)maximum((*tree)->root)->end->elem) == 7, "ListDelete-33");
	ASSERT(f, *((long*)maximum((*tree)->root)->prev->elem) == 6l, "ListDelete-34");
	ASSERT(f, maximum((*tree)->root)->end->next == NULL , "ListDelete-35");
	ASSERT(f, maximum((*tree)->root)->next == NULL , "ListDelete-36");

	value = 0;
	delete((*tree), &value);
	ASSERT(f, *((long*)minimum((*tree)->root)->elem) == 0, "ListDelete-37");
	ASSERT(f, *((long*)minimum((*tree)->root)->end->elem) == 0, "ListDelete-38");
	ASSERT(f, *((long*)minimum((*tree)->root)->next->elem) == 1l, "ListDelete-39");
	ASSERT(f, minimum((*tree)->root)->prev == NULL , "ListDelete-40");

	delete((*tree), &value);
	ASSERT(f, *((long*)minimum((*tree)->root)->elem) == 1, "ListDelete-41");
	ASSERT(f, *((long*)minimum((*tree)->root)->end->elem) == 1, "ListDelete-42");
	ASSERT(f, *((long*)minimum((*tree)->root)->next->elem) == 2l, "ListDelete-43");
	ASSERT(f, minimum((*tree)->root)->prev == NULL , "ListDelete-44");

	fprintf(f, "\nAll tests for List Delete passed!\n");
	fclose(f);
}


void test_free(TTree **tree1, TTree **tree2) {

	if ((*tree1) != NULL && (*tree1)->root != NULL) {
		destroyTreeNode((*tree1), (*tree1)->root->left->left);
		destroyTreeNode((*tree1), (*tree1)->root->left->right);
		destroyTreeNode((*tree1), (*tree1)->root->right->left);
		destroyTreeNode((*tree1), (*tree1)->root->right->right);
		destroyTreeNode((*tree1), (*tree1)->root->left);
		destroyTreeNode((*tree1), (*tree1)->root->right);
		destroyTreeNode((*tree1), (*tree1)->root);
		free(*tree1);
	}

	destroyTree(*tree2);
}


void draw_tree(TTree *tree, int type) {
	// type 1 - for int
	// type 2 - for strings

	FILE *f = fopen("tree.dot", "w");
	print_dot(tree->root, f, type);
	fclose(f);

	system("dot tree.dot | neato -n -Tpng -o tree.png");
}


void test_build_tree(TTree **tree) {

	FILE *f = fopen("outputs/simple_key_tree.dot", "w");

	buildTreeFromFile("inputs/simple_key.txt", (*tree));

	if (*tree == NULL) {
		fprintf(f, "Empty tree passed!\n");
		fclose(f);

		f = fopen("outputs/key_tree.dot", "w");
		fprintf(f, "Empty tree passed!\n");
		fclose(f);
		return;
	}

	if ((*tree)->root == NULL) {
		fprintf(f, "Invalid tree!\n");
		free(*tree);
		*tree = NULL;
		fclose(f);
	} else {
		print_dot((*tree)->root, f, 2);
		fclose(f);
		// system("dot outputs/simple_key_tree.dot | neato -n -Tpng -o outputs/simple_key_tree.png");
	}

	destroyTree(*tree);
	*tree = createTree(
		createStrElement,
		destroyStrElement,
		createIndexInfo,
		destroyIndexInfo,
		compareStr);

	buildTreeFromFile("inputs/key.txt", (*tree));

	f = fopen("outputs/key_tree.dot", "w");

	if ((*tree)->root == NULL) {
		fprintf(f, "Invalid tree!\n");
		free(*tree);
		*tree = NULL;
		fclose(f);

	} else {
		print_dot((*tree)->root, f, 2);
		fclose(f);
		// system("dot outputs/key_tree.dot | neato -n -Tpng -o outputs/key_tree.png");
	}
}



void test_inorder_key(TTree **tree) {

	Range *key = inorderKeyQuery((*tree));

	printKey("outputs/output_inorder_key.out", key);

	if (key != NULL) {
		decrypt("inputs/cipher1.txt", "outputs/cipher1.txt", key);
		free(key->index);
		free(key);
	}
}


void test_level_key(TTree **tree) {

	Range *key = levelKeyQuery((*tree));

	printKey("outputs/output_level_key.out", key);

	if (key != NULL) {
		decrypt("inputs/cipher2.txt", "outputs/cipher2.txt", key);
		free(key->index);
		free(key);
	}
}


void test_range_key(TTree **tree) {

	Range *key = rangeKeyQuery((*tree), "CD", "GG");

	printKey("outputs/output_range_key.out", key);

	if (key != NULL) {
		decrypt("inputs/cipher3.txt", "outputs/cipher3.txt", key);
		free(key->index);
		free(key);
	}
}


int main() {

	TTree *tree1 = NULL;
	TTree *tree2 = NULL;

	test_init(&tree1);
	create_simple_tree(&tree1);
	test_search(&tree1);
	test_minmax(&tree1);
	test_succ_pred(&tree1);
	test_rotations(&tree1);
	test_insert(&tree2);
	test_delete(&tree2);
	test_list_insert(&tree2);
	test_list_delete(&tree2);
	test_free(&tree1, &tree2);

	TTree *dict = NULL;
	dict = createTree(
		createStrElement,
		destroyStrElement,
		createIndexInfo,
		destroyIndexInfo,
		compareStr);

	test_build_tree(&dict);
	test_inorder_key(&dict);
	test_level_key(&dict);
	test_range_key(&dict);

	destroyTree(dict);

	return 0;
}