#pragma once

#include <unordered_map>

class CMonitorServer;

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

private:
	friend class CMonitorServer;
	std::unordered_map<DWORD64, BYTE> serverMap;
	CMonitorServer* master;
};

class CMonitorServer {
public:
	void Init();
	//lan���� ��Ŷ �ۼ� Ȥ�� �佺
	void LanToNet(BYTE serverID, CPacket* packet);

private:
	void SendToDB();
	

private:
	SCMonitor netServer;
	SSMonitor lanServer;
	CTLSDBConnector* db;
};
