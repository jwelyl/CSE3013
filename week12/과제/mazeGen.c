#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define FALSE	0
#define TRUE	1

void resetHeightWall(int* heightWall);
void resetWidthWall(int* widthWall);
void drawCeilingFloorOutline(FILE* fp);	//	천장, 바닥 outline 그림
void decideFirstRowWalls(int* maze, int* heightWall);	//	0번째 줄 벽 결정
void drawHeightWalls(int* heightWall, FILE* fp);		//	세로벽 "|" 그림
void drawWidthWalls(int* widthWall, FILE* fp);			//	가로벽 "-" 그림
void decideInterRowWalls(int* maze, int* widthWall, int* room);	//	위층과의 벽 결정
void decideMidRowWalls(int* maze, int* heightWall);	//	중간층의 벽 결정
void decideLastWalls(int* maze, int* heightWall);	//	마지막 층의 벽 지움
void drawMaze(int* maze, int* heightWall, int* widthWall, int* room, FILE* fp);	//	미로 그리는 함수, 위의 함수들을 호출함
void makeImperfectMaze(char** mazeArray, FILE* fp);	//	완전 미로로부터 불완전 미로를 만들고 파일에 출력함:

int WIDTH, HEIGHT;	//	미로의 너비, 높이
int mazeHeight, mazeWidth;	//	미로를 저장할 배열의 행, 열 크기
int delNum;	//	불완전 미로를 만들기 위해 추가로 지울 벽의 개수

int main(void) {
	char** mazeArray = NULL;	//	미로 정보를 저장할 2차원 char 배열

	int* maze = NULL;
	int* heightWall = NULL;	//	세로 벽 "|" 의 배열 (true면 벽 존재, false면 존재 x)
	int* widthWall = NULL;		//	가로 벽 "-" 의 배열 ('')
	FILE* fp;
	
	int room = 1;
	
	srand(time(NULL));

	scanf("%d %d", &WIDTH, &HEIGHT);
	fp = fopen("maze.maz", "w");	//	maze.maz 파일 열기
	
	mazeHeight = 2 * HEIGHT + 1;
	mazeWidth = 2 * WIDTH + 2;		//			'\n' 포함한 열의 크기
	delNum = (HEIGHT < WIDTH) ? HEIGHT : WIDTH;	
	delNum = delNum / 2;	//	불완전 미로를 위해 추가로 지울 벽의 개수

	maze = (int*)malloc(sizeof(int) * WIDTH);
	heightWall = (int*)malloc(sizeof(int) * WIDTH - 1);
	widthWall = (int*)malloc(sizeof(int) * WIDTH);
	mazeArray = (char**)malloc(sizeof(char*) * mazeHeight);
	for(int i = 0; i < mazeHeight; i++)
		mazeArray[i] = (char*)malloc(sizeof(char) * mazeWidth);
	
	drawMaze(maze, heightWall, widthWall, &room, fp);	//	완전 미로 생성 후 maze.maz 파일에 입력
	
	fclose(fp);
	free(maze);
	free(heightWall);
	free(widthWall);

	fp = fopen("maze.maz", "r");	//	maze.maz 파일로부터 완전 미로 정보를 읽어 mazeArray에 저장
	for(int i = 0; i < mazeHeight; i++) {
		for(int j = 0; j < mazeWidth; j++) {
			fscanf(fp, "%c", &mazeArray[i][j]);
		} 
	}
	fclose(fp);
	
	makeImperfectMaze(mazeArray, fp); //	완전 미로로부터 불완전 미로를 만든다.
	for(int i = 0; i < mazeHeight; i++)
		free(mazeArray[i]);
	free(mazeArray);
	
	return 0;
}

void resetHeightWall(int* heightWall) {
	for(int i = 0; i < WIDTH - 1; i++) 
		heightWall[i] = TRUE;	//	모든 벽이 있는 상태로 리셋함
}

void resetWidthWall(int* widthWall) {
	for(int i = 0; i < WIDTH; i++)
		widthWall[i] = TRUE;	//	모든 벽이 있는 상태로 리셋함
}

