#include "stdafx.h"
#include "Bone.h"

int GetBoneFullSize()
{
	return 65;
}

void GetBodyLower(bool arr[65], float f_arr[65], float ratio)
{

	for (int i = 0; i < 65; ++i)
	{
		arr[i] = false;
		f_arr[i] = 0.0f;
	}

	
	
	arr[mixamorig_Hips] = true;
	arr[mixamorig_LeftUpLeg] = true;
	arr[mixamorig_LeftLeg] = true;
	arr[mixamorig_LeftFoot] = true;
	arr[mixamorig_LeftToeBase] = true;
	arr[mixamorig_LeftToe_End] = true;
	arr[mixamorig_RightUpLeg] = true;
	arr[mixamorig_RightLeg] = true;
	arr[mixamorig_RightFoot] = true;
	arr[mixamorig_RightToeBase] = true;
	arr[mixamorig_RightToe_End] = true;

	
	f_arr[mixamorig_Hips] = ratio;
	f_arr[mixamorig_LeftUpLeg] = ratio;
	f_arr[mixamorig_LeftLeg] = ratio;
	f_arr[mixamorig_LeftFoot] = ratio;
	f_arr[mixamorig_LeftToeBase] = ratio;
	f_arr[mixamorig_LeftToe_End] = ratio;
	f_arr[mixamorig_RightUpLeg] = ratio;
	f_arr[mixamorig_RightLeg] = ratio;
	f_arr[mixamorig_RightFoot] = ratio;
	f_arr[mixamorig_RightToeBase] = ratio;
	f_arr[mixamorig_RightToe_End] = ratio;
}

void GetBodyUpper(bool arr[65], float f_arr[65], float ratio)
{

	for (int i = 0; i < 65; ++i)
	{
		arr[i] = true;
		f_arr[i] = 0.0f;
	}
	//arr = { true };
	arr[mixamorig_Hips] = false;
	arr[mixamorig_LeftUpLeg] = false;
	arr[mixamorig_LeftLeg] = false;
	arr[mixamorig_LeftFoot] = false;
	arr[mixamorig_LeftToeBase] = false;
	arr[mixamorig_LeftToe_End] = false;
	arr[mixamorig_RightUpLeg] = false;
	arr[mixamorig_RightLeg] = false;
	arr[mixamorig_RightFoot] = false;
	arr[mixamorig_RightToeBase] = false;
	arr[mixamorig_RightToe_End] = false;

	for (int i = 0; i < 65; ++i)
	{
		if (arr[i]) f_arr[i] = ratio;
	}
}
