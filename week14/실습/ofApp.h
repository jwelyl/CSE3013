#pragma once

#ifndef __OF_APP_H__
#define __OF_APP_H__
#include "ofMain.h"

typedef struct _Node {
	int vertex_name;
	_Node* link;
} Node;

bool stackEmpty();
void stackPush(int name);
int stackPeek();
int stackPop();

void insertList(Node** list, Node* node);
void deleteList(Node** list);

typedef struct _Vertex {
	int name;
	float x, y;	//	방의 중심의 x 좌표, y 좌표
	bool visited;
	_Vertex* up, * down, * left, * right;
	_Vertex* parent;
} Vertex;

Vertex makeVertex(int roomNum, int i, int j);

typedef struct _Wall {
	float x1, y1;	//	벽을 나타내는 선분의 시작점 좌표
	float x2, y2;	//	벽을 나타내는 선분의 끝점 좌표
} Wall;

Wall makeWall(float x1, float y1, float x2, float y2);

class ofApp : public ofBaseApp {

public:
	bool start_flag;
	bool draw_flag;
	bool dfs_flag;

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	bool readFile(const char* filename);
	void makeVertexTree();
	void freeMemory();
};

void returnIdx(int name, int* hidx, int* widx);
void dfs(Vertex* root, int target);

#endif