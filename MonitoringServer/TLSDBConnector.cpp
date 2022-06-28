#include "stdafx.h"
#include "TLSDBConnector.h"
#include "DBConnector.h"

#define TIME_LIMIT 5


CTLSDBConnector::CTLSDBConnector(char* IP, char* user, char* password, unsigned int port, char* schema)
{
	memmove_s(this->IP, 16, IP, 16);
	memmove_s(this->user, 32, user, 32);
	memmove_s(this->password, 32, password, 32);
	this->port = port;
	memmove_s(this->schema, 128, schema, 128);

	tlsIdx = TlsAlloc();
}

CTLSDBConnector::~CTLSDBConnector()
{
	TlsFree(tlsIdx);
	Disconnect();
}

bool CTLSDBConnector::ConnectToDB()
{
	CDBConnector* db = (CDBConnector*)TlsGetValue(tlsIdx);
	if (db == NULL) {
		db = new CDBConnector(IP, user, password, port, schema);
		TlsSetValue(tlsIdx, db);
	}
	
	return db->ConnectToDB();
}

void CTLSDBConnector::Disconnect()
{
	CDBConnector* db = (CDBConnector*)TlsGetValue(tlsIdx);
	if (db == NULL) {
		return;
	}

	db->Disconnect();
	delete db;
}

bool CTLSDBConnector::SelectQuery(char* query)
{
	CDBConnector* db = (CDBConnector*)TlsGetValue(tlsIdx);
	if (db == NULL) {
		ConnectToDB();
		db = (CDBConnector*)TlsGetValue(tlsIdx);
	}

	return db->SelectQuery(query);
}

bool CTLSDBConnector::SaveQuery(char* query)
{
	CDBConnector* db = (CDBConnector*)TlsGetValue(tlsIdx);
	if (db == NULL) {
		ConnectToDB();
		db = (CDBConnector*)TlsGetValue(tlsIdx);
	}

	return db->SaveQuery(query);
}

MYSQL_ROW CTLSDBConnector::FetchRow()
{
	CDBConnector* db = (CDBConnector*)TlsGetValue(tlsIdx);
	if (db == NULL) {
		return NULL;
	}

	return db->FetchRow();
}

void CTLSDBConnector::ResultFree()
{
	CDBConnector* db = (CDBConnector*)TlsGetValue(tlsIdx);
	if (db == NULL) {
		return;
	}

	db->ResultFree();
}