void drawCeilingFloorOutline(FILE* fp) {
	for(int i = 0; i < WIDTH; i++)
		fprintf(fp, "+-");
	fprintf(fp, "+\n");
}

void decideFirstRowWalls(int* maze, int* heightWall) {
	if(HEIGHT >= 2) {
		for(int i = 0; i < WIDTH - 1; i++) {
			int flag = rand() % 2;	//	랜덤 결정 (0일 때 벽을 허뭄)
		
			if(flag == 0) {
				heightWall[i] = FALSE;	//	두 방 사이의 벽을 허뭄
				maze[i + 1] = maze[i];
			}
		}
	} else {
		for(int i = 0; i < WIDTH - 1; i++)
			heightWall[i] = FALSE;
	}
}

void drawHeightWalls(int* heightWall, FILE* fp) {
	fprintf(fp, "|");	//	왼쪽 가장자리 벽
	for(int i = 0; i < WIDTH - 1; i++) {
		fprintf(fp, " ");	//	방을 그림
		if(heightWall[i] == TRUE) fprintf(fp, "|");	//	벽이 있을 경우 벽을 그림
		else fprintf(fp, " ");		//	허물어진 벽을 표현
	}				
	fprintf(fp, " |\n");	//	오른쪽 가장자리 벽
}

void drawWidthWalls(int* widthWall, FILE* fp) {
	fprintf(fp, "+");
	for(int i = 0; i < WIDTH; i++) {
		if(widthWall[i] == TRUE) fprintf(fp, "-+");	//	벽이 있을 경우 모서리와 벽을 그림
		else fprintf(fp, " +");							//	벽이 없을 경우 모서리만 그림
	}				
	fprintf(fp, "\n");
}

void decideInterRowWalls(int* maze, int* widthWall, int* room) {
	int previous = maze[0];
	int meet = FALSE;	//	윗 줄과 연결되면 TRUE(각 방마다)
//	resetWidthWall(widthWall);

	for(int i = 0; i < WIDTH; i++) {
		int flag = rand() % 2;					

		if(flag == 0) {
			widthWall[i] = FALSE;	//	두 방 사이의 벽 허뭄
			meet = TRUE;
		} else {
			widthWall[i] = TRUE;	//	두 방 사이의 벽을 세움
			meet = FALSE;
		}
		if(i < WIDTH - 1) {
			if(previous != maze[i + 1]) {	//	위층의 방이 다음 방과 연결되있지 않을 경우
				if(meet == FALSE)	{	//	심지어 아랫줄과도 연결되지 않을 경우 -> 폐쇄 공간 발생
					widthWall[i] = FALSE;	//	폐쇄 공간 방지
				}
				else meet = FALSE;
				previous = maze[i + 1];
			}
		}		
		if(i == WIDTH - 1 && meet == FALSE) {	//	마지막 방인데 윗줄과 연결된 방이 없을 경우
			widthWall[i] = FALSE;	//	강제로 벽을 헐음
		}
		if(widthWall[i] == TRUE) {	//	그러고도 벽이 남았다면
			maze[i] = (*room)++;
		}		
	}			
}

void decideMidRowWalls(int* maze, int* heightWall) {
	int meet = FALSE;
	int flag;
	int previous;

	for(int i = 0; i < WIDTH - 1; i++) {
		if(maze[i] == maze[i + 1]) {	//	두 방의 번호가 같다면 이미 연결되어 있음
			heightWall[i] = TRUE;	//	벽을 두어야 함					
		}	else {
			flag = rand() % 2;	//	벽을 허물지 결정
			if(flag == 0) {
				previous = maze[i + 1];			
				maze[i + 1] = maze[i];	
				for(int j = 0; j < WIDTH; j++) {
					if(maze[j] == previous) maze[j] = maze[i];	//	이전 방과 같은 방을 모두 바꿔줌
				}
				heightWall[i] = FALSE;
			} else heightWall[i] = TRUE;
		}				
	}				
}

