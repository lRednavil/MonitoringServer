#pragma once
class CMonitoringServer : public CLanServer
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

class CTest : public CLanClient 
{
public:
	//�õ��Լ� �ۼ���
	virtual void Init();
	//accept ����, IP filterinig ���� ����
	virtual bool OnConnect();
	//message �м� ����
	virtual void OnRecv(CPacket* packet);
	//������ ��� ����(�Ƹ� ����Ȯ���� disconnect)
	virtual void OnExpt();

	virtual void OnDisconnect();

	virtual void OnError(int error, const WCHAR* msg);

	//�����Լ� �ۼ���
	virtual void OnStop();

private:
	int cnt;
};

