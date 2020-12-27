#include "mazeGen.h"

int main(void) {
	int* maze = NULL;
	int* heightWall = NULL;	//	세로 벽 "|" 의 배열 (true면 벽 존재, false면 존재 x)
	int* widthWall = NULL;		//	가로 벽 "-" 의 배열 ('')
//	char fileName[20]; 
	FILE* fp;
	
	int room = 1;
	
	srand(time(NULL));

	scanf("%d %d", &WIDTH, &HEIGHT);
//	sprintf(fileName, "maze%dby%d.maz", WIDTH, HEIGHT);
	fp = fopen("maze.maz", "w");

	maze = (int*)malloc(sizeof(int) * WIDTH);
	heightWall = (int*)malloc(sizeof(int) * WIDTH - 1);
	widthWall = (int*)malloc(sizeof(int) * WIDTH);
	
	drawMaze(maze, heightWall, widthWall, &room, fp);
	
	fclose(fp);
	free(maze);
	free(heightWall);
	free(widthWall);
	
	return 0;
}

