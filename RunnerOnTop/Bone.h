#pragma once

#include"stdafx.h"

struct Bone {
	int parentIndex;
	std::string boneName;
	std::string parentName;
	D3DXMATRIX offset;
	std::vector<D3DXMATRIX> mMatrix;
	Bone(int pindex, std::string name, std::string pname) {
		parentIndex = pindex;
		boneName = name;
		parentName = pname;
	}
};
