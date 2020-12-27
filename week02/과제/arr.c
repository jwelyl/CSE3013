#include "header.h"

void initArray(int* arr) {
	int i;
	for(i = 0; i < LEN; i++)
		arr[i] = 0;
}

void printNum(int* arr) {
	int i;
	for(i = 0; i < LEN; i++)
		printf("%d ", arr[i]);
	printf("\n");		
}