void decideLastWalls(int* maze, int* heightWall) {
	int previous;

	resetHeightWall(heightWall);

	for(int i = 0; i < WIDTH - 1; i++) {
		if(maze[i] == maze[i + 1]) heightWall[i] = TRUE;
		else {
			heightWall[i] = FALSE;
			previous = maze[i + 1];
			maze[i + 1] = maze[i];
			for(int j = 0; j < WIDTH; j++) {
				if(maze[j] == previous) maze[j] = maze[i];				
			}
		}
	}
}

void drawMaze(int* maze, int* heightWall, int* widthWall, int* room, FILE* fp) {
	resetHeightWall(heightWall);
	resetWidthWall(widthWall);

	for(int i = 0; i < WIDTH; i++)
		maze[i] = (*room)++;		//	0번째 줄 방 서로 다른 값으로 초기화

	drawCeilingFloorOutline(fp);
	decideFirstRowWalls(maze, heightWall);

	drawHeightWalls(heightWall, fp);
	
	for(int i = 1; i < HEIGHT - 1; i++) {	//	1번째 줄부터 HEIGHT - 2번째 줄까지
		decideInterRowWalls(maze, widthWall, room);
		drawWidthWalls(widthWall, fp);	
		decideMidRowWalls(maze, heightWall);
		drawHeightWalls(heightWall, fp);
	}
	
	if(HEIGHT >= 2) {
		decideInterRowWalls(maze, widthWall, room);
		drawWidthWalls(widthWall, fp);
		decideLastWalls(maze, heightWall);
		drawHeightWalls(heightWall, fp);
	}
	drawCeilingFloorOutline(fp);
}

void makeImperfectMaze(char** mazeArray, FILE* fp) {
	int flag = 0;
	int walls = HEIGHT * WIDTH - HEIGHT - WIDTH + 1;	//	완전 미로에서의 벽의 개수
	int passed = 0;		//	지금까지 만난 벽의 개수(지운 개수 + 지우지 않은 개수)
	int deleted = 0;	//	지운 벽의 개수
	int toDelete = delNum;	//	앞으로 지워야할 벽의 개수


	if(deleted == delNum) return;	//	HEIGHT 또는 WIDTH가 1일 경우 벽을 지워서 불완전 미로를 만들 수 없음 

	for(int i = 1; i < 2 * HEIGHT; i++) {
		if(deleted == delNum) break;

		for(int j = 1; j < 2 * WIDTH; j++) {
			if((i % 2 == 1 && j % 2 == 0) ||(i % 2 == 0 && j % 2 == 1)) {	//	벽이 위치할 수 있는 좌표
				if(mazeArray[i][j] == ' ') continue;	//	벽이 아닐 경우 다음 벽을 탐색하러 감
				else {	//	벽일 경우
					if(walls - passed > toDelete) {	//	남은 벽의 개수가 지워야할 벽의 개수보다 많을 경우
						flag = rand() % 101; 
						
						if(flag % 10 == 0) { //	10% 정도의 확률로 벽을 지움
							mazeArray[i][j] = ' ';
							deleted++;
							toDelete--;
							passed++;
						} else 	passed++;	//	벽을 지우지 않음
					}
					else if(walls - passed == toDelete) {	//	남은 벽의 개수가 지워야할 벽의 개수와 같을 경우
						mazeArray[i][j] = ' ';	//	무조건 벽을 지워야 함
						deleted++;
						toDelete--;
						passed++;
					}
					else {
						fprintf(stderr, "something was wrong!\n");
						exit(-1);
					}
					if(deleted == delNum) break;
				}
			}
			else continue;	//	방 또는 '+'이므로 넘어감
		}					
	} 

	fp = fopen("maze.maz", "w");	//	maze.maz 파일에 불완전미로를 덮어 씀
	for(int i = 0; i < mazeHeight; i++) {
		for(int j = 0; j < mazeWidth; j++) {
			fprintf(fp, "%c", mazeArray[i][j]);
		}
	}
	fclose(fp);
}	
