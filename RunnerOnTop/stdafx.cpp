// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// RunnerOnTop.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"
#include "xnaCollision.h"

// TODO: 필요한 추가 헤더는
// 이 파일이 아닌 STDAFX.H에서 참조합니다.

//bool CollisionCheck(D3DXVECTOR3 & max1, D3DXVECTOR3 & min1, D3DXVECTOR3 & max2, D3DXVECTOR3 & min2, bool & x, bool & y, bool & z)
//{
//	/*
//	if (min2.x < min1.x && max1.x < max2.x && min2.z < min1.z && max1.z < max2.z) {
//		if (max1.y > min2.y && min1.y < max2.y)
//			y = true;
//	}
//
//	if (min2.x > min1.x &&  max2.x < max1.x && min2.z> min2.z) {
//	
//	}
//*/
//
//
//
//	if (x || y || z)
//		return true;
//	else
//		return false;
//}

bool CollisionCheck(D3DXVECTOR3 & max1, D3DXVECTOR3 & min1, D3DXVECTOR3 & max2, D3DXVECTOR3 & min2, D3DXVECTOR3 & shift, bool & x, bool & y, bool & z)
{
	XNA::AxisAlignedBox aabbX;
	XNA::AxisAlignedBox aabbY;
	XNA::AxisAlignedBox aabbZ;

	XNA::AxisAlignedBox OBJ_aabb;

	aabbX.Center = { (max1.x + min1.x) / 2.0f + shift.x,
					(max1.y + min1.y) / 2.0f ,
					(max1.z + min1.z) / 2.0f };

	aabbY.Center = { (max1.x + min1.x) / 2.0f,
		(max1.y + min1.y) / 2.0f + shift.y,
		(max1.z + min1.z) / 2.0f };

	aabbZ.Center = { (max1.x + min1.x) / 2.0f,
		(max1.y + min1.y) / 2.0f ,
		(max1.z + min1.z) / 2.0f + shift.z };

	D3DXVECTOR3 tmp = (max1 - min1) / 2.0f;
	aabbX.Extents = aabbY.Extents = aabbZ.Extents = { tmp.x, tmp.y, tmp.z };

	tmp = (max2 + min2) / 2.0f;
	OBJ_aabb.Center = { tmp.x ,tmp.y, tmp.z };

	tmp = (max2 - min2) / 2.0f;
	OBJ_aabb.Extents = { tmp.x, tmp.y, tmp.z };

	
	x = XNA::IntersectAxisAlignedBoxAxisAlignedBox(&aabbX, &OBJ_aabb);
	y = XNA::IntersectAxisAlignedBoxAxisAlignedBox(&aabbY, &OBJ_aabb);
	z = XNA::IntersectAxisAlignedBoxAxisAlignedBox(&aabbZ, &OBJ_aabb);

	if (x || y || z)
		return true;
	else
		return false;
}
