#pragma once
#include <iostream>
#include "sqlinclude/mysql.h"
#include "sqlinclude/errmsg.h"

#pragma comment(lib, "mysqlclient")

class CDBConnector
{
public:
	//생성 후  ConnectToDB호출할 것
	CDBConnector(char* IP, char* user, char* password, unsigned int port, char* schema);
	~CDBConnector();

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

	bool isConnected;

	MYSQL conn;
	MYSQL* connection = nullptr;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	int query_stat;
};

