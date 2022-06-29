#include "stdafx.h"
#include "MonitoringServer.h"

#define WritePrivateProfileInt(section, key, value, buffer, fileName) \
_itow_s(value, buffer, 10); \
WritePrivateProfileString(section, key, buffer, fileName)

char password[] = "ajfw@!cv980dSZ[fje#@fdj123948djf";

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
	SetTimeOut(sessionID, INFINITE);
	return true;
}

bool SCMonitor::OnClientLeave(DWORD64 sessionID)
{
	return false;
}

void SCMonitor::OnRecv(DWORD64 sessionID, CPacket* packet)
{
	WORD type;
	*packet >> type;

	switch (type) {
	case en_PACKET_CS_MONITOR_TOOL_REQ_LOGIN:
	{
		_LOG(LOG_LEVEL_DEBUG, L"Req_Login");
		Recv_Login(sessionID, packet);
	}
	break;
	
	default:
		_LOG(LOG_LEVEL_DEBUG, L"On Recv Default Triggered");
		Disconnect(sessionID);
	}
	PacketFree(packet);
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

void SCMonitor::Recv_Login(DWORD64 sessionID, CPacket* packet)
{
	char code[33] = { 0 };
	packet->GetData(code, 32);

	if (packet->GetDataSize() > 0) {
		_LOG(LOG_LEVEL_DEBUG, L"packet left");
		Disconnect(sessionID);
		return;
	}

	if (strcmp(password, code) != 0) {
		_LOG(LOG_LEVEL_DEBUG, L"login code diff");
		Res_Login(sessionID, dfMONITOR_TOOL_LOGIN_ERR_SESSIONKEY);
		Disconnect(sessionID);
		return;
	}

	Res_Login(sessionID, dfMONITOR_TOOL_LOGIN_OK);
}

void SCMonitor::Res_Login(DWORD64 sessionID, BYTE res)
{
	CPacket* packet = PacketAlloc();
	constexpr WORD type = en_PACKET_CS_MONITOR_TOOL_RES_LOGIN;

	*packet << type << res;

	SendPacket(sessionID, packet);
}


void CMonitorServer::Init()
{
	netServer.Init();
	netServer.master = this;
	lanServer.Init();
	lanServer.master = this;

	db = new CTLSDBConnector("10.0.2.2", "root", "root", 3306, "logdb");

	db->ConnectToDB();

	memset(monitorMax, 0xff, sizeof(monitorMax));
}

void CMonitorServer::LanToNet(BYTE serverID, CPacket* packet)
{
	CPacket* sendMsg = netServer.PacketAlloc();
	constexpr WORD type = en_PACKET_CS_MONITOR_TOOL_DATA_UPDATE;
	int len = packet->GetDataSize();

	BYTE serverNo;
	BYTE dataType;
	int dataVal;
	int timeVal;

	*packet >> serverNo >> dataType >> dataVal >> timeVal;

	*sendMsg << type << serverID << dataType << dataVal << timeVal;

	SaveValue(serverNo, dataType, dataVal);

	netServer.SendPacketToAll(sendMsg);
}

void CMonitorServer::SaveValue(BYTE serverNo, BYTE dataType, int dataVal)
{
	monitorCnt[dataType]++;
	monitorTot[dataType] += dataVal;
	monitorMin[dataType] = min(dataVal, monitorMin[dataType]);
	monitorMax[dataType] = max(dataVal, monitorMax[dataType]);
}

void CMonitorServer::SendToDB()
{
	tm timeVal;
	time_t tv;

	char tableName[64];
	char query[256];
	char tablequery[256];

	time(&tv);
	localtime_s(&timeVal, &tv);

	sprintf_s(tableName, "monitorLog_%d%d", timeVal.tm_year + 1900, timeVal.tm_mon + 1);

	for (int type = 0; type < dfMONITOR_DATA_END; type++) {
		if (monitorCnt[type] == 0) continue;

		sprintf_s(query, "INSERT INTO `%s` (`logtime`, `serverno`, `servername`, `type`, `avr`, `min`, `max`) VALUES (NOW(), %d, %s, %d, %lld, %lld, %lld)", tableName, 1, "Server",
			type, monitorTot[type] / monitorCnt[type], monitorMin[type], monitorMax[type]);
			
		if (db->SaveQuery(query) == 1146) {
			sprintf_s(tablequery, "CREATE TABLE `%s` LIKE `monitorlog_template`", tableName);
			db->SaveQuery(query);
		}
	}
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
	return true;
}

bool SSMonitor::OnClientJoin(DWORD64 sessionID)
{
	return true;
}

bool SSMonitor::OnClientLeave(DWORD64 sessionID)
{
	return false;
}

void SSMonitor::OnRecv(DWORD64 sessionID, CPacket* packet)
{
	WORD type;
	*packet >> type;
	int serverID;

	switch (type) 
	{
	case en_PACKET_SS_MONITOR_LOGIN: 
	{
		*packet >> serverID;
		serverMap[sessionID] = serverID;
	}
	break;
	case en_PACKET_SS_MONITOR_DATA_UPDATE:
	{
		master->LanToNet(serverMap[sessionID], packet);
	}
	break;

	default:
		Disconnect(sessionID);

	}
	PacketFree(packet);
}

void SSMonitor::OnError(int error, const WCHAR* msg)
{
}

void SSMonitor::OnStop()
{
}
