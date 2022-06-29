#include "stdafx.h"
#include "DBConnector.h"

#pragma comment (lib, "Winmm")

#define TIME_LIMIT 5

CDBConnector::CDBConnector(char* IP, char* user, char* password, unsigned int port, char* schema)
{
	memmove_s(this->IP, 16, IP, 16);
	memmove_s(this->user, 32, user, 32);
	memmove_s(this->password, 32, password, 32);
	this->port = port;
	memmove_s(this->schema, 128, schema, 128);

	isConnected = false;

	mysql_init(&conn);
	unsigned int ssl = SSL_MODE_DISABLED;
	mysql_options(&conn, MYSQL_OPT_SSL_MODE, &ssl);
}

CDBConnector::~CDBConnector()
{
	Disconnect();
}

bool CDBConnector::ConnectToDB()
{
	if (isConnected) return true;

	connection = mysql_real_connect(&conn, IP, user, password, schema, port, (char*)NULL, 0);
	if (connection == NULL)
	{
		_FILE_LOG(LOG_LEVEL_ERROR, L"DB_Error_Log", L"Mysql query error : %S \n", mysql_error(&conn));
		_FILE_LOG(LOG_LEVEL_ERROR, L"DB_Error_Log", L"Mysql query error : %d \n", mysql_errno(&conn));
		isConnected = false;
		return false;
	}

	char useSchema[128];
	sprintf_s(useSchema, "use %s", schema);
	mysql_query(connection, useSchema);

	isConnected = true;
	return true;
}

void CDBConnector::Disconnect()
{
	if (!isConnected) return;

	mysql_close(connection);
}

int CDBConnector::SelectQuery(char* query)
{
	DWORD startTime = timeGetTime();
	DWORD endTime;
	int ret = 0;
	query_stat = mysql_query(connection, query);

	if (query_stat != 0)
	{
		ret = mysql_errno(&conn);
		_FILE_LOG(LOG_LEVEL_ERROR, L"DB_Error_Log", L"Mysql query error : %S \n", mysql_error(&conn));
		_FILE_LOG(LOG_LEVEL_ERROR, L"DB_Error_Log", L"Mysql query error : %d \n", ret);
		_FILE_LOG(LOG_LEVEL_ERROR, L"DB_Error_Log", L"Mysql query : %S \n", query);


		return ret;
	}

	sql_result = mysql_store_result(connection);

	//시간 초과시 로깅
	endTime = timeGetTime();
	if (endTime - startTime >= TIME_LIMIT) {
		_FILE_LOG(LOG_LEVEL_ERROR, L"QueryLog", L"Query Time Over : Time -> %ums, Query -> %S", endTime - startTime, query);
	}

	return ret;
}

int CDBConnector::SaveQuery(char* query)
{
	
	DWORD startTime = timeGetTime();
	DWORD endTime;
	int ret = 0;
	query_stat = mysql_query(connection, query);

	if (query_stat != 0)
	{
		ret = mysql_errno(&conn);
		_FILE_LOG(LOG_LEVEL_ERROR, L"DB_Error_Log", L"Mysql query error : %S \n", mysql_error(&conn));
		_FILE_LOG(LOG_LEVEL_ERROR, L"DB_Error_Log", L"Mysql query error : %d \n", ret);
		_FILE_LOG(LOG_LEVEL_ERROR, L"DB_Error_Log", L"Mysql query : %S \n", query);

		//connection = mysql_real_connect(&conn, "127.0.0.1", "root", "root", "test", 3306, (char*)NULL, 0);
		//if (connection == NULL)
		//{
		//	//wprintf_s(L"Mysql connection error : %s \n", mysql_error(&conn));
		//}
		//else {
		//	wprintf_s(L"Mysql Reconnected \n");
		//}
		return ret;
	}

	//시간 초과시 로깅
	endTime = timeGetTime();
	if (endTime - startTime >= TIME_LIMIT) {
		_FILE_LOG(LOG_LEVEL_ERROR, L"QueryLog", L"Query Time Over : Time -> %ums, Query -> %S", endTime - startTime, query);
	}
	
	return ret;
}

MYSQL_ROW CDBConnector::FetchRow()
{
	return mysql_fetch_row(sql_result);
}

void CDBConnector::ResultFree()
{
	mysql_free_result(sql_result);
}
