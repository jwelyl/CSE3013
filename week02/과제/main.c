#include "header.h"

int main(void) {
	int T, N, i;
	int num[10];	//	0~9까지의 숫자의 개수를 저장한 배열
	double duration;

	scanf("%d", &T);
	for(i = 0; i < T; i++) {
		scanf("%d", &N);
		
		clock_t start = clock();	
		initArray(num);
		countNum(N, num);
		printNum(num);
		duration = ((double)(clock() - start)) / CLOCKS_PER_SEC;
		printf("duration : %f\n", duration);
	}

	return 0;
}

