#pragma once
#include "stdafx.h"

const int map_size_n = 20; // horizontal size of the map
const int map_size_m = 20; // vertical size size of the map
static int map[map_size_n][map_size_m];
static int closed_nodes_map[map_size_n][map_size_m]; // map of closed (tried-out) nodes
static int open_nodes_map[map_size_n][map_size_m]; // map of open (not-yet-tried) nodes
static int dir_map[map_size_n][map_size_m]; // map of directions
const int dir = 8; // number of possible directions to go at any position
// if dir==4
//static int dx[dir]={1, 0, -1, 0};
//static int dy[dir]={0, 1, 0, -1};
 // if dir==8
static int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };
static int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };

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

//A* �˰���
// std:;string ������ �𷺼� �ε����� ��ȯ
std::string pathFind(const int & xStart, const int & yStart, const int & xFinish, const int & yFinish);


//���� x,y  �Ѻ��� ��� ����, �̴ϸ� x,z �ƽø� x,z
node_pos PositionToNodeIndex(float fx, float fy, float dm, D3DXVECTOR2 minMap, D3DXVECTOR2 maxMap);

node_pos_float NodeIndexToPosition(node_pos pos, float dm, D3DXVECTOR2 minMap, D3DXVECTOR2 maxMap);


std::vector<node_pos> PathStringToNodeIndex(std::string path, node_pos pos);

