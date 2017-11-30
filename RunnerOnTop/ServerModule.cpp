#include "stdafx.h"
#include "ServerModule.h"
#include "protocol.h"

extern GAMESTATENUM gameState;

ServerModule::ServerModule()
{


}


ServerModule::~ServerModule()
{
}



void ServerModule::init(HINSTANCE hInst, HWND hWnd) {
	
	hInstance = hInst;
	m_hWnd = hWnd;

	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	g_mysocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SERVERPORT);
	ServerAddr.sin_addr.s_addr = htonl(ipAddress);

	std::cout << ipAddress << std::endl;

	int Result = WSAConnect(g_mysocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr), NULL, NULL, NULL, NULL);

	//------------------------------------------------------------------------------------------------------------------
	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = BUF_SIZE;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = BUF_SIZE;

	int retval = send(g_mysocket, game_id, strlen(game_id), 0);
	//char buf[10];
	//retval = recv(g_mysocket, buf, 10, 0);
	//buf[retval] = '\0';
	//------------------------------------------------------------------------------------------------------------------
	WSAAsyncSelect(g_mysocket, m_hWnd, WM_SOCKET, FD_CLOSE | FD_READ);



#if (DebugMod == TRUE)
	err_display("Connect : ", Result);
#endif
}

void ServerModule::clienterror() {
	exit(-1);
}

void ServerModule::ReadPacket(SOCKET sock) {
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret) {
		int err_code = WSAGetLastError();
#if (DebugMod == TRUE)
		printf("Recv Error [%d]\n", err_code);
#endif
	}

	BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

	while (0 != iobyte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (iobyte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			iobyte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, iobyte);
			saved_packet_size += iobyte;
			iobyte = 0;
		}
	}
}


void  ServerModule::ProcessPacket(char *ptr)
{
	static bool first_time = true;
	switch (ptr[1]) {
	case SC_MATCHED:
	{
		gameState = LOADING;
		InvalidateRect(m_hWnd, NULL, true);
		sc_packet_matched *my_packet = reinterpret_cast<sc_packet_matched *>(ptr);
		
		game->OnCreate(hInstance, m_hWnd, my_packet->map+1);
		game->BuildObjects(my_packet->map+1, my_packet->character);
		SendLoadedPacket();
	}
	break;

	case SC_STARTGAME:
	{
		gameState = INGAME;
		SetTimer(m_hWnd, 0, 50, NULL);
	}
	break;

	case SC_ENDGAME:
	{
		sc_packet_end *my_packet = reinterpret_cast<sc_packet_end *>(ptr);
		KillTimer(m_hWnd, 0);
		if (my_packet->win == 1)
			gameState = YOUWIN;
		else
			gameState = GAMEOVER;

		InvalidateRect(m_hWnd, NULL, false);
		ReleaseCapture();
	}
	break;

	case SC_POSITION:
	{
		sc_packet_position *my_packet = reinterpret_cast<sc_packet_position *>(ptr);
		game->m_pNPC[my_packet->character]->m_d3dxmtxWorld = my_packet->matrix;

	}
	break;

	case SC_ANIMTAION:
	{
		sc_packet_animation *my_packet = reinterpret_cast<sc_packet_animation *>(ptr);

		game->m_pNPC[my_packet->character]->m_pState->SetState(my_packet->state);
	}
	break;
	default:
#if (DebugMod == TRUE)
		printf("Unknown PACKET type [%d]\n", ptr[1]);
#endif
		break;
	}
}

void ServerModule::SendStartMatchingPacket(int map)
{
	cs_packet_match_start *my_packet = reinterpret_cast<cs_packet_match_start *>(send_buffer);
	my_packet->size = sizeof(*my_packet);
	send_wsabuf.len = sizeof(*my_packet);
	DWORD iobyte;
	my_packet->type = CS_STARTMATCHING;
	my_packet->map = map;
	
	int ret = WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (ret) {
		int error_code = WSAGetLastError();
	}
}

void ServerModule::SendCancelMatchingPacket()
{
	cs_packet_match_cancel *my_packet = reinterpret_cast<cs_packet_match_cancel *>(send_buffer);
	my_packet->size = sizeof(*my_packet);
	send_wsabuf.len = sizeof(*my_packet);
	DWORD iobyte;
	my_packet->type = CS_CANCELMATCHING;


	int ret = WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (ret) {
		int error_code = WSAGetLastError();
	}
}

void ServerModule::SendPositionPacket(D3DXMATRIX matrix)
{
	cs_packet_position *my_packet = reinterpret_cast<cs_packet_position *>(send_buffer);
	my_packet->size = sizeof(*my_packet);
	send_wsabuf.len = sizeof(*my_packet);
	
	my_packet->matrix =  matrix;
	my_packet->type = CS_POSITION;

	DWORD iobyte;
	int ret = WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (ret) {
		int error_code = WSAGetLastError();
	}


}

void ServerModule::SendCatchPacket()
{
	cs_packet_catch *my_packet = reinterpret_cast<cs_packet_catch *>(send_buffer);
	my_packet->size = sizeof(*my_packet);
	send_wsabuf.len = sizeof(*my_packet);
	DWORD iobyte;
	my_packet->type = CS_CATCH;


	int ret = WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (ret) {
		int error_code = WSAGetLastError();
	}
}

void ServerModule::SendGoalPacket()
{
	cs_packet_goal *my_packet = reinterpret_cast<cs_packet_goal *>(send_buffer);
	my_packet->size = sizeof(*my_packet);
	send_wsabuf.len = sizeof(*my_packet);
	DWORD iobyte;
	my_packet->type = CS_GOAL;


	int ret = WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (ret) {
		int error_code = WSAGetLastError();
	}
}

void ServerModule::SendLoadedPacket()
{
	
	cs_packet_loaded *my_packet = reinterpret_cast<cs_packet_loaded *>(send_buffer);
	my_packet->size = sizeof(*my_packet);
	send_wsabuf.len = sizeof(*my_packet);
	DWORD iobyte;
	my_packet->type = CS_LOADED;


	int ret = WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (ret) {
		int error_code = WSAGetLastError();
	}
}

void ServerModule::SendAnimationPacket(int state)
{
	cs_packet_animation *my_packet = reinterpret_cast<cs_packet_animation *>(send_buffer);
	my_packet->size = sizeof(*my_packet);
	send_wsabuf.len = sizeof(*my_packet);
	my_packet->type = CS_ANIMTAION;
	my_packet->state = state;
	
	DWORD iobyte;
	int ret = WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (ret) {
		int error_code = WSAGetLastError();
	}
}

void ServerModule::SetGameFrameWork(CGameFramework *framework)
{
	game = framework;
}
