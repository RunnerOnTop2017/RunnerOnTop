#include "stdafx.h"
#include "PathFinder.h"
#include "xnaCollision.h"

path_node::path_node(int xp, int yp, int d, int p)
{
	xPos = xp; yPos = yp; level = d; priority = p;
}

std::string pathFind(const int & xStart, const int & yStart, const int & xFinish, const int & yFinish, int map[map_size_n][map_size_m])
{

	static std::priority_queue<path_node> pq[2]; // list of open (not-yet-tried) nodes
	static int closed_nodes_map[map_size_n][map_size_m]; // map of closed (tried-out) nodes
	static int open_nodes_map[map_size_n][map_size_m]; // map of open (not-yet-tried) nodes
	static int dir_map[map_size_n][map_size_m]; // map of directions

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


std::string detailpathFind(const int & xStart, const int & yStart, const int & xFinish, const int & yFinish, int map[map_size_n][map_size_m])
{

	static std::priority_queue<path_node> pq[2]; // list of open (not-yet-tried) nodes
	static int closed_nodes_map[map_size_n][map_size_m]; // map of closed (tried-out) nodes
	static int open_nodes_map[map_size_n][map_size_m]; // map of open (not-yet-tried) nodes
	static int dir_map[map_size_n][map_size_m]; // map of directions

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

	ret.x = (int)((fx - minMap.x) / m_dx);
	ret.y = (int)((fy - minMap.y) / m_dz);

	return ret;
}



node_pos_float NodeIndexToPosition(node_pos pos, float dm, D3DXVECTOR2 minMap, D3DXVECTOR2 maxMap)
{
	node_pos_float ret;


	float distX = abs(maxMap.x - minMap.x);
	float distZ = abs(maxMap.y - minMap.y);
	
	float m_dx = (distX / dm);
	float m_dz = (distZ / dm);


	ret.x = (m_dx * pos.x) + (m_dx * 0.5f) + (minMap.x);
	ret.y = (m_dz * pos.y) + (m_dz * 0.5f) + (minMap.y);

	return ret;
}

D3DXVECTOR4 NodeIndexToMinMax(node_pos pos, D3DXVECTOR2 minMap, D3DXVECTOR2 maxMap)
{
	D3DXVECTOR4 ret;


	float distX = abs(maxMap.x - minMap.x);
	float distZ = abs(maxMap.y - minMap.y);

	float m_dx = (distX / map_size_n);
	float m_dz = (distZ / map_size_m);


	ret.x = (m_dx * pos.x) +  minMap.x; // minx
	ret.y = (m_dz * pos.y) +  minMap.y; // miny
	ret.z = (m_dx * pos.x) + (m_dx) + minMap.x; // maxx
	ret.w = (m_dz * pos.y) + (m_dz) + minMap.y; // maxy

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

void CreateNodeMap(int map[map_size_n][map_size_m], D3DXVECTOR2 minMap, D3DXVECTOR2 maxMap, CDiffusedShader * pShader, int start_Obj, int cnt_Obj, bool dis)
{

	CGameObject **Obj = pShader->m_ppObjects;

	for (int y = 0; y < map_size_m; ++y)
	{
		for (int x = 0; x < map_size_n; ++ x)
		{
			XNA::AxisAlignedBox aabb1;
			XNA::AxisAlignedBox aabb2;

			float distX = maxMap.x - minMap.x;
			float distZ = maxMap.y - minMap.y;

			float m_dx = (distX / map_size_n);
			float m_dz = (distZ / map_size_m);

			float centerx = (m_dx * x) + (m_dx * 0.5f) - abs(minMap.x);
			float centery = (m_dz * y) + (m_dz * 0.5f) - abs(minMap.y);

			float extentsx = m_dx / 2.0f;

			aabb1.Center = { centerx,2000.0f, centery };
			aabb1.Extents = { m_dx / 2.0f, 1500.0f, m_dz / 2.0f };

			for (int i = start_Obj; i < cnt_Obj; ++i)
			{
				OBJECTTAG tag = ((CCubeMesh*)Obj[i]->m_pMesh)->m_tag;
				CDiffuseNormalVertex *mVertices = ((CCubeMesh*)Obj[i]->m_pMesh)->pVertices;
				D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
				D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };
				D3DXVECTOR3 center = (d3dxvMax + d3dxvMin) / 2.0f;
				aabb2.Center = { center.x, center.y, center.z };
				aabb2.Extents = { center.x - d3dxvMin.x , center.y - d3dxvMin.y, center.z - d3dxvMin.z };
				if (tag == MAP)
				{
					if (dis == XNA::IntersectAxisAlignedBoxAxisAlignedBox(&aabb1, &aabb2))
					{
						map[x][y] = 1;
					}
					
				}
			
			}
				
		}
	}

	

}

void CreateNodeDetailMap(int map[map_size_n][map_size_m], D3DXVECTOR2 minMap, D3DXVECTOR2 maxMap, CDiffusedShader * pShader, int start_Obj, int cnt_Obj)
{

	CGameObject **Obj = pShader->m_ppObjects;

	for (int y = 0; y < map_size_m; ++y)
	{
		for (int x = 0; x < map_size_n; ++x)
		{
			XNA::AxisAlignedBox aabb1;
			XNA::AxisAlignedBox aabb2;

			float distX = maxMap.x - minMap.x;
			float distZ = maxMap.y - minMap.y;

			float m_dx = (distX / map_size_n);
			float m_dz = (distZ / map_size_m);

			float centerx = (m_dx * x) + (m_dx * 0.5f)  + minMap.x;
			float centery = (m_dz * y) + (m_dz * 0.5f)  + minMap.y;

			float extentsx = m_dx / 2.0f;

			aabb1.Center = { centerx,2000.0f, centery };
			aabb1.Extents = { m_dx / 2.0f, 1500.0f, m_dz / 2.0f };
			int i = 0;
			bool collisionCheck = false;
			for (i = start_Obj; i < cnt_Obj; ++i)
			{
				OBJECTTAG tag = ((CCubeMesh*)Obj[i]->m_pMesh)->m_tag;
				CDiffuseNormalVertex *mVertices = ((CCubeMesh*)Obj[i]->m_pMesh)->pVertices;
				D3DXVECTOR3 d3dxvMax = { mVertices[1].m_d3dxvPosition.x , mVertices[4].m_d3dxvPosition.y, mVertices[2].m_d3dxvPosition.z };
				D3DXVECTOR3 d3dxvMin = { mVertices[0].m_d3dxvPosition.x , mVertices[0].m_d3dxvPosition.y,  mVertices[0].m_d3dxvPosition.z };
				D3DXVECTOR3 center = (d3dxvMax + d3dxvMin) / 2.0f;
				aabb2.Center = { center.x, center.y, center.z };
				aabb2.Extents = { center.x - d3dxvMin.x , center.y - d3dxvMin.y, center.z - d3dxvMin.z };
				
				if (tag == DOOR)
				{
					if (true == XNA::IntersectAxisAlignedBoxAxisAlignedBox(&aabb1, &aabb2))
					{
						map[x][y] = 0;
						break;

					}
				}
				else if (tag == FENCEHOLE)
				{
					if (true == XNA::IntersectAxisAlignedBoxAxisAlignedBox(&aabb1, &aabb2))
					{
						map[x][y] = 0;
						break;

					}
				}
				else if (tag == CONDITIONER)
				{
					if (true == XNA::IntersectAxisAlignedBoxAxisAlignedBox(&aabb1, &aabb2))
					{
						map[x][y] = 0;
						break;

					}
				}
				else if (true == XNA::IntersectAxisAlignedBoxAxisAlignedBox(&aabb1, &aabb2))
				{
					collisionCheck = true;
				}
				

			}
			if (i == cnt_Obj && !collisionCheck)
			{
				map[x][y] = 0;
			}

		}
	}
}

void FindEndPoint(int &ex, int &ey, int direction, int map[map_size_n][map_size_m])
{
	
	if (direction == 0) // ����
	{
		if (map[map_size_n - 1][map_size_m / 2] == 0)
		{
			ex = map_size_n-1;
			ey = map_size_m/2;
		}
		else
		{
			if (map[map_size_n - 1][map_size_m / 2] == 1)
			{
				if (map[map_size_n - 1][map_size_m / 2 + 1] == 0)
				{
					for (int i = map_size_m / 2; i < map_size_m; ++i)
					{
						if (map[map_size_n - 1][i] == 0)
						{
							ex = map_size_n-1;
							ey = i;
							break;
						}
					}
				}
				else
				{
					for (int i = map_size_m / 2; i > -1; --i)
					{
						if (map[map_size_n - 1][i] == 0)
						{
							ex = map_size_n-1;
							ey = i;
							break;
						}
					}
				}
				
			}
		}
		
	}
	else if (direction == 2) // ��
	{
		if (map[0][map_size_m / 2] == 0)
		{
			ex = 0;
			ey = map_size_m / 2;
		}
		else
		{
			if (map[0][map_size_m / 2] == 1)
			{
				if (map[0][map_size_m / 2 + 1] == 0)
				{
					for (int i = map_size_m / 2; i < map_size_m; ++i)
					{
						if (map[0][i] == 0)
						{
							ex = 0;
							ey = i;
							break;
						}
					}
				}
				else
				{
					for (int i = map_size_m / 2; i > -1; --i)
					{
						if (map[0][i] == 0)
						{
							ex = 0;
							ey = i;
							break;
						}
					}
				}

			}
		}
	}
	else if (direction == 1) // �Ʒ�
	{
		if (map[map_size_n/2][map_size_m-1] == 0)
		{
			ex = map_size_n / 2;
			ey = map_size_m-1;
		}
		else
		{
			if (map[map_size_n / 2][map_size_m-1] == 1)
			{
				if (map[map_size_n / 2 + 1][map_size_m-1] == 0)
				{
					for (int i = map_size_n / 2; i < map_size_n; ++i)
					{
						if (map[i][map_size_m-1] == 0)
						{
							ex = i;
							ey = map_size_m - 1;
							break;
						}
					}
				}
				else
				{
					for (int i = map_size_n / 2; i > -1; --i)
					{
						if (map[i][map_size_m - 1] == 0)
						{
							ex = i;
							ey = map_size_m - 1;
							break;
						}
					}
				}

			}
		}
	}
	else if (direction == 3) // ��
	{

		if (map[map_size_n / 2][0] == 0)
		{
			ex = map_size_n / 2;
			ey = 0;
		}
		else
		{
			if (map[map_size_n / 2][0] == 1)
			{
				if (map[map_size_n / 2 + 1][0] == 0)
				{
					for (int i = map_size_n / 2; i < map_size_n; ++i)
					{
						if (map[i][0] == 0)
						{
							ex = i;
							ey = 0;
							break;
						}
					}
				}
				else
				{
					for (int i = map_size_n / 2; i > -1; --i)
					{
						if (map[i][0] == 0)
						{
							ex = i;
							ey = 0;
							break;
						}
					}
				}

			}
		}
	}
}

bool operator<(const path_node & a, const path_node & b)
{
	return a.getPriority() > b.getPriority();
}

bool operator!=(node_pos a, node_pos b)
{
	if (a.x != b.x || a.y != b.y)
		return true;
	return false;
}
float GetDistance(float x1, float z1, float x2, float z2)
{

	return sqrt(((x2 - x1)*(x2 - x1)) + ((z2 - z1)*(z2 - z1)));
}