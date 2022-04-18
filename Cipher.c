#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>

#include "Cipher.h"



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
	return NULL;
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
	return NULL;
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