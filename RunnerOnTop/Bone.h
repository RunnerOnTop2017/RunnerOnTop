#pragma once

#include"stdafx.h"


/*
---------CFBX-----------
-------------Cluster-------------
[0] mixamorig:Hips       �� �� �� ��  : [-1] RootNode
[1] mixamorig:Spine      �� �� �� ��  : [0] mixamorig:Hips
[2] mixamorig:Spine1     �� �� �� ��  : [1] mixamorig:Spine
[3] mixamorig:Spine2     �� �� �� ��  : [2] mixamorig:Spine1
[4] mixamorig:Neck       �� �� �� ��  : [3] mixamorig:Spine2
[5] mixamorig:Head       �� �� �� ��  : [4] mixamorig:Neck
[6] mixamorig:HeadTop_End        �� �� �� ��  : [5] mixamorig:Head
[7] mixamorig:LeftShoulder       �� �� �� ��  : [3] mixamorig:Spine2
[8] mixamorig:LeftArm    �� �� �� ��  : [7] mixamorig:LeftShoulder
[9] mixamorig:LeftForeArm        �� �� �� ��  : [8] mixamorig:LeftArm
[10] mixamorig:LeftHand  �� �� �� ��  : [9] mixamorig:LeftForeArm
[11] mixamorig:LeftHandThumb1    �� �� �� ��  : [10] mixamorig:LeftHand
[12] mixamorig:LeftHandThumb2    �� �� �� ��  : [11] mixamorig:LeftHandThumb1
[13] mixamorig:LeftHandThumb3    �� �� �� ��  : [12] mixamorig:LeftHandThumb2
[14] mixamorig:LeftHandThumb4    �� �� �� ��  : [13] mixamorig:LeftHandThumb3
[15] mixamorig:LeftHandIndex1    �� �� �� ��  : [10] mixamorig:LeftHand
[16] mixamorig:LeftHandIndex2    �� �� �� ��  : [15] mixamorig:LeftHandIndex1
[17] mixamorig:LeftHandIndex3    �� �� �� ��  : [16] mixamorig:LeftHandIndex2
[18] mixamorig:LeftHandIndex4    �� �� �� ��  : [17] mixamorig:LeftHandIndex3
[19] mixamorig:LeftHandMiddle1   �� �� �� ��  : [10] mixamorig:LeftHand
[20] mixamorig:LeftHandMiddle2   �� �� �� ��  : [19] mixamorig:LeftHandMiddle1
[21] mixamorig:LeftHandMiddle3   �� �� �� ��  : [20] mixamorig:LeftHandMiddle2
[22] mixamorig:LeftHandMiddle4   �� �� �� ��  : [21] mixamorig:LeftHandMiddle3
[23] mixamorig:LeftHandRing1     �� �� �� ��  : [10] mixamorig:LeftHand
[24] mixamorig:LeftHandRing2     �� �� �� ��  : [23] mixamorig:LeftHandRing1
[25] mixamorig:LeftHandRing3     �� �� �� ��  : [24] mixamorig:LeftHandRing2
[26] mixamorig:LeftHandRing4     �� �� �� ��  : [25] mixamorig:LeftHandRing3
[27] mixamorig:LeftHandPinky1    �� �� �� ��  : [10] mixamorig:LeftHand
[28] mixamorig:LeftHandPinky2    �� �� �� ��  : [27] mixamorig:LeftHandPinky1
[29] mixamorig:LeftHandPinky3    �� �� �� ��  : [28] mixamorig:LeftHandPinky2
[30] mixamorig:LeftHandPinky4    �� �� �� ��  : [29] mixamorig:LeftHandPinky3
[31] mixamorig:RightShoulder     �� �� �� ��  : [3] mixamorig:Spine2
[32] mixamorig:RightArm  �� �� �� ��  : [31] mixamorig:RightShoulder
[33] mixamorig:RightForeArm      �� �� �� ��  : [32] mixamorig:RightArm
[34] mixamorig:RightHand         �� �� �� ��  : [33] mixamorig:RightForeArm
[35] mixamorig:RightHandThumb1   �� �� �� ��  : [34] mixamorig:RightHand
[36] mixamorig:RightHandThumb2   �� �� �� ��  : [35] mixamorig:RightHandThumb1
[37] mixamorig:RightHandThumb3   �� �� �� ��  : [36] mixamorig:RightHandThumb2
[38] mixamorig:RightHandThumb4   �� �� �� ��  : [37] mixamorig:RightHandThumb3
[39] mixamorig:RightHandIndex1   �� �� �� ��  : [34] mixamorig:RightHand
[40] mixamorig:RightHandIndex2   �� �� �� ��  : [39] mixamorig:RightHandIndex1
[41] mixamorig:RightHandIndex3   �� �� �� ��  : [40] mixamorig:RightHandIndex2
[42] mixamorig:RightHandIndex4   �� �� �� ��  : [41] mixamorig:RightHandIndex3
[43] mixamorig:RightHandMiddle1  �� �� �� ��  : [34] mixamorig:RightHand
[44] mixamorig:RightHandMiddle2  �� �� �� ��  : [43] mixamorig:RightHandMiddle1
[45] mixamorig:RightHandMiddle3  �� �� �� ��  : [44] mixamorig:RightHandMiddle2
[46] mixamorig:RightHandMiddle4  �� �� �� ��  : [45] mixamorig:RightHandMiddle3
[47] mixamorig:RightHandRing1    �� �� �� ��  : [34] mixamorig:RightHand
[48] mixamorig:RightHandRing2    �� �� �� ��  : [47] mixamorig:RightHandRing1
[49] mixamorig:RightHandRing3    �� �� �� ��  : [48] mixamorig:RightHandRing2
[50] mixamorig:RightHandRing4    �� �� �� ��  : [49] mixamorig:RightHandRing3
[51] mixamorig:RightHandPinky1   �� �� �� ��  : [34] mixamorig:RightHand
[52] mixamorig:RightHandPinky2   �� �� �� ��  : [51] mixamorig:RightHandPinky1
[53] mixamorig:RightHandPinky3   �� �� �� ��  : [52] mixamorig:RightHandPinky2
[54] mixamorig:RightHandPinky4   �� �� �� ��  : [53] mixamorig:RightHandPinky3
[55] mixamorig:LeftUpLeg         �� �� �� ��  : [0] mixamorig:Hips
[56] mixamorig:LeftLeg   �� �� �� ��  : [55] mixamorig:LeftUpLeg
[57] mixamorig:LeftFoot  �� �� �� ��  : [56] mixamorig:LeftLeg
[58] mixamorig:LeftToeBase       �� �� �� ��  : [57] mixamorig:LeftFoot
[59] mixamorig:LeftToe_End       �� �� �� ��  : [58] mixamorig:LeftToeBase
[60] mixamorig:RightUpLeg        �� �� �� ��  : [0] mixamorig:Hips
[61] mixamorig:RightLeg  �� �� �� ��  : [60] mixamorig:RightUpLeg
[62] mixamorig:RightFoot         �� �� �� ��  : [61] mixamorig:RightLeg
[63] mixamorig:RightToeBase      �� �� �� ��  : [62] mixamorig:RightFoot
[64] mixamorig:RightToe_End      �� �� �� ��  : [63] mixamorig:RightToeBase
*/


