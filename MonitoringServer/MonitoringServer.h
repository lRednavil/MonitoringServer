#pragma once
class CMonitoringServer : public CLanServer
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

class CTest : public CLanClient 
{
public:
	//시동함수 작성용
	virtual void Init();
	//accept 직후, IP filterinig 등의 목적
	virtual bool OnConnect();
	//message 분석 역할
	virtual void OnRecv(CPacket* packet);
	//예외일 경우 선택(아마 높은확률로 disconnect)
	virtual void OnExpt();

	virtual void OnDisconnect();

	virtual void OnError(int error, const WCHAR* msg);

	//종료함수 작성용
	virtual void OnStop();

private:
	int cnt;
};

