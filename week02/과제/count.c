#include "header.h"

void countNum(int N, int* arr) {
	int pg;
	for(pg = 1; pg <= N; pg++) {
		currentNum(pg, arr);
	}				
}

void currentNum(int page, int* arr) {
	int i;
	do {
		i = page % 10;
		arr[i] += 1;
		page /= 10;
	} while(page != 0);			
}

/*
void currentNum(int page, int* arr) {
	int i;
	if(page / 10 == 0) {	//	한 자릿수일 경우
		arr[page] += 1;
		return;
	} else {
		i = page % 10;
		page /= 10;
		arr[i] += 1;
		currentNum(page, arr);
	}
}*/
