#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "huff.h"


/* PROGRAM LAYOUT
 * Open file, store contents in string
 * Count number of occurences of each character by traversing string
 * Use character count array to build tree
 * Perform preorder traversal of tree, store traversal codes (0 - left, 1 - right)
 * Write header to file
 * Traverse string, match corresponding character to traversal codes
 * Shift + write corresponding bits to file
 * Calculate amount of time taken to perform tasks
 */
int main(int argc, char * argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Invalid input");
		return EXIT_FAILURE;
	}

	/* Create two arrays
	 * 1.) stores binary tree sequence 
	 * 2.) stores count of each character
	 * */
	char ascii[ASCIISIZE];
	int asciiCount[ASCIISIZE];
	for (int i = 0; i < ASCIISIZE; ascii[i] = '\0', asciiCount[i++] = 0);

	/* Read and store file contents to string */
	char * file_str = getString(argv[1]);
	if (file_str == NULL){
		fprintf(stderr, "Unable to read input file");
		return EXIT_FAILURE;
	}

	//printf("%s\n", file_str);
	
	for (int j = 0; file_str[j] != '\0'; asciiCount[(int) file_str[j++]]++);

	
	Tree * huffTree = readTree(ascii);
	
	if (!huffTree){
		fprintf(stderr, "Unable to build Huffman binary tree");
	}
	
	/*
	// Write to array for preorder binary tree traversal
	getPreorder(&ascii, tree);

	// Name + open .huff file

	// For each element of array, shift bits to byte, write to .huff file
	
	unsigned char writeByte = 0x0;
	for (int k = 0; file_str[k] != '\0'; k++){
		// shift bits into byte
		fwrite
	}

	fclose
	
	*/
	destroyTree(huffTree);	

	free(file_str);

	return EXIT_SUCCESS;
}

char * mergeStrings (const char * str1, const char * str2) {
	return NULL;
}

/* Perform preorder traversal on tree, store codes in array
void getPreorder(char * ascii[], Tree * tree){
	char * str1, str2;
}
*/

/* Read and store file contents to string */

char * getString(char * Filename) {
	FILE * fptr = fopen(Filename, "r");
	if (fptr == NULL) {
		return NULL;
	}
	fseek(fptr, 0, SEEK_END);
	int length = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);

	char * inputStr = malloc(sizeof(*inputStr) * length);
	
	if (inputStr) {
		fread(inputStr, 1, length, fptr);
	}

	fclose(fptr);
	return inputStr;
}

/* Builds forest of trees, then builds single binary tree*/

Tree * readTree(char ascii[]){
	// Create linked list
	Node * list = NULL;

	for (int i = 0; i < ASCIISIZE; i++) {
		if (ascii[i]){
			// Create + add node to list in correct location
			list = addToList(list, buildList(NULL, buildTree(i, ascii[i], NULL, NULL)));
		}
	}

	// build binary tree from list
	return popTree(buildTreeFromList(list));
	// return NULL;	
}

/*
 * Build binary tree given linked list (forest of trees)
 * Find two smallest nodes, combine and push to front of list
 */
List * buildTreeFromList(Node * list) {
	// while list has more than one node
	while (list->next != NULL) {
		Tree * min1 = getMinimumNode(Node * list);
		Tree * min2 = getMinimumNode(Node * list);
		list = addToList(buildList(NULL, buildTree('\0', min1->count + min2->count, min1, min2)));
	}
	return list;
}

List * getMinimumNode(Node * list) {
	while (list->next != NULL) {
		list = list->next;
	}
	return popList(list);
}

/* Pushes new node to correct location in list */
List * addToList(Node * list, Node * newNode){
	Node * prev = list, head = list;
	if (head == NULL){
		return head;
	}
	while (true) {
		// If node is larger
		if (newNode->ptr->count > list->ptr->count) {
			break;
		}
		else if (newNode->ptr->count < list->ptr->count) {
			if (list->next != NULL) {
				prev = list;
				list = list->next;
			}
			else {
				break;
			}
		}
		else if (newNode->ptr->count == list->ptr->count) {
			if (newNode->ptr->label < list->ptr->label) {
				break;
			}
			else if (list->next != NULL) {
				prev = list;
				list = list->next;
			}
		}
	}
	pushNode(prev, newNode);
	return head;
}


/* Push node into linked list */
void pushNode(Node * pos, Node * newNode) {
	newNode->next = pos->next;
	pos->next = newNode;
}

/* Pop node from linked list */
List * popList(Node * node) {
	List * temp = node;
	node = node->next;
	temp->next = NULL;
	return temp;
}

/* Remove tree associated with list node */
Tree * popTree(Node * node) {
	Tree * tmp = node->ptr;
	node->ptr = NULL;
	destroyList(node);
	return tmp;
}

/* Build linked list node */
Node * buildList(Node * next, Tree * ptr){
	Node * list = malloc(sizeof(*list));
	list->next = next;
	list->ptr = ptr;
	return list;
}

/* Build binary tree node */
Tree * buildTree(char label, int count, Tree * left, Tree * right){
	Tree * tree = malloc(sizeof(*tree));
	tree->label = label;
	tree->count = count;
	tree->left = left;
	tree->right = right;
	return tree;
}

/* Release all memory associated with list */
void destroyList(Node * list){
	Node * temp;
	
	while (list != NULL) {
		temp = list;
		list = list->next;
		destroyTree(list->ptr);
		free(temp);
	}
}

/* Release all memory associated with tree */ 
void destroyTree(Tree * tree){
	if (tree == NULL){
		return;
	}
	destroyTree(tree->left);
	destroyTree(tree->right);
	free(tree);
}
