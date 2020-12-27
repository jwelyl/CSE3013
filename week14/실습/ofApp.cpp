#include "ofApp.h"

Vertex** vertex = NULL;	//	Vertex ��� ���� �迭
Wall* wall = NULL;

Vertex* root = NULL;	//	Vertex Ʈ���� ���

Node* stackTop = NULL;	//	stack top
Node* stackDel = NULL;	//	���ÿ��� ���ŵ� ������ �ӽ� �����ϴ� ����Ʈ

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
		
		if (dfs_flag) {	//	dfs Ž�� �� �湮�� ��� ��θ� �����Ͽ� �׸�
			Node* cur = stackTop;
			while (cur) {
			//	printf("��� �׸���(�ʷϻ�)\n");
				int hidx, widx;
				returnIdx(cur->vertex_name, &hidx, &widx);
				ofSetColor(0, 255, 0);
				if (vertex[hidx][widx].parent->x == vertex[hidx][widx].x &&
					vertex[hidx][widx].parent->y == vertex[hidx][widx].y) {
					//	������ ��������� ǥ��
					ofSetColor(255, 0, 255);
					ofDrawCircle(vertex[hidx][widx].x, vertex[hidx][widx].y, 5);
				}
				else {
					ofDrawLine(vertex[hidx][widx].parent->x, vertex[hidx][widx].parent->y,
						vertex[hidx][widx].x, vertex[hidx][widx].y);
					if (vertex[hidx][widx].name == WIDTH * HEIGHT) {
						//	������ ��������� ǥ��
						ofSetColor(255, 0, 255);
						ofDrawCircle(vertex[hidx][widx].x, vertex[hidx][widx].y, 5);
					}
				}
				cur = cur->link;
			}
			cur = stackDel;
			while (cur) {
			//	printf("��� ������(������)\n");
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

	wall = (Wall*)malloc(sizeof(Wall) * maxWall);	//	���� �ִ� ����

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
			if (i % 2 == 1 && j % 2 == 1 && mazeArray[i][j] == ' ') {	//	���� ���
				int newI = (i - 1) / 2;
				int newJ = (j - 1) / 2;
				Vnum++;
				Vertex v = makeVertex(Vnum, j * r, i * r);

				if (j == 1) v.left = NULL;					//	���� ���� ���� ���	v.left = NULL;
				if (j == 2 * WIDTH - 1) v.right = NULL; 	//	���� ������ ���� ��� v.right = NULL;				
				if (i == 1) v.up = NULL;					//	���� ���� ���� ���	 v.up = NULL;
				if (i == 2 * HEIGHT - 1) v.down = NULL;		//	���� �Ʒ��� ���� ��� v.down = NULL;

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
			if (i % 2 == 1 && j % 2 == 0) {	//	���� �� '|'�� ���� �� �ִ� ����
				int prevX = (j - 2) / 2;	//	'|' �� ���� ���� �� ���� ���� �ε���
				int nextX = j / 2;			//	'|' �� ���� ������ �� ���� ���� �ε���
				int Y = (i - 1) / 2;
				if (mazeArray[i][j] == '|') {	//	���� ���� ���
					vertex[Y][prevX].right = NULL;
					vertex[Y][nextX].left = NULL;
				}
				else if (mazeArray[i][j] == ' ') {	//	���� ���� ���
					vertex[Y][prevX].right = &vertex[Y][nextX];		//	�ش� ��ġ ���� �¿� ��� ���� ����
					vertex[Y][nextX].left = &vertex[Y][prevX];
				}
			}
			else if (i % 2 == 0 && j % 2 == 1) {
				int X = (j - 1) / 2;
				int prevY = (i - 2) / 2;	//	'-' �� ���� ���� �� ���� ���� �ε���
				int nextY = i / 2;			//	'-' �� ���� �Ʒ��� �� ���� ���� �ε���

				if (mazeArray[i][j] == '-') {	//	���� ���� ���		
					vertex[prevY][X].down = NULL;
					vertex[nextY][X].up = NULL;
				}
				else if (mazeArray[i][j] == ' ') {	//	���� ���� ���
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

	//	���� ���� 0���� reset
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
	v.visited = false;	//	���� �湮���� �ʾ���
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
	root->parent = root;	//	������ ������ �湮�� �� �����Ƿ� �������� �����ϰ� ����
	root->visited = true;	//	�������� �湮
	stackPush(root->name);	//	���ÿ� ����

	while (!stackEmpty()) {
		int u = stackPeek();		
		if (u == target) return;	//	������ ���� ���� ���� ���� �������� ��� ����
		
		int hidx, widx;
		returnIdx(u, &hidx, &widx);
		if (vertex[hidx][widx].down && !vertex[hidx][widx].down->visited) {	//	1����. �Ʒ��� �湮
			vertex[hidx][widx].down->visited = true;	//	�Ʒ����� �湮��
			vertex[hidx][widx].down->parent = &vertex[hidx][widx];	//	���� ���� �Ʒ����� �θ�� ����
			stackPush(vertex[hidx][widx].down->name);	//	���ÿ� ����
		}
		else if (vertex[hidx][widx].right && !vertex[hidx][widx].right->visited) {	//	2����. ������ ��(�Ʒ����� �湮�� �� ���� ���)
			vertex[hidx][widx].right->visited = true;
			vertex[hidx][widx].right->parent = &vertex[hidx][widx];
			stackPush(vertex[hidx][widx].right->name);
		}
		else if (vertex[hidx][widx].left && !vertex[hidx][widx].left->visited) {	//	3����. ���� ��(�Ʒ���, ������ ���� �湮�� �� ���� ���)
			vertex[hidx][widx].left->visited = true;
			vertex[hidx][widx].left->parent = &vertex[hidx][widx];
			stackPush(vertex[hidx][widx].left->name);
		}
		else if (vertex[hidx][widx].up && !vertex[hidx][widx].up->visited) {	//	4����. ����(�Ʒ���, ������ ��, ���� ���� �湮�� �� ���� ���)
			vertex[hidx][widx].up->visited = true;
			vertex[hidx][widx].up->parent = &vertex[hidx][widx];
			stackPush(vertex[hidx][widx].up->name);
		}
		else stackPop();	//	�湮�� �� �ִ� ���� ���� ��� �ش� ���� ���ÿ��� ������
	}
}