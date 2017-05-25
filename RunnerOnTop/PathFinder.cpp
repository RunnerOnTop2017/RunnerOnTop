#include "stdafx.h"
#include "PathFinder.h"


path_node::path_node(int xp, int yp, int d, int p)
{
	xPos = xp; yPos = yp; level = d; priority = p;
}

std::string pathFind(const int & xStart, const int & yStart, const int & xFinish, const int & yFinish)
{
	static std::priority_queue<path_node> pq[2]; // list of open (not-yet-tried) nodes
	static int pqi; // pq index
	static path_node* n0;
	static path_node* m0;
	static int i, j, x, y, xdx, ydy;
	static char c;
	pqi = 0;

	// reset the node maps
	for (y = 0; y<map_size_m; y++)
	{
		for (x = 0; x<map_size_n; x++)
		{
			closed_nodes_map[x][y] = 0;
			open_nodes_map[x][y] = 0;
			if (map[x][y] == 1)
			{
				closed_nodes_map[x][y] = 1;

			}
			
		}
	}

	// create the start node and push into list of open nodes
	n0 = new path_node(xStart, yStart, 0, 0);
	n0->updatePriority(xFinish, yFinish);
	pq[pqi].push(*n0);
	open_nodes_map[x][y] = n0->getPriority(); // mark it on the open nodes map
	delete n0; //added by someone
			   // A* search
	while (!pq[pqi].empty())
	{
		// get the current node w/ the highest priority
		// from the list of open nodes
		n0 = new path_node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
			pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

		x = n0->getxPos(); y = n0->getyPos();

		pq[pqi].pop(); // remove the node from the open list
		open_nodes_map[x][y] = 0;
		// mark it on the closed nodes map
		closed_nodes_map[x][y] = 1;

		// quit searching when the goal state is reached
		//if((*n0).estimate(xFinish, yFinish) == 0)
		if (x == xFinish && y == yFinish)
		{
			// generate the path from finish to start
			// by following the directions
			std::string path = "";
			while (!(x == xStart && y == yStart))
			{
				j = dir_map[x][y];
				c = '0' + (j + dir / 2) % dir;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			// garbage collection
			delete n0;
			// empty the leftover nodes
			while (!pq[pqi].empty()) pq[pqi].pop();
			return path;
		}

		// generate moves (child nodes) in all possible directions
		for (i = 0; i<dir; i++)
		{
			xdx = x + dx[i]; ydy = y + dy[i];

			if (!(xdx<0 || xdx>map_size_n - 1 || ydy<0 || ydy>map_size_m - 1 || map[xdx][ydy] == 1
				|| closed_nodes_map[xdx][ydy] == 1))
			{
				// generate a child node
				m0 = new path_node(xdx, ydy, n0->getLevel(),
					n0->getPriority());
				m0->nextLevel(i);
				m0->updatePriority(xFinish, yFinish);

				// if it is not in the open list then add into that
				if (open_nodes_map[xdx][ydy] == 0)
				{
					open_nodes_map[xdx][ydy] = m0->getPriority();
					pq[pqi].push(*m0);
					delete m0; // Only <-- new added by commenter // mark its parent node direction
					dir_map[xdx][ydy] = (i + dir / 2) % dir;
				}
				else if (open_nodes_map[xdx][ydy]>m0->getPriority())
				{
					// update the priority info
					open_nodes_map[xdx][ydy] = m0->getPriority();
					// update the parent direction info
					dir_map[xdx][ydy] = (i + dir / 2) % dir;

					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(pq[pqi].top().getxPos() == xdx &&
						pq[pqi].top().getyPos() == ydy))
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pq[pqi].pop(); // remove the wanted node

								   // empty the larger size pq to the smaller one
					if (pq[pqi].size()>pq[1 - pqi].size()) pqi = 1 - pqi;
					while (!pq[pqi].empty())
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pqi = 1 - pqi;
					pq[pqi].push(*m0); delete m0; // only 2nd item added new // add the better node instead
				}
				else delete m0; // garbage collection
			}
		}
		delete n0; // garbage collection
	}
	return ""; // no route found
}
node_pos PositionToNodeIndex(float fx, float fy, float dm, D3DXVECTOR2 minMap, D3DXVECTOR2 maxMap)
{
	node_pos ret;

	float distX = abs(maxMap.x - minMap.x);
	float distZ = abs(maxMap.y - minMap.y);

	float m_dx = (distX / dm);
	float m_dz = (distZ / dm);

	ret.x = (int)((fx + abs(minMap.x)) / m_dx);
	ret.y = (int)((fy + abs(minMap.y)) / m_dz);

	return ret;
}

node_pos_float NodeIndexToPosition(node_pos pos, float dm, D3DXVECTOR2 minMap, D3DXVECTOR2 maxMap)
{
	node_pos_float ret;


	float distX = maxMap.x - minMap.x;
	float distZ = maxMap.y - minMap.y;

	float m_dx = (int)(distX / dm);
	float m_dz = (int)(distZ / dm);


	ret.x = (m_dx * pos.x) + (m_dx * 0.5f) - abs(minMap.x);
	ret.y = (m_dz * pos.y) + (m_dz * 0.5f) - abs(minMap.y);

	return ret;
}

std::vector<node_pos> PathStringToNodeIndex(std::string path, node_pos pos)
{
	node_pos now = pos;
	std::vector<node_pos> ret;
	int end = path.length();
	for (int i = 0; i < path.length(); ++i)
	{
		char c = path.at(i);
		int n = c - '0';//atoi(&c);
		now.x += dx[n];
		now.y += dy[n];

		ret.push_back(now);
	}

	return ret;
}

bool operator<(const path_node & a, const path_node & b)
{
	return a.getPriority() > b.getPriority();
}