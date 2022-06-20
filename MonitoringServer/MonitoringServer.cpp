#include "stdafx.h"
#include "MonitoringServer.h"

void CMonitoringServer::Init()
{
	Start(L"127.0.0.1", 15000, 2, 2, true, 100, 5000);
}

bool CMonitoringServer::OnConnectionRequest(WCHAR* IP, DWORD Port)
{
	return true;
}

bool CMonitoringServer::OnClientJoin(DWORD64 sessionID)
{
	_LOG(LOG_LEVEL_DEBUG, L"Joined to Server");

	return true;
}

bool CMonitoringServer::OnClientLeave(DWORD64 sessionID)
{
	return false;
}

void CMonitoringServer::OnRecv(DWORD64 sessionID, CPacket* packet)
{
	int cnt;
	CPacket* sendMsg = PacketAlloc();
	*packet >> cnt;

	_LOG(LOG_LEVEL_DEBUG, L"Server Recvd %d", cnt);

	PacketFree(packet);

	*sendMsg << cnt;
	SendPacket(sessionID, sendMsg);
}

void CMonitoringServer::OnError(int error, const WCHAR* msg)
{
}

void CMonitoringServer::OnStop()
{
}

void CTest::Init()
{
	Start(true);
	Connect(L"Test", L"127.0.0.1", 15000);
}

bool CTest::OnConnect()
{
	_LOG(LOG_LEVEL_DEBUG, L"Connected");
	CPacket* packet = PacketAlloc();
	cnt = 1;
	*packet << cnt;
	SendPacket(packet);
	return true;
}

void CTest::OnRecv(CPacket* packet)
{
	_LOG(LOG_LEVEL_DEBUG, L"Client Recv %d", cnt++);

	CPacket* sendMsg = PacketAlloc();
	*sendMsg << cnt;

	PacketFree(packet);

	SendPacket(sendMsg);
}

void CTest::OnExpt()
{
}

void CTest::OnDisconnect()
{
}

void CTest::OnError(int error, const WCHAR* msg)
{
}

void CTest::OnStop()
{
}
