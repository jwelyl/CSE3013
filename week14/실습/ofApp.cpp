#include "ofApp.h"

Vertex** vertex = NULL;	//	Vertex 노드 저장 배열
Wall* wall = NULL;

Vertex* root = NULL;	//	Vertex 트리의 노드

Node* stackTop = NULL;	//	stack top
Node* stackDel = NULL;	//	스택에서 제거된 노드들을 임시 저장하는 리스트

char** mazeArray = NULL;
int WIDTH, HEIGHT;
int MAZE_WIDTH = 0, MAZE_HEIGHT = 0;
int Vnum = 0;
int Wnum = 0;
int r = 20;

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("Maze Example");
	ofSetBackgroundColor(255, 255, 255);
	ofSetLineWidth(5);
	ofSetColor(0, 255, 0);

	draw_flag = false;
	start_flag = false;
	dfs_flag = false;
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	if (draw_flag) {
		for (int i = 0; i < Wnum; i++) {
			ofSetColor(85, 129, 191);
			ofDrawLine(wall[i].x1, wall[i].y1, wall[i].x2, wall[i].y2);
		}
		
		if (dfs_flag) {	//	dfs 탐색 후 방문한 길과 경로를 구분하여 그림
			Node* cur = stackTop;
			while (cur) {
			//	printf("경로 그리기(초록색)\n");
				int hidx, widx;
				returnIdx(cur->vertex_name, &hidx, &widx);
				ofSetColor(0, 255, 0);
				if (vertex[hidx][widx].parent->x == vertex[hidx][widx].x &&
					vertex[hidx][widx].parent->y == vertex[hidx][widx].y) {
					//	시작점 보라색으로 표시
					ofSetColor(255, 0, 255);
					ofDrawCircle(vertex[hidx][widx].x, vertex[hidx][widx].y, 5);
				}
				else {
					ofDrawLine(vertex[hidx][widx].parent->x, vertex[hidx][widx].parent->y,
						vertex[hidx][widx].x, vertex[hidx][widx].y);
					if (vertex[hidx][widx].name == WIDTH * HEIGHT) {
						//	도착점 보라색으로 표시
						ofSetColor(255, 0, 255);
						ofDrawCircle(vertex[hidx][widx].x, vertex[hidx][widx].y, 5);
					}
				}
				cur = cur->link;
			}
			cur = stackDel;
			while (cur) {
			//	printf("경로 미포함(빨간색)\n");
				int hidx, widx;
				returnIdx(cur->vertex_name, &hidx, &widx);
				ofSetColor(255, 0, 0);
				ofDrawLine(vertex[hidx][widx].parent->x, vertex[hidx][widx].parent->y,
					vertex[hidx][widx].x, vertex[hidx][widx].y);
				cur = cur->link;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'l' || key == 'L') {
		cout << "'L' key pressed" << endl;
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
		if (openFileResult.bSuccess) {
			string file_location = openFileResult.getPath();
			const char* file = file_location.c_str();
			readFile(file);
		}
		else {
			OF_EXIT_APP(-1);
		}
		//	draw_flag = true;
	}
	if (key == 'q' || key == 'Q') {
		cout << "Quit" << endl;
		freeMemory();
		OF_EXIT_APP(0);
	}
	if (draw_flag) {
		if (key == 'd' || key == 'D') {	//	DFS key
			cout << "Depth-First-Search!" << endl;

			dfs(root, WIDTH * HEIGHT);
			dfs_flag = true;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

bool ofApp::readFile(const char* filename) {
	freeMemory();

	int length = 0;
	FILE* fp = fopen(filename, "r");
	char ch;
	bool flag = false;

	if (!fp) {
		cout << "Failed to open file" << endl;
		return false;
	}

	while ((ch = fgetc(fp)) != EOF) {
		if (ch != '\n' && !flag)	MAZE_WIDTH++;
		if (ch == '\n') flag = true;
		length++;
	}
	MAZE_WIDTH++;
	MAZE_HEIGHT = length / MAZE_WIDTH;
	WIDTH = MAZE_WIDTH / 2 - 1;
	HEIGHT = (MAZE_HEIGHT - 1) / 2;

	fclose(fp);

	int maxWall = WIDTH * HEIGHT + WIDTH + HEIGHT + 1;
	int num = 0;

	vertex = (Vertex**)malloc(sizeof(Vertex*) * HEIGHT);
	for (int i = 0; i < HEIGHT; i++)
		vertex[i] = (Vertex*)malloc(sizeof(Vertex) * WIDTH);

	wall = (Wall*)malloc(sizeof(Wall) * maxWall);	//	벽의 최대 개수

	mazeArray = (char**)malloc(sizeof(char*) * MAZE_HEIGHT);
	for (int i = 0; i < MAZE_HEIGHT; i++)
		mazeArray[i] = (char*)malloc(sizeof(char) * MAZE_WIDTH);

	fp = fopen(filename, "r");
	if (!fp) {
		cout << "Failed to open file" << endl;
		return false;
	}

	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_WIDTH; j++) {
			fscanf(fp, "%c", &mazeArray[i][j]);
			if (i % 2 == 1 && j % 2 == 1 && mazeArray[i][j] == ' ') {	//	방일 경우
				int newI = (i - 1) / 2;
				int newJ = (j - 1) / 2;
				Vnum++;
				Vertex v = makeVertex(Vnum, j * r, i * r);

				if (j == 1) v.left = NULL;					//	가장 왼쪽 방의 경우	v.left = NULL;
				if (j == 2 * WIDTH - 1) v.right = NULL; 	//	가장 오른쪽 방의 경우 v.right = NULL;				
				if (i == 1) v.up = NULL;					//	가장 위쪽 방의 경우	 v.up = NULL;
				if (i == 2 * HEIGHT - 1) v.down = NULL;		//	가장 아래쪽 방의 경우 v.down = NULL;

				vertex[newI][newJ] = v;
			}

			if (i % 2 == 0 && j % 2 == 1 && mazeArray[i][j] == '-')
				wall[Wnum++] = makeWall(r * (j - 1), r * i, r * (j + 1), r * i);
			if (i % 2 == 1 && j % 2 == 0 && mazeArray[i][j] == '|')
				wall[Wnum++] = makeWall(r * j, r * (i - 1), r * j, r * (i + 1));
		}
	}
	makeVertexTree();

	draw_flag = true;
	fclose(fp);

	return true;
}


void ofApp::makeVertexTree() {
	for (int i = 1; i < 2 * HEIGHT; i++) {
		for (int j = 1; j < 2 * WIDTH; j++) {
			if (i % 2 == 1 && j % 2 == 0) {	//	세로 벽 '|'이 있을 수 있는 공간
				int prevX = (j - 2) / 2;	//	'|' 벽 기준 왼쪽 방 정점 저장 인덱스
				int nextX = j / 2;			//	'|' 벽 기준 오른쪽 방 정점 저장 인덱스
				int Y = (i - 1) / 2;
				if (mazeArray[i][j] == '|') {	//	벽이 있을 경우
					vertex[Y][prevX].right = NULL;
					vertex[Y][nextX].left = NULL;
				}
				else if (mazeArray[i][j] == ' ') {	//	벽이 없을 경우
					vertex[Y][prevX].right = &vertex[Y][nextX];		//	해당 위치 기준 좌우 노드 서로 연결
					vertex[Y][nextX].left = &vertex[Y][prevX];
				}
			}
			else if (i % 2 == 0 && j % 2 == 1) {
				int X = (j - 1) / 2;
				int prevY = (i - 2) / 2;	//	'-' 벽 기준 위쪽 방 정점 저장 인덱스
				int nextY = i / 2;			//	'-' 벽 기준 아래쪽 방 정점 저장 인덱스

				if (mazeArray[i][j] == '-') {	//	벽이 있을 경우		
					vertex[prevY][X].down = NULL;
					vertex[nextY][X].up = NULL;
				}
				else if (mazeArray[i][j] == ' ') {	//	벽이 없을 경우
					vertex[prevY][X].down = &vertex[nextY][X];
					vertex[nextY][X].up = &vertex[prevY][X];
				}
			}
		}
	}

	root = &vertex[0][0];
}

void ofApp::freeMemory() {
	if (!start_flag) {
		start_flag = true;
		return;
	}
	draw_flag = false;

	if (vertex) {
		for (int i = 0; i < HEIGHT; i++) {
			if (vertex[i]) {
				free(vertex[i]);
				vertex[i] = NULL;
			}
		}
		free(vertex);
		vertex = NULL;
	}

	if (wall) {
		free(wall);
		wall = NULL;
	}

	if (mazeArray) {
		for (int i = 0; i < MAZE_HEIGHT; i++) {
			if (mazeArray[i]) {
				free(mazeArray[i]);
				mazeArray[i] = NULL;
			}
		}
		free(mazeArray);
		mazeArray = NULL;
	}

	if (dfs_flag) dfs_flag = false;
	deleteList(&stackDel);
	deleteList(&stackTop);

	//	전역 변수 0으로 reset
	MAZE_HEIGHT = 0;
	MAZE_WIDTH = 0;
	Vnum = 0;
	Wnum = 0;
	root = NULL;
}

Vertex makeVertex(int roomNum, int i, int j) {
	Vertex v;
	v.name = roomNum;
	v.x = i;	v.y = j;
	v.visited = false;	//	아직 방문하지 않았음
	v.up = NULL;	v.down = NULL;	v.left = NULL;	v.right = NULL;

	return v;
}

Wall makeWall(float x1, float y1, float x2, float y2) {
	Wall w;
	w.x1 = x1;	w.y1 = y1;	w.x2 = x2; w.y2 = y2;
	return w;
}

void returnIdx(int name, int* hidx, int* widx) {
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			if (name == WIDTH * h + w + 1) {
				*hidx = h;
				*widx = w;
				return;
			}
		}
	}
}

