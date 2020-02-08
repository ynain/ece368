#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "sorting.h"
#include <math.h>


/******************************
*Yash Nain
*sorting.c: 6/18/18.
*ynain@purdue.edu
*Compiling: [gcc -Werror -lm -Wall -O3 sorting.c sorting_main.c -o proj1]
*******************************/

int getSequenceSize(int length);
int * getSequence(int length);
void printArray(long * Array, int Size);
void insertionSort(int * sequence, int length);


/*
	loads the file into Arr and returns Arr, size of array is stored in *Size
	when there is a file error, or memory allocation error, return NULL, and	
	set *Size to 0
*/
long *Load_From_File(char *Filename, int *Size)
{
    long *Arr = NULL;
    *Size = 0;

	FILE * fp = fopen(Filename, "r");
	if (fp == NULL){
		return NULL;
	}

	if (!feof(fp)){
		fscanf(fp, "%d", Size);
	}

	Arr = malloc(sizeof(*Arr) * *Size);
	if (Arr == NULL){
		fclose(fp);
		*Size = 0;
		return NULL;
	}
	
	int i;
	for (i = 0; i < *Size; fscanf(fp, "%ld", &Arr[i++]));

	fclose(fp);

    return Arr;
}

/*
	Save the Array to the file Filename
	Return the number of elements saved to file
*/
int Save_To_File(char *Filename, long *Array, int Size)
{
    int n_written = 0;

	FILE * fp = fopen(Filename, "w");
	if (fp == NULL){
		return n_written;
	}

	if (Size != 0){
		for (; n_written < Size; fprintf(fp, "%ld \n", Array[n_written++]));	
	}

	fclose(fp);
    return n_written;
}


/*
	Print the sequence in the order in which it appears in the triangle
 	2^(p)3^(q) is the largest number, 
	If Size is 0 or 1, an empty file should be created
*/
int Print_Seq(char *Filename, int length)
{
	int seq_size = getSequenceSize(length);
	int * sequence = getSequence(length);

	FILE * fp = fopen(Filename, "w");
	
	int i;
	for (i = 0; i < seq_size; fprintf(fp, "%d\n", sequence[i++]));

	fclose(fp);
	free(sequence);

	return seq_size;
}



void Shell_Insertion_Sort(long *a, int length, double *ncomp, double *nswap)
{
	/* 
	 * Get sorted sequence from function
	 * Get size of sequence
	 * Sort sequence
	 * Cycle through each, perform sorting
	 */

	int seq_size = getSequenceSize(length);
	int * sequence = getSequence(length);
	insertionSort(sequence, seq_size);
	
	int k = 0, gap;
	for (k = seq_size-1; k >= 0; k--) {
		gap = sequence[k];
		int i, j;
		//for (j = gap, i = 0; j < length; j++) {
		for (j = gap; j < length; j++) {
			long temp = a[j];
			i = j;
			while ((i >= gap) && (a[i-gap] > temp)) {
				a[i] = a[i-gap];
				i -= gap;
				*ncomp += 1.0;
				*nswap += 1.0;
			}
			*ncomp+=1.0;
			a[i] = temp;
		}
	}
	free(sequence);
}


void Shell_Selection_Sort(long *a, int length, double *ncomp, double *nswap)
{
	/* 
	 * Get sorted sequence from function
	 * Get size of sequence
	 * Sort sequence
	 * Cycle through each, perform selection sorting
	 */
	
	int seq_size = getSequenceSize(length);
	int * sequence = getSequence(length);
	insertionSort(sequence, seq_size);
	
	int i, j, k, gap;
	for (k = seq_size-1; k >= 0; k--){
		gap = sequence[k];

		for (i = length - 1; i > 0; i -= gap) {
			int max_index = 0;
			for (j = gap; j <= i; j += gap){
				if (a[j] >= a[max_index]) {
					max_index = j;
				}
				*ncomp += 1.0;
			}
			long temp = a[max_index];
			a[max_index] = a[i];
			a[i] = temp;

			*nswap += 1.0;
		}
	}
	free(sequence);
}

void printArray(long * Array, int Size) {
	int i;
	for (i = 0; i < Size; i++){
		printf("%ld ", Array[i]);
	}
	printf("\n");
}

int * getSequence(int length){
	/*
	 * Allocate enough memory for array
	 * Generate numbers
	 * Return array of numbers
	 */

	int * sequence = malloc(sizeof(*sequence) * getSequenceSize(length));
	int k, p = 0, q = 0, seq_val = 0, seq_size = 0;

	for (k = 0; pow(2, p) <= length; k++){
		for (p = k, q = 0; p >= 0 && q <= k;){
			seq_val = pow(2, p--) * pow(3, q++);
			if (seq_val > length){
				break;
			}	
			sequence[seq_size++] = seq_val;
		}
	}
	return sequence;
}

int getSequenceSize(int length){
	int k, p = 0, q = 0, seq_size = 0, seq_val = 0;

	for (k = 0; pow(2, p) <= length; k++){
		for (p = k, q = 0; p >= 0 && q <= k;){
			seq_val = pow(2, p--) * pow(3, q++);
			if (seq_val > length){
				break;
			}
			seq_size++;
		}
	}
	return seq_size;
}

/*
 * Simple insertion sort, to order the gap sequence,
 * which reduces overall time
 */
void insertionSort(int * sequence, int length){
	int i, j;
	for (i = length-1; i > 0; i--){
		int max_index = 0;
		for (j = 1; j <= i; j++){
			if (sequence[j] >= sequence[max_index]){
				max_index = j;
			}
		}
		int temp = sequence[max_index];
		sequence[max_index] = sequence[i];
		sequence[i] = temp;
	}
}
