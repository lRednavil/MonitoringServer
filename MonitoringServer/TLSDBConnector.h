#pragma once
#include <iostream>
#include "sqlinclude/mysql.h"
#include "sqlinclude/errmsg.h"

#pragma comment(lib, "mysqlclient")

class CTLSDBConnector
{
public:
	CTLSDBConnector(const char* IP, const char* user, const char* password, const unsigned int port, const char* schema);
	~CTLSDBConnector();

	bool ConnectToDB();
	void Disconnect();

	int SelectQuery(char* query);
	int SaveQuery(char* query);

	MYSQL_ROW FetchRow();
	void ResultFree();

private:
	char IP[16];
	char user[32];
	char password[32];
	char schema[128];
	unsigned int port;

	DWORD tlsIdx;
};

