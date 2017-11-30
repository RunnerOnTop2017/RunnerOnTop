#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include<D3DX10math.h>

// ��Ŷ ���� ����->Ŭ���̾�Ʈ
#define SC_MATCHED   1
#define SC_STARTGAME 2
#define SC_ENDGAME    3
#define SC_POSITION    4
#define SC_ANIMTAION 5


// ��Ŷ ���� 2 Ŭ���̾�Ʈ->����
#define CS_STARTMATCHING   1
#define CS_CANCELMATCHING  2
#define CS_POSITION 3
#define CS_CATCH 4
#define CS_GOAL  5
#define CS_LOADED 6
#define CS_ANIMTAION 7




// BYTE�� 255������ �ν��Ͽ� int���� BOOL�� ����

struct sc_packet_matched {
	BYTE size;
	BYTE type;
	WORD map;
	WORD character;
};

struct sc_packet_start {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_end {
	BYTE size;
	BYTE type;
	WORD id;
	BYTE win;
};

struct sc_packet_position {
	BYTE size;
	BYTE type;
	WORD character;
	D3DXMATRIX matrix;
};

struct sc_packet_animation {
	BYTE size;
	BYTE type;
	BYTE character;
	BYTE state;
};

//��Ŷ���� Ŭ��->����

struct cs_packet_match_start {
	BYTE size;
	BYTE type;
	BYTE map;
	WORD id;
};

struct cs_packet_match_cancel {
	BYTE size;
	BYTE type;
	WORD id;
};

struct cs_packet_position {
	BYTE size;
	BYTE type;
	WORD id;
	D3DXMATRIX matrix;
};

struct cs_packet_catch {
	BYTE size;
	BYTE type;
	WORD id;
};

struct cs_packet_goal {
	BYTE size;
	BYTE type;
	WORD id;
};

struct cs_packet_loaded {
	BYTE size;
	BYTE type;
	WORD id;
};

struct cs_packet_animation {
	BYTE size;
	BYTE type;
	BYTE character;
	BYTE state;
};

#endif