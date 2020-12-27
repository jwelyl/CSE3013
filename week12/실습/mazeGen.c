#include "mazeGen.h"

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