bool stackEmpty() {
	return !stackTop;
}

void stackPush(int name) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->vertex_name = name;
	node->link = stackTop;
	stackTop = node;
}

int stackPeek() {
	if (!stackTop) return -1;
	return stackTop->vertex_name;
}

int stackPop() {
	if (!stackTop) return -1;
	Node* delNode = stackTop;
	int ret = delNode->vertex_name;
	stackTop = delNode->link;
	delNode->link = NULL;
	insertList(&stackDel, delNode);
}

void insertList(Node** list, Node* node) {
	if (*list) node->link = *list;
	*list = node;
}

void deleteList(Node** list) {
	while (*list) {
		Node* del = *list;
		*list = del->link;
		if(del->link) del->link = NULL;
		free(del);
	}
}

void dfs(Vertex* root, int target) {
	root->parent = root;	//	시작점 이전에 방문한 방 없으므로 시작점과 동일하게 설정
	root->visited = true;	//	시작점을 방문
	stackPush(root->name);	//	스택에 삽입

	while (!stackEmpty()) {
		int u = stackPeek();		
		if (u == target) return;	//	스택의 가장 위에 쌓인 방이 도착점일 경우 종료
		
		int hidx, widx;
		returnIdx(u, &hidx, &widx);
		if (vertex[hidx][widx].down && !vertex[hidx][widx].down->visited) {	//	1순위. 아랫방 방문
			vertex[hidx][widx].down->visited = true;	//	아랫방을 방문함
			vertex[hidx][widx].down->parent = &vertex[hidx][widx];	//	이전 방을 아랫방의 부모로 설정
			stackPush(vertex[hidx][widx].down->name);	//	스택에 삽입
		}
		else if (vertex[hidx][widx].right && !vertex[hidx][widx].right->visited) {	//	2순위. 오른쪽 방(아랫방을 방문할 수 없을 경우)
			vertex[hidx][widx].right->visited = true;
			vertex[hidx][widx].right->parent = &vertex[hidx][widx];
			stackPush(vertex[hidx][widx].right->name);
		}
		else if (vertex[hidx][widx].left && !vertex[hidx][widx].left->visited) {	//	3순위. 왼쪽 방(아랫방, 오른쪽 방을 방문할 수 없을 경우)
			vertex[hidx][widx].left->visited = true;
			vertex[hidx][widx].left->parent = &vertex[hidx][widx];
			stackPush(vertex[hidx][widx].left->name);
		}
		else if (vertex[hidx][widx].up && !vertex[hidx][widx].up->visited) {	//	4순위. 윗방(아랫방, 오른쪽 방, 왼쪽 방을 방문할 수 없을 경우)
			vertex[hidx][widx].up->visited = true;
			vertex[hidx][widx].up->parent = &vertex[hidx][widx];
			stackPush(vertex[hidx][widx].up->name);
		}
		else stackPop();	//	방문할 수 있는 방이 없을 경우 해당 방을 스택에서 제거함
	}
}