#define mixamorig_Hips	0
#define mixamorig_Spine	1     
#define mixamorig_Spine1	2    
#define mixamorig_Spine2	3  
#define  mixamorig_Neck	4       
#define  mixamorig_Head	5       
#define  mixamorig_HeadTop_End 6
#define  mixamorig_LeftShoulder	7      
#define  mixamorig_LeftArm	8    
#define  mixamorig_LeftForeArm	9
#define  mixamorig_LeftHand	10 
#define  mixamorig_LeftHandThumb1 11   
#define  mixamorig_LeftHandThumb2 12 
#define  mixamorig_LeftHandThumb3   13 
#define  mixamorig_LeftHandThumb4    14
#define  mixamorig_LeftHandIndex1    15
#define  mixamorig_LeftHandIndex2    16
#define  mixamorig_LeftHandIndex3    17
#define  mixamorig_LeftHandIndex4    18
#define  mixamorig_LeftHandMiddle1   19
#define  mixamorig_LeftHandMiddle2   20
#define  mixamorig_LeftHandMiddle3   21
#define  mixamorig_LeftHandMiddle4   22
#define  mixamorig_LeftHandRing1     23
#define  mixamorig_LeftHandRing2     24
#define  mixamorig_LeftHandRing3     25
#define  mixamorig_LeftHandRing4     26
#define  mixamorig_LeftHandPinky1    27
#define  mixamorig_LeftHandPinky2    28
#define  mixamorig_LeftHandPinky3    29
#define  mixamorig_LeftHandPinky4    30
#define  mixamorig_RightShoulder     31
#define  mixamorig_RightArm  32
#define mixamorig_RightForeArm 33
#define mixamorig_RightHand      34
#define mixamorig_RightHandThumb1  35
#define mixamorig_RightHandThumb2   36
#define mixamorig_RightHandThumb3   37
#define mixamorig_RightHandThumb4   38
#define mixamorig_RightHandIndex1   39
#define mixamorig_RightHandIndex2   40
#define mixamorig_RightHandIndex3   41
#define mixamorig_RightHandIndex4   42
#define mixamorig_RightHandMiddle1  43
#define mixamorig_RightHandMiddle2  44
#define mixamorig_RightHandMiddle3  45
#define mixamorig_RightHandMiddle4  46
#define mixamorig_RightHandRing1    47
#define mixamorig_RightHandRing2    48
#define mixamorig_RightHandRing3    49
#define mixamorig_RightHandRing4    50
#define mixamorig_RightHandPinky1   51
#define mixamorig_RightHandPinky2   52
#define mixamorig_RightHandPinky3   53
#define mixamorig_RightHandPinky4   54
#define mixamorig_LeftUpLeg         55
#define mixamorig_LeftLeg   56
#define mixamorig_LeftFoot  57
#define mixamorig_LeftToeBase 58
#define mixamorig_LeftToe_End   59
#define mixamorig_RightUpLeg      60
#define mixamorig_RightLeg  61
#define mixamorig_RightFoot   62
#define mixamorig_RightToeBase  63    
#define mixamorig_RightToe_End    64


	 int GetBoneFullSize();
	 void GetBodyLower(bool arr[65], float f_arr[65], float ratio);
	 void GetBodyUpper(bool arr[65], float f_arr[65], float ratio);
	 void GetLeftArm(bool arr[65], float f_arr[65], float ratio);
	 void GetRightArm(bool arr[65], float f_arr[65], float ratio);
