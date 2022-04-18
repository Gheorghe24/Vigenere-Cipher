#ifndef CIPHER_H_
#define CIPHER_H_

#include "TreeMap.h"

/* Lungimea maxima a unui buffer */
#define BUFLEN 1024

/* Lungimea unui element din arbore */
#define ELEMENT_TREE_LENGTH 5

/* Range de offset-uri */
typedef struct Range{
	int *index;
	int size;  		// Numarul curent de indecsi din range
	int capacity;	/* Capacitatea maxima a range-ului
					 * daca size == capacity se va aloca memorie suplimentara
					 * (se va dubla capacitatea)
					 */
}Range;

void buildTreeFromFile(char* fileName, TTree* tree);

void encrypt(char *inputFile, char *outputFile, Range *key);
void decrypt(char *inputFile, char *outputFile, Range *key);

void printKey(char *fileName, Range *key);
Range* inorderKeyQuery(TTree* tree);
Range* levelKeyQuery(TTree* tree);
Range* rangeKeyQuery(TTree* tree, char* q, char* p);


#endif /* CIPHER_H_ */