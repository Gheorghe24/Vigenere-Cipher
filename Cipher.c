#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Cipher.h"


void* createStrElement(void* str);
void destroyStrElement(void* elem);
/* Construieste un multi-dictionar pe baza unui fisier text
 * Cheia (elem) unui nod va fi reprezentata de un cuvant din text
 * iar valoarea (info) va fi indexul de inceput al acelui cuvant
 * ignorand caracterele separatoare (",.? \n\r") - i.e. numarul
 * de caractere A-Z precedente
 *
 * E.g: THIS IS AN EXAMPLE
 *
 * 			IS (4)
 *		    / \
 * 	  (6) AN  THIS (0)
 *         \
 *	 	 EXAMPLE (8)
 * */
void buildTreeFromFile(char* fileName, TTree* tree) {

	// Verificarea argumentelor
	if(fileName == NULL || tree == NULL)
		return;
	FILE *file = fopen(fileName, "r");
	int ind_len = 0;
	char *line = (char *) malloc(BUFLEN * sizeof(char));
	if(line == NULL) {
		return;
	}
	while (fgets(line, BUFLEN, file))
	{
		char *p = strtok(line, ",.? \n");
		while (p)
		{
			char *elem = createStrElement(p);
			insert(tree, elem, &ind_len);
			ind_len += (int)strlen(p);
			destroyStrElement(elem);
			p = strtok(NULL, ",.? \r\n");
		} 
	}
	
	fclose(file);
	free(line);

}


/* Functie pentru afisarea unei chei de criptare
 * O cheie este reprezentata print-o serie de offset-uri
 *
 * E.g: key = [1, 2, 3, 4]
 * input text  = A    A    A    A
 *			   +1|  +2|  +3|  +4|
 * 				 V    V    V    V
 * output text = B    C    D    E
 *
 */
void printKey(char *fileName, Range *key) {

	FILE *f = fopen(fileName, "w");

	if (key == NULL) {
		fprintf(f, "No key provided!\n");
		fclose(f);
		return;
	}

	fprintf(f, "Decryption key of length %d is:\n", key->size);

	// Afiseaza fiecare offset din cheie % 26 (lungimea
	// alfabetului A-Z)
	for (int i = 0; i < key->size; i++) {
		fprintf(f, "%d ", key->index[i] % 26);

		// Rand nou pentru lizibilitate
		if ((i + 1) % 10 == 0)
			fprintf(f, "\n");
	}

	fclose(f);
}


/* Returneaza cheia obtinuta de parcurgerea in ordine
 * crescatoare a nodurilor arborelui (parcurgand si listele
 * de duplicate)
 */
Range* inorderKeyQuery(TTree* tree) {
	if (tree == NULL || tree->root == NULL)
        return NULL;
	Range *key = (Range*) malloc(sizeof(Range));
	if(key == NULL) {
		return NULL;
	}
	int i = 0;
	key->size = 0;
	key->capacity = tree->size;
	key->index = malloc(sizeof(int) * tree->size);
	//parcurg de la minimum, ptu a salva indecsii in ordine crescatoare
	TreeNode *node = minimum(tree->root);
    while (node!=NULL)
	{	
		key->index[i++] = *(int *) node->info;
        key->size++;
        node = node->next;
	}
	return key;
}


/* Functie pentru extragerea cheii formate din valorile
 * nodurilor de la nivelul ce contine cel mai frecvent cuvant
 * (in cazul in care exista mai multe cuvinte cu numar maxim
 * de aparitii atunci se va considera primul nod dintre acestea conform
 * parcurgerii in inordine a arborelui)
 */
Range* levelKeyQuery(TTree* tree) {
    return NULL;
}




/* Extragerea cheii din nodurile aflate intr-un anumit
 * domeniu de valori specificat
 */
Range* rangeKeyQuery(TTree* tree, char* q, char* p) {
	if(tree == NULL || tree->root == NULL)
		return NULL;
	Range *key = malloc(sizeof(Range));
	if(key == NULL) {
		return NULL;
	}
	key->capacity = 30;
	//nu conteaza ce capacitate pun la inceput,oricum se dubleaza in cazul in care e prea putin
	//incepem cu size de la 0
	key->size = 0;
	key->index = malloc(sizeof(int) * key->capacity);
	TreeNode *node = minimum(tree->root);
	//parcurg iarasi lista de la inceput
	//insa de data asta caut noduri a carui element sa fie intre p si q
    while (node!=NULL)
	{	
		if(tree->compare(node->elem, q) == 1 && tree->compare(p, node->elem) == 1) {
			//maresc capacitatea de 2 ori in cazul in care e prea mic size-ul
			if(key->size == key->capacity){
				key->index = realloc(key->index, key->capacity * sizeof(int) * 2);
				key->capacity = key->capacity*2;
			}
			//adaug de fiecare data cand respect cerinta si maresc sizeul
			key->index[key->size] = *(int*)node->info;
			key->size++;
		}
		node = node->next;
	}
	return key;
}


void encrypt(char *inputFile, char *outputFile, Range *key) {

	FILE * f_in  = fopen(inputFile,  "r");
	FILE * f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char*) malloc(BUFLEN+1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL) {

		for (int i = 0; i < strlen(buff); i++) {
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r') {
				c = ((toupper(buff[i]) - 'A') + key->index[idx] % 26) % 26 + 'A';
				idx += 1;
			} else
			 	c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}


void decrypt(char *inputFile, char *outputFile, Range *key) {
 	
	FILE * f_in  = fopen(inputFile,  "r");
	FILE * f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char*) malloc(BUFLEN+1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL) {

		for (int i = 0; i < strlen(buff); i++) {
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r') {
				c = ((toupper(buff[i]) - 'A') - (key->index[idx] % 26) + 26) % 26 + 'A';
				idx += 1;
			} else
			 	c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}