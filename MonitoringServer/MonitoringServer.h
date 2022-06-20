#pragma once

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

};

class CMonitorServer {
public:
	void Init();

	void SendThrough(CPacket* packet);

private:
	SCMonitor netServer;
	SSMonitor lanServer;

};
