#pragma once
#include "stdafx.h"
#include "Shader.h"
//static int map[map_size_n][map_size_m];

const int dir = 4; // number of possible directions to go at any position
// if dir==4
static int dx[dir]={1, 0, -1, 0};
static int dy[dir]={0, 1, 0, -1};
 // if dir==8
//static int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };
//static int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };

struct node_pos {
	int x;
	int y;
};

struct node_pos_float {
	float x;
	float y;
};

class path_node
{
	// current position
	int xPos;
	int yPos;
	// total distance already travelled to reach the node
	int level;
	// priority=level+remaining distance estimate
	int priority;  // smaller: higher priority

public:
	path_node(int xp, int yp, int d, int p);

	int getxPos() const { return xPos; }
	int getyPos() const { return yPos; }
	int getLevel() const { return level; }
	int getPriority() const { return priority; }

	void updatePriority(const int & xDest, const int & yDest)
	{
		priority = level + estimate(xDest, yDest) * 10; //A*
	}

	// give better priority to going strait instead of diagonally
	void nextLevel(const int & i) // i: direction
	{
		level += (dir == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
	}

	// Estimation function for the remaining distance to the goal.
	const int & estimate(const int & xDest, const int & yDest) const
	{
		static int xd, yd, d;
		xd = xDest - xPos;
		yd = yDest - yPos;

		// Euclidian Distance
		d = static_cast<int>(sqrt(xd*xd + yd*yd));


		return(d);
	}
};

// Determine priority (in the priority queue)
bool operator<(const path_node & a, const path_node & b);

//A* 알고리즘
// std:;string 형태의 디렉션 인덱스로 반환
std::string pathFind(const int & xStart, const int & yStart, const int & xFinish, const int & yFinish, int map[map_size_n][map_size_m]);


//현재 x,y  한변의 노드 개수, 미니멈 x,z 맥시멈 x,z
node_pos PositionToNodeIndex(float fx, float fy, float dm, D3DXVECTOR2 minMap, D3DXVECTOR2 maxMap);

node_pos_float NodeIndexToPosition(node_pos pos, float dm, D3DXVECTOR2 minMap, D3DXVECTOR2 maxMap);


std::vector<node_pos> PathStringToNodeIndex(std::string path, node_pos pos);

// 노드맵을 만들어줌 노드맵과 바운딩박스의 충돌여부가 마지막 bool값이면 벽을 만들어줌
void CreateNodeMap(int map[map_size_n][map_size_m], D3DXVECTOR2 minMap, D3DXVECTOR2 maxMap, CDiffusedShader* pShader, int start_Obj, int cnt_Obj, bool dis);