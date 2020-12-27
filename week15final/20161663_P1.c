#include <stdio.h>
#include <stdlib.h>
#define TRUE		1
#define FALSE		0
#define NONE	 -1

//	필수 구현 함수
int empty(int* a);
void push(int** a, int* element_size, int* alloc_size, int value);
int pop(int** a, int* element_size, int* alloc_size);
int front(int* a, int element_size);
int back(int* a, int element_size);
int print_all(int* a, int element_size);

//	추가 정의한 함수
void routine(int* a, int* element_size, int* alloc_size);

int main(void) {
	int* vector = NULL;		//	Vector를 가리킬 포인터 변수
	int element_size = 0;	//	Vector에 저장된 원소의 개수(초기값 = 0)		
	int alloc_size;				//	Vector를 위해 할당된 배열의 크기

	/*	Vector 초기화	*/
	vector = (int*)malloc(sizeof(int));
	vector[0] = element_size;
	alloc_size = 1;	//	처음 할당된 배열 크기 = 1

	routine(vector, &element_size, &alloc_size);
	return 0;
}

//	필수 구현 함수
int empty(int* a) {
	return (a[0] == 0) ? TRUE : FALSE;				
}

void push(int** a, int* element_size, int* alloc_size, int value) {
	int* temp = *a;	//	메모리 누수 방지 위한 backup

	if(*alloc_size == *element_size + 1) {	//	Vect가or가 꽉 찼을 경우
		*alloc_size *= 2;	//	Vector의 크기를 2배로 증가
		*a = realloc(*a, sizeof(int) * (*alloc_size));	//	새로 할당

		if(!(*a)) {	//	realloc 실패했을 경우
			printf("메모리 할당(2배 증가) 실패!\n");
			free(temp);
			exit(-1);
		}
	}
	
	(*a)[0] = ++(*element_size);	//	원소의 개수를 1 증가 후 저장
	(*a)[*element_size] = value;	//	새로운 원소 추
}

int pop(int** a, int* element_size, int* alloc_size) {
	if(*element_size == 0) return NONE;

	int* temp = *a;
	int ret = (*a)[*element_size];

	if(*alloc_size == 2 * (*element_size)) {	//	원소 제거 시 Vector 크기 절반으로 줄일 수 있는 경우
		*alloc_size /= 2;		//	Vector의 크기를 절반으로 감소
		*a = realloc(*a, sizeof(int) * (*alloc_size));	//	새로 할당

		if(!(*a)) {	//	realloc 실패했을 경우
			printf("메모리 할당(절반으로 감소) 실패\n");
			free(temp);
			exit(-1);
		}
	}

	(*a)[0] = --(*element_size);	//	원소 개수 1 감소 후 저장
	return ret; 			
}

int front(int* a, int element_size) {
	if(element_size == 0) return NONE;
	return a[1];	//	a[0]는 원소의 개수, a[1]이 첫 번째 원소
}

int back(int* a, int element_size) {
	if(element_size == 0) return NONE;
	return a[element_size];	//	가장 마지막 원소 반환
}

int print_all(int* a, int element_size) {
	if(a[0] == 0) return NONE;			
	
	int i;

	//	vector의 가장 앞의 원소의 개수를 제외하고 모두 출력
	for(i = 1; i <= element_size; i++)
		printf("%d ", a[i]);
	printf("\n");
	return TRUE;
}

//	추가 정의한 함수 구현
//	vector를 다루는 함수
void routine(int* a, int* element_size, int* alloc_size) {
	int key;		//	입력받은 키
	int value;	//	Vector에 삽입할 원소
	int ret;		//	반환된 값


	while(TRUE) {
		scanf("%d", &key);
		switch(key) {
		case 0: 
			printf("%d\n", empty(a));
			break;
		case 1:
			scanf("%d", &value);
			push(&a, element_size, alloc_size, value);
			printf("%d\n", *alloc_size);
			break;
		case 2:
			ret = pop(&a, element_size, alloc_size);
			if(ret == NONE) printf("%d\n", ret);
			else printf("%d %d\n", ret, *alloc_size);
			break;
		case 3:
			printf("%d\n", front(a, *element_size));
			break;
		case 4:
			printf("%d\n", back(a, *element_size));
			break;
		case 5:
			ret = print_all(a, *element_size);
			if(ret == NONE) printf("%d\n", ret);
		}				
	}
}
