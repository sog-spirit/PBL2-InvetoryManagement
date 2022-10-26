#include "Database.h"
#include <iostream>
#include <conio.h>
#include <string.h>

void Database::InitializeDatabaseInstance() {
    sqlConnectionHandle = NULL;
    sqlStatementHandle = NULL;

    if (SQL_SUCCESS != SQLAllocHandle(
        SQL_HANDLE_ENV,
        SQL_NULL_HANDLE,
        &sqlEnvironmentHandle
    )) {
        std::cout << "ERROR alloc sqlEnvironmentHandle.\n";
        DestroyDatabaseInstance();
    }

    if (SQL_SUCCESS != SQLSetEnvAttr(
        sqlEnvironmentHandle,
        SQL_ATTR_ODBC_VERSION,
        (SQLPOINTER) SQL_OV_ODBC3,
        0
    )) {
        std::cout << "ERROR set sqlEnvironmentHandle.\n";
        DestroyDatabaseInstance();
    }

    if (SQL_SUCCESS != SQLAllocHandle(
        SQL_HANDLE_DBC,
        sqlEnvironmentHandle,
        &sqlConnectionHandle
    )) {
        std::cout << "ERROR alloc sqlConnectionHandle.\n";
        DestroyDatabaseInstance();
    }

    std::cout << "Connecting to SQL Server...\n";

    switch (SQLDriverConnect(
        sqlConnectionHandle,
        NULL,
        inConnectionString,
        SQL_NTS,
        outConnectionString,
        1024,
        NULL,
        SQL_DRIVER_NOPROMPT
    )) {
    case SQL_SUCCESS:
        std::cout << "Connect to SQL Server successfully.\n";
        break;

    case SQL_SUCCESS_WITH_INFO:
        std::cout << "Connect to SQL Server successfully.\n";
        break;

    case SQL_INVALID_HANDLE:
        std::cout << "ERROR connect to SQL Server.\n";
        DestroyDatabaseInstance();
        break;

    case SQL_ERROR:
        std::cout << "ERROR connect to SQL Server.\n";
        DestroyDatabaseInstance();
        break;

    default:
        std::cout << "ERROR connect to SQL Server.\n";
        DestroyDatabaseInstance();
        break;
    }

    if (SQL_SUCCESS != SQLAllocHandle(
        SQL_HANDLE_STMT,
        sqlConnectionHandle,
        &sqlStatementHandle
    )) {
        std::cout << "ERROR alloc sqlStatementHandle\n";
        DestroyDatabaseInstance();
    }
}

void Database::DestroyDatabaseInstance() {
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStatementHandle);
    SQLDisconnect(sqlConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvironmentHandle);
    std::cout << "Database instance destroyed successfully.\n";
    exit(0);
}

void Database::GetCategories() {
    SQLWCHAR* query = (SQLWCHAR*)L"SELECT * FROM CATEGORY";

    std::cout << "Getting all categories...\n";
    if (SQL_SUCCESS != SQLExecDirect(
        sqlStatementHandle,
        query,
        SQL_NTS
    )) {
        std::cout << "An error has occurred while getting all categories...\n";
    }
    else {
        SQLINTEGER ptrSqlVersion;
        int categoryId;
        char categoryName[50];
        while (SQLFetch(sqlStatementHandle) == SQL_SUCCESS) {
            SQLGetData(sqlStatementHandle, 1, SQL_C_DEFAULT, &categoryId, 1, &ptrSqlVersion);
            SQLGetData(sqlStatementHandle, 2, SQL_C_CHAR, categoryName, sizeof(categoryName), &ptrSqlVersion);
            std::cout << "\nProduct id: " << categoryId << "\nProduct name: " << categoryName << "\n";
        }
    }
    SQLCancel(sqlStatementHandle);
}