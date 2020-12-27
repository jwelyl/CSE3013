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

int WIDTH, HEIGHT;	//	미로의 너비, 높이
