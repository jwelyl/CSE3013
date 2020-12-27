#include <stdio.h>
#include <time.h>
#define LEN			10

void initArray(int* arr);							//	계산을 위해 배열의 모든 원소를 0으로 초기화
void currentNum(int page, int* arr);	//	현재 페이지의 숫자들의 개수를 계산하여 반영함
void countNum(int N, int* arr);				//	책의 페이지의 모든 숫자들의 개수를 계산함.
void printNum(int* arr);							//	배열을 출력함.


