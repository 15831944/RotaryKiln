#include <sql.h>
#include <sqlext.h>
#pragma comment(lib, "odbc32.lib")

#include <iostream>
#include <string>
#include <chrono>
#include <conio.h>
#include <thread>

int main()
{
    using namespace std;
    SQLHENV henv;
    SQLHDBC hdbc;
    SQLRETURN retcode;
    SQLCHAR ConnStrOut[256];
    SQLCHAR ConnStr[256] = "DSN=MySQLServer;UID=sa;PWD=sa";
    SQLSMALLINT ConnStrOutLen;
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    retcode = SQLDriverConnect(hdbc, NULL, ConnStr, SQL_NTS, ConnStrOut, sizeof(ConnStrOut),
        &ConnStrOutLen, SQL_DRIVER_NOPROMPT);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
    {
        cout << "ok" << endl; cout << ConnStrOutLen << endl; cout << ConnStrOut << endl;
    }
    return 0;

}