#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	createRankList();
	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	deleteList(&head);
	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();

//	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	DrawBox(10, WIDTH+10,4,8);


	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(16,WIDTH+10);
	printw("SCORE");
	DrawBox(17,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(18,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
		move(11+i, WIDTH+13);
		for(j = 0; j < 4; j++) {
			if(block[nextBlock[2]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
			//
				move(i+y+1,j+x+1);
			//	
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	int i, j;

	for(i = blockY; i < blockY + 4; i++) {
		for(j = blockX; j < blockX + 4; j++) {
			if(block[currentBlock][blockRotate][i - blockY][j - blockX] == 1) {
				if(i < 0 || i >= HEIGHT || j < 0 || j >= WIDTH) return 0;
				else if(f[i][j] == 1) return 0;				
			}				
		}			
	}

	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int i, j;
	int prevY = blockY, prevX = blockX, prevR = blockRotate;	//	이전 블록의 위치 정보
	int shadowY = prevY;

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다.
	
	//	이전 블록의 위치 정보를 찾기
	switch(command) {
	case KEY_UP:
		prevR = (blockRotate + 3) % 4;
		break;
	case KEY_DOWN:
		prevY--;
		break;
	case KEY_RIGHT:
		prevX--;
		break;
	case KEY_LEFT:
		prevX++;
		break;
	default:
		break;
	}

	while(CheckToMove(f, currentBlock, prevR, shadowY + 1, prevX)) {
		shadowY++;				
	}

	//	이전 블록의 정보를 지우기.
	for(i = prevY; i < prevY + 4; i++) {
		for(j = prevX; j < prevX + 4; j++) {
			if(block[currentBlock][prevR][i - prevY][j - prevX] == 1 && i >= 0) {
				move(i + 1, j + 1);
				printw("%c", '.');
			}
		}
	}
	//	이전의 그림자 정보를 지우기.
	for(i = shadowY; i < shadowY + 4; i++) {
		for(j = prevX; j < prevX + 4; j++) {
			if(block[currentBlock][prevR][i - shadowY][j - prevX] == 1 && i >= 0) {
				move(i + 1, j + 1);
				printw("%c", '.');
			}			
		}
	}

	//	새로운 블록 정보 그리기
//	DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig) {
	if(CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);				
	}	else {
		if(blockY == -1) gameOver = 1;
		
	  score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand() % 7;
		DrawNextBlock(nextBlock);
		
		blockY = -1;
		blockX = WIDTH / 2 - 2;
		blockRotate = 0;
		PrintScore(score);	
		DrawField();			
	}
	timed_out = 0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int i, j;
	int touched = 0;
	int Score = 0;

	//Block이 추가된 영역의 필드값을 바꾼다.
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {				
			if(block[currentBlock][blockRotate][i][j] == 1) {
				if(blockY + i + 1 == HEIGHT) touched++;
				else if(f[blockY + i + 1][blockX + j] == 1) touched++;
				f[blockY + i][blockX + j] = 1;
			}
		}
	}
	Score += (touched * 10);

	DrawField();

	return Score;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
	int i, j, k;
	int line = 0;

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	for(i = 0; i < HEIGHT; i++) {
		for(j = 0; j < WIDTH; j++) 
			if(f[i][j] == 0) break;			
 		if(j < WIDTH) continue;
		line++;
		
		for(j = i - 1; j >= -1; j--) {
			for(k = 0; k < WIDTH; k++) {
				if(j == -1) f[j + 1][k] = 0;
				else f[j + 1][k] = f[j][k]; 				
			}
		}				
	}

	return line * line * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	int shadowY = y;
	while(CheckToMove(field, blockID, blockRotate, shadowY + 1, x)) {
		shadowY++;				
	}
	DrawBlock(shadowY, x, blockID, blockRotate, '/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawShadow(y, x, blockID, blockRotate);
	DrawBlock(y, x, blockID, blockRotate, ' ');		
}

///////////////////////////////////////////////////////////////////////////

RankNode* makeNode(char* _name, int _score) {
	RankNode* node = (RankNode*)malloc(sizeof(RankNode));
	strcpy(node->name, _name);
	node->rankScore = _score;
	node->prev = NULL;
	node->next = NULL;
	
	return node;				
}

void insertNode(RankNode** head, RankNode* newNode) {
	RankNode* cur;
	
	if(!(*head)) {
		*head = newNode;	//	빈 리스트일 경우
	} else if((*head)->rankScore < newNode->rankScore) {
		//	최고점일 경우
		(*head)->prev = newNode;
		newNode->next = *head;
		*head = newNode;
	} else {
		cur = *head;
		for(; ; cur = cur->next) {
			if(!cur->next) {
				cur->next = newNode;
				newNode->prev = cur;
				break;				
			} else if(cur->next->rankScore < newNode->rankScore) {
				newNode->prev = cur;
				newNode->next = cur->next;
				cur->next->prev = newNode;
				cur->next = newNode;
				break;				
			}
		}	
	}
	
	nNode++;
}

void deleteList(RankNode** head) {	//	연결 리스트의 노드를 전부 삭제
	RankNode* cur = *head;
	RankNode* del;
	if(!cur) return;	//	이미 빈 리스트

	for(; cur; ) {
		del = cur;
		cur = cur->next;
		if(cur) cur->prev = NULL;
		if(del->next) del->next = NULL;				
		free(del);
	}				

	*head = NULL;
}
 
void createRankList(){
	// user code
	int num, i;
	char _name[NAMELEN];
	int _score;
	FILE* fp = fopen("rank.txt", "r");
	
	if(!fp) return ;	//	rank.txt 파일이 존재하지 않을 경우
	
	else if(fscanf(fp, "%d", &num) == EOF) {	//	rank.txt 파일이 빈 파일일 경우
		fclose(fp);
		return ;	
	} 
 
	for(i = 0; i < num; i++) {
		fscanf(fp, "%s %d", _name, &_score);
		RankNode* node = makeNode(_name, _score);
		insertNode(&head, node);
	}

	fclose(fp);
}

void printNode(RankNode* head) {
	int X = -1, Y = -1, count;
	RankNode* cur = head;

	printw("x: ");
	echo();
	scanw("%d", &X);
	printw("y: ");
	scanw("%d", &Y);
	noecho();

	printw("       name       |   score   \n");
	printw("------------------------------\n");
	if(X <= 0 && Y <= 0) {	//	X, Y 둘다 입력받지 않은 경우
		X = 1;
		Y = nNode;				
	} else if(X <= 0) {	//	X만 입력받지 않은 경우
		X = 1;				
	} else if(Y <= 0) {	//	Y만 입력받지 않은 경우
		Y = nNode;				
	} else if(X > Y || X > nNode) {	//	X가 Y보다 큰 경우 -> 예외처리
		printw("\nsearch failure: no rank in the list\n");
		getch();
		return ;
	}

//	printw("X : %d, Y : %d\n", X, Y);

	count = 0;

	for(; cur; cur = cur->next) {
		count++;
		if(X <= count && count <= Y) {
			printw(" %-17s| %-12d\n", cur->name, cur->rankScore);
		}				
	}
}

char rankMenu() {
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	return wgetch(stdscr);
}

void rank(){
	clear();
	switch(rankMenu()) {
	case '1':
		printNode(head);
		break;
	case '2':
		printw("implemented in week 8 assignment\n");
		break;
	case '3':
		printw("implemented in week 8 assignment\n");
		break;
	default:
		printw("invalid input\n");
		break;
	}
	getch();
	return ;
}

void writeRankFile(){
	// user code
	RankNode* cur = head;
	FILE* fp = fopen("rank.txt", "w");
	fprintf(fp, "%d\n", nNode);

	for(; cur; cur = cur->next) {
		fprintf(fp, "%s %d\n", cur->name, cur->rankScore); 				
	}

	fclose(fp);
}

void newRank(int score){
	// user code
	char name[NAMELEN];
	clear();

	echo();
	printw("your name: ");
	wgetnstr(stdscr, name, NAMELEN);
	noecho();

	RankNode* node = makeNode(name, score);
	insertNode(&head, node);

	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
