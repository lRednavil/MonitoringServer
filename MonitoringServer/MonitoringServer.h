#pragma once

#include <unordered_map>

class CMonitorServer;

class SCMonitor : public CNetServer
{
public:
	//시동함수 작성용
	virtual void Init();
	//accept 직후, IP filterinig 등의 목적
	virtual bool OnConnectionRequest(WCHAR* IP, DWORD Port);
	//return false; 시 클라이언트 거부.
	//return true; 시 접속 허용
	virtual bool OnClientJoin(DWORD64 sessionID);
	virtual bool OnClientLeave(DWORD64 sessionID);

	//message 분석 역할
	virtual void OnRecv(DWORD64 sessionID, CPacket* packet);

	virtual void OnError(int error, const WCHAR* msg);

	virtual void OnStop();

	virtual void OnTimeOut(DWORD64 sessionID, int reason);

private:
	void Recv_Login(DWORD64 sessionID, CPacket* packet);
	void Res_Login(DWORD64 sessionID, BYTE res);

private:
	friend class CMonitorServer;
	CMonitorServer* master;
};

class SSMonitor : public CLanServer
{
public:
	//시동함수 작성용
	virtual void Init();
	//accept 직후, IP filterinig 등의 목적
	virtual bool OnConnectionRequest(WCHAR* IP, DWORD Port);
	//return false; 시 클라이언트 거부.
	//return true; 시 접속 허용
	virtual bool OnClientJoin(DWORD64 sessionID);
	virtual bool OnClientLeave(DWORD64 sessionID);

	//message 분석 역할
	virtual void OnRecv(DWORD64 sessionID, CPacket* packet);

	virtual void OnError(int error, const WCHAR* msg);

	virtual void OnStop();

private:
	friend class CMonitorServer;
	std::unordered_map<DWORD64, BYTE> serverMap;
	CMonitorServer* master;
};

class CMonitorServer {
public:
	void Init();
	//lan에서 패킷 작성 혹은 토스
	void LanToNet(BYTE serverID, CPacket* packet);

private:
	void SendToDB();
	

private:
	SCMonitor netServer;
	SSMonitor lanServer;
	CTLSDBConnector* db;
};
