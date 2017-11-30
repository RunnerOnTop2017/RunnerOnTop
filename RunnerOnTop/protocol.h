#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include<D3DX10math.h>

// 패킷 정보 서버->클라이언트
#define SC_MATCHED   1
#define SC_STARTGAME 2
#define SC_ENDGAME    3
#define SC_POSITION    4
#define SC_ANIMTAION 5


// 패킷 정보 2 클라이언트->서버
#define CS_STARTMATCHING   1
#define CS_CANCELMATCHING  2
#define CS_POSITION 3
#define CS_CATCH 4
#define CS_GOAL  5
#define CS_LOADED 6
#define CS_ANIMTAION 7




// BYTE를 255까지만 인식하여 int형인 BOOL로 수정

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

//패킷정보 클라->서버

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