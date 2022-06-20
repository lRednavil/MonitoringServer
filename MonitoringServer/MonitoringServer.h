#pragma once

class SCMonitor : public CNetServer
{
public:
	//�õ��Լ� �ۼ���
	virtual void Init();
	//accept ����, IP filterinig ���� ����
	virtual bool OnConnectionRequest(WCHAR* IP, DWORD Port);
	//return false; �� Ŭ���̾�Ʈ �ź�.
	//return true; �� ���� ���
	virtual bool OnClientJoin(DWORD64 sessionID);
	virtual bool OnClientLeave(DWORD64 sessionID);

	//message �м� ����
	virtual void OnRecv(DWORD64 sessionID, CPacket* packet);

	virtual void OnError(int error, const WCHAR* msg);

	virtual void OnStop();

	virtual void OnTimeOut(DWORD64 sessionID, int reason);
};

class SSMonitor : public CLanServer
{
public:
	//�õ��Լ� �ۼ���
	virtual void Init();
	//accept ����, IP filterinig ���� ����
	virtual bool OnConnectionRequest(WCHAR* IP, DWORD Port);
	//return false; �� Ŭ���̾�Ʈ �ź�.
	//return true; �� ���� ���
	virtual bool OnClientJoin(DWORD64 sessionID);
	virtual bool OnClientLeave(DWORD64 sessionID);

	//message �м� ����
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
