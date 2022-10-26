#pragma once

#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <stdlib.h>

class Database {
#define SQL_RESULT_LEN 256
#define SQL_RETURN_CODE_LEN 1024

private:
	SQLHANDLE sqlConnectionHandle;
	SQLHANDLE sqlStatementHandle;
	SQLHANDLE sqlEnvironmentHandle;
	SQLWCHAR* inConnectionString = (SQLWCHAR*) L"DRIVER={SQL Server}; SERVER=LAPTOP-T6EK2SH6; DATABASE=PBL2; trusted=true;";
	SQLWCHAR outConnectionString[SQL_RESULT_LEN];

public:
	void InitializeDatabaseInstance();
	void DestroyDatabaseInstance();
};