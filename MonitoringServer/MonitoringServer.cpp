#include "stdafx.h"
#include "MonitoringServer.h"

#define WritePrivateProfileInt(section, key, value, buffer, fileName) \
_itow_s(value, buffer, 10); \
WritePrivateProfileString(section, key, buffer, fileName)

void SCMonitor::Init()
{
	WCHAR IP[16];
	DWORD PORT;
	DWORD createThreads;
	DWORD runningThreads;
	bool isNagle;
	DWORD maxConnect;
	DWORD snap;
	int packetSize;

	WCHAR temp[22];

	GetPrivateProfileString(L"NetServerOpt", L"IP", L"0.0.0.0", IP, 16, L".//ServerSettings.ini");
	PORT = GetPrivateProfileInt(L"NetServerOpt", L"PORT", NULL, L".//ServerSettings.ini");
	createThreads = GetPrivateProfileInt(L"NetServerOpt", L"CreateThreads", NULL, L".//ServerSettings.ini");
	runningThreads = GetPrivateProfileInt(L"NetServerOpt", L"RunningThreads", NULL, L".//ServerSettings.ini");
	isNagle = GetPrivateProfileInt(L"NetServerOpt", L"isNagle", NULL, L".//ServerSettings.ini");
	maxConnect = GetPrivateProfileInt(L"NetServerOpt", L"MaxConnect", NULL, L".//ServerSettings.ini");
	snap = GetPrivateProfileInt(L"NetServerOpt", L"SnapLatency", 4, L".//ServerSettings.ini");
	packetSize = GetPrivateProfileInt(L"NetServerOpt", L"PacketSize", 1460, L".//ServerSettings.ini");

	
	WritePrivateProfileString(L"NetServerOpt", L"IP", IP, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"NetServerOpt", L"PORT", PORT, temp, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"NetServerOpt", L"CreateThreads", createThreads, temp, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"NetServerOpt", L"RunningThreads", runningThreads, temp, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"NetServerOpt", L"isNagle", isNagle, temp, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"NetServerOpt", L"MaxConnect", maxConnect, temp, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"NetServerOpt", L"SnapLatency", snap, temp, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"NetServerOpt", L"PacketSize", packetSize, temp, L".//ServerSettings.ini");

	if ((PORT * createThreads * runningThreads * maxConnect) == 0) {
		_FILE_LOG(LOG_LEVEL_ERROR, L"INIT_LOG", L"INVALID ARGUMENTS or No ini FILE");
		CRASH();
	}

	Start(IP, PORT, createThreads, runningThreads, isNagle, maxConnect, snap, packetSize);
}

bool SCMonitor::OnConnectionRequest(WCHAR* IP, DWORD Port)
{
	_LOG(LOG_LEVEL_DEBUG, L"Request From %s :: %d", IP, Port);
	return true;
}

bool SCMonitor::OnClientJoin(DWORD64 sessionID)
{
	_LOG(LOG_LEVEL_DEBUG, L"Joined to Server");

	return true;
}

bool SCMonitor::OnClientLeave(DWORD64 sessionID)
{
	return false;
}

void SCMonitor::OnRecv(DWORD64 sessionID, CPacket* packet)
{
	int cnt;
	CPacket* sendMsg = PacketAlloc();
	*packet >> cnt;

	_LOG(LOG_LEVEL_DEBUG, L"Server Recvd %d", cnt);

	PacketFree(packet);

	*sendMsg << cnt;
	SendPacket(sessionID, sendMsg);
}

void SCMonitor::OnError(int error, const WCHAR* msg)
{
}

void SCMonitor::OnStop()
{
}

void SCMonitor::OnTimeOut(DWORD64 sessionID, int reason)
{
}


void CMonitorServer::Init()
{
	netServer.Init();
	lanServer.Init();
}

void SSMonitor::Init()
{
	WCHAR IP[16];
	DWORD PORT;
	DWORD createThreads;
	DWORD runningThreads;
	bool isNagle;
	DWORD maxConnect;
	int packetSize;

	WCHAR temp[22];

	GetPrivateProfileString(L"LanServerOpt", L"IP", L"0.0.0.0", IP, 16, L".//ServerSettings.ini");
	PORT = GetPrivateProfileInt(L"LanServerOpt", L"PORT", NULL, L".//ServerSettings.ini");
	createThreads = GetPrivateProfileInt(L"LanServerOpt", L"CreateThreads", NULL, L".//ServerSettings.ini");
	runningThreads = GetPrivateProfileInt(L"LanServerOpt", L"RunningThreads", NULL, L".//ServerSettings.ini");
	isNagle = GetPrivateProfileInt(L"LanServerOpt", L"isNagle", NULL, L".//ServerSettings.ini");
	maxConnect = GetPrivateProfileInt(L"LanServerOpt", L"MaxConnect", NULL, L".//ServerSettings.ini");
	packetSize = GetPrivateProfileInt(L"LanServerOpt", L"PacketSize", 1460, L".//ServerSettings.ini");


	WritePrivateProfileString(L"LanServerOpt", L"IP", IP, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"LanServerOpt", L"PORT", PORT, temp, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"LanServerOpt", L"CreateThreads", createThreads, temp, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"LanServerOpt", L"RunningThreads", runningThreads, temp, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"LanServerOpt", L"isNagle", isNagle, temp, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"LanServerOpt", L"MaxConnect", maxConnect, temp, L".//ServerSettings.ini");
	WritePrivateProfileInt(L"LanServerOpt", L"PacketSize", packetSize, temp, L".//ServerSettings.ini");

	if ((PORT * createThreads * runningThreads * maxConnect) == 0) {
		_FILE_LOG(LOG_LEVEL_ERROR, L"INIT_LOG", L"INVALID ARGUMENTS or No ini FILE");
		CRASH();
	}

	Start(IP, PORT, createThreads, runningThreads, isNagle, maxConnect, packetSize);
}

bool SSMonitor::OnConnectionRequest(WCHAR* IP, DWORD Port)
{
	return false;
}

bool SSMonitor::OnClientJoin(DWORD64 sessionID)
{
	return false;
}

bool SSMonitor::OnClientLeave(DWORD64 sessionID)
{
	return false;
}

void SSMonitor::OnRecv(DWORD64 sessionID, CPacket* packet)
{
}

void SSMonitor::OnError(int error, const WCHAR* msg)
{
}

void SSMonitor::OnStop()
{
}
