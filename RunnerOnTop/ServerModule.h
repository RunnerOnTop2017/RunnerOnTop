#pragma once
#include "stdafx.h"
#include "GameFramework.h"

class ServerModule
{
private:
	HINSTANCE hInstance;
	HWND m_hWnd;

private:
	SOCKET g_mysocket;

	WSABUF	send_wsabuf;
	char 	send_buffer[BUF_SIZE];
	WSABUF	recv_wsabuf;
	char	recv_buffer[BUF_SIZE];
	char	packet_buffer[BUF_SIZE];
	DWORD		in_packet_size = 0;
	int		saved_packet_size = 0;

	CGameFramework *game;

public:
	DWORD ipAddress;
	char game_id[10];
public:
	ServerModule();
	~ServerModule();
	
public:
	void init(HINSTANCE hInst, HWND hWnd);

	void clienterror();

	void ReadPacket(SOCKET sock);

	void ProcessPacket(char *ptr);

	void SendStartMatchingPacket(int map);

	void SendCancelMatchingPacket();

	void SendPositionPacket(D3DXMATRIX matrix);

	void SendCatchPacket();

	void SendGoalPacket();

	void SendLoadedPacket();

	void SendAnimationPacket(int state);

	void SendItemPacket(int kind, int index, float x, float y, float z);

	void SetGameFrameWork(CGameFramework *framework);
};

