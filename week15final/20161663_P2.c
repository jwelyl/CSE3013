#include <stdio.h>
#include <stdlib.h>
#define MAX_LEN		1001	//	'\0' 포함 1001
#define MAX_ADD		 101	//	'\0' 포함 101
#define LEFT_END	 0		//	가장 왼쪽 커서(DUMMY 노드)

typedef struct node {
	char value;
	struct node* next;
	struct node* prev;
} NODE;

//	필수 구현 함수
void left(NODE** cursor);
void right(NODE** cursor);
void del(NODE** cursor);
void add(NODE** cursor, char* data);
void quit(NODE* head);

//	추가 정의한 함수
NODE* makeNode(char data);
void deleteNL(char* data);
void insertNode(NODE** cursor, char data);
void routine(NODE* head, NODE** cursor, char* data);

int main(void) {
	int idx = 0;
	char initText[MAX_LEN];	//	처음 문자열(최대 길이 1000)
	char input[MAX_ADD];		//	추가로 입력할 문자열(최대 길이 100)
	
	NODE* text = NULL;		//	문자열을 저장할 doubly linked list header
	NODE* cursor = NULL;	//	문자열의 커서

	fgets(initText, MAX_LEN, stdin);	//	처음 문자열 입력
	
	//	처음 문자열 전처리
	deleteNL(initText);
		
	text = makeNode(LEFT_END);	//	header : dummy node
	cursor = text;
	add(&cursor, initText);

	routine(text, &cursor, input);
	free(text);	//	header dummy node 해제

	return 0;
}

//	필수 구현 함수
void left(NODE** cursor) {
	if((*cursor)->value == LEFT_END) return;	//	이미 가장 왼쪽 커서일 경우
	*cursor = (*cursor)->prev;	//	커서 왼쪽으로 이동
}

void right(NODE** cursor) {
	if(!((*cursor)->next)) return;	//	이미 가장 오른쪽 커서일 경우
	*cursor = (*cursor)->next;	//	커서 오른쪽으로 이동				
}

void del(NODE** cursor) {
	if((*cursor)->value == LEFT_END) return;	//	커서가 가장 왼쪽에 있을 경우
	
	NODE* del = *cursor;
	
	del->prev->next = del->next;
	if(del->next) 	del->next->prev = del->prev;	// 다음 노드와 이전 노드를 연결
	(*cursor) = del->prev;
	del->prev = NULL;
	del->next = NULL;
	free(del);	//	메모리 해제
}

void add(NODE** cursor, char* data) {
	int idx = 0;

	while(data[idx] != 0)
		insertNode(cursor, data[idx++]);	//	문장 안의 모든 문자를 노드에 삽입
}

void quit(NODE* head) {
	NODE* cur = head->next;
	FILE* fp = fopen("data.txt", "w");
	
	//	header를 제외한 문자가 저장되어 있는 노드들에서 문자를 출력 후 메모리 해제
	//	header는 main 함수에서 해제
	for(; cur; cur = cur->next) {
		fprintf(fp, "%c", cur->value);
		del(&cur);
	}
}

//	추가 정의한 함수 구현
//	NODE 동적 할당 후 반환 함수
NODE* makeNode(char data) {
	NODE* node = (NODE*)malloc(sizeof(NODE));
	node->value = data;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

//	문자열에서 '\n' 문자 없애는 함
void deleteNL(char* data) {
	int idx = 0;

	//	문자열 전처리
	while(data[idx] != 0) {	
		if(data[idx] == '\n') {	//	'\n'을 '\0'으로 바꿈 
			data[idx] = 0;
			break;
		}
		idx++;
	}
}

//	이중 연결리스트에 노드를 삽입하는 함수
//	(현재 커서 오른쪽에 삽입 후 삽입된 문자 뒤에 커서를 위치)
void insertNode(NODE** cursor, char data) {
	NODE* node = makeNode(data);

	node->prev = *cursor;
	if((*cursor)->next) {
		node->next = (*cursor)->next;
		(*cursor)->next->prev = node;
	}

	//	현재 커서 오른쪽에 노드 삽입
	(*cursor)->next = node;
	//	커서를 삽입된 문자 뒤에 위치
	*cursor = node; 			
}

//	명령 함수
void routine(NODE* head, NODE** cursor, char* data) {
	char key;	//	입력받은 키

	while(1) {
		scanf("%c", &key);

		if(key == 'L' || key == 'l') {
			getchar();	//	'\n'키 제거
			left(cursor);
		}
		else if(key == 'R' || key == 'r') {
			getchar();
			right(cursor);
		}
		else if(key == 'D' || key == 'd') {
			getchar();
			del(cursor);
		}
		else if(key == 'A' || key == 'a') {
			getchar();	//	입력키 'A'와 추가할 문장 사이의 공백 제거
			fgets(data, MAX_ADD, stdin);
			deleteNL(data);
			add(cursor, data);
		}
		else if(key == 'Q' || key == 'q') {
			getchar();
			quit(head);
			break;
		}
	}
}

