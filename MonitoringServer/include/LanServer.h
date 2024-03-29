#pragma once
struct SESSION;
class CPacket;

class CLanServer
{
public:
	//오픈 IP / 포트 / 워커스레드 수(생성수, 러닝수) / 나글옵션 / 최대접속자 수
	bool Start(const WCHAR * IP, DWORD port, DWORD createThreads, DWORD runningThreads, bool isNagle, DWORD maxConnect, DWORD packetSize = CPacket::eBUFFER_DEFAULT);
	void Stop();
	int GetSessionCount();

	bool Disconnect(DWORD64 sessionID);
	bool SendPacket(DWORD64 sessionID, CPacket* packet);
	//접속자 전원에게 send
	void SendPacketToAll(CPacket* packet);
	//기본 참조카운트 1부여 및 초기화 실행
	CPacket* PacketAlloc();
	void	PacketFree(CPacket* packet);
	int GetPacketPoolCapacity();
	int GetPacketPoolUse();

	//시동함수 작성용
	virtual void Init() = 0;
	//accept 직후, IP filterinig 등의 목적
	virtual bool OnConnectionRequest(WCHAR* IP, DWORD Port) = 0; 
	//return false; 시 클라이언트 거부.
	//return true; 시 접속 허용
	virtual bool OnClientJoin(DWORD64 sessionID) = 0;
	virtual bool OnClientLeave(DWORD64 sessionID) = 0;

	//message 분석 역할
	virtual void OnRecv(DWORD64 sessionID, CPacket* packet) = 0;

	virtual void OnError(int error, const WCHAR* msg) = 0;

	//종료함수 작성용
	virtual void OnStop() = 0;
private:

	bool NetInit(const WCHAR * IP, DWORD port, bool isNagle);
	bool ThreadInit(const DWORD createThreads, const DWORD runningThreads);

	void NetClose();
	void ThreadClose();

	//packet에 header 할당
	void	HeaderAlloc(CPacket* packet);

	//return NULL for fail
	//FindSession + Check Flag + Session 재확인
	//반드시 LoseSession과 페어를 맞출 것
	SESSION* AcquireSession(DWORD64 sessionID);
	//ioCnt 차감 이후 Release 세션 진입
	//반드시 AcquireSession 이나 ioCnt 증가 후에 사용
	void LoseSession(SESSION* session);

	SESSION* FindSession(DWORD64 sessionID);
	bool	MakeSession(WCHAR* IP, SOCKET sock, DWORD64* ID);
	void	ReleaseSession(SESSION* session);

	static unsigned int __stdcall WorkProc(void* arg);
	static unsigned int __stdcall AcceptProc(void* arg);
	void _WorkProc();
	void _AcceptProc();
	void RecvProc(SESSION* session);
	bool RecvPost(SESSION* session);
	bool SendPost(SESSION* session);

	

private:
	//array for session
	SESSION* sessionArr;
	//stack for session index
	CLockFreeStack<int> sessionStack;

	//sessionID용으로 사용
	DWORD64 totalAccept;

	//monitor
	DWORD sessionCnt;
	BYTE netMode; // << 나중에 화이트리스트 모드 등등 변경용
	bool isServerOn;

	//readonly
	CTLSMemoryPool<CPacket>* packetPool;
	DWORD maxConnection;
	int packetSize;
	SOCKET listenSock;
	HANDLE hIOCP;

	HANDLE* hThreads;
};

