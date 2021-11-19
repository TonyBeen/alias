/*************************************************************************
    > File Name: main.cpp
    > Author: hsz
    > Mail:
    > Created Time: Fri 20 Aug 2021 04:20:36 PM CST
 ************************************************************************/

#include <iostream>
#include "mysql.h"
#include <log/log.h>
#include <utils/Errors.h>

#define LOG_TAG __FILE__

using namespace std;
using namespace Jarvis;

int main(int argc, char **argv)
{
    InitLog(LogLevel::DEBUG, true);
    LOGD("come in main");
    MySqlConn sql("mysql", "123456", "userdb");
    if (sql.isSqlValid() == false) {
        LOGE("sql init error");
        return -1;
    }
    int nRetCode = OK;
    // "user_id, user_name, user_password, user_login_ip"
    nRetCode = sql.SelectSql("userinfo", nullptr, "user_name = \"hsz\"");
    if (nRetCode != OK) {
        LOGE("return code = %d", nRetCode);
        if (nRetCode == UNKNOWN_ERROR) {
            LOGE("errno = %um errmsg = %s\n", sql.getErrno(), sql.getErrorStr());
        }
    }
    MySqlRes::sp res = sql.getSqlRes();
    if (res->isVaild()) {
        int fields = res->getColumnCount();
        LOGI("fields = %d", fields);
        for (int i = 0; i < fields; ++i) {
            cout << sql.getFieldByIdx(i);
            cout << "\t";
        }
        cout << endl;
        while (res->next()) {
            for (int i = 0; i < fields; ++i) {
                cout << res->getString(i) << "\t";
            }
            cout << endl;
        }
    } else {
        LOGE("Unable to get resource");
    }
    
    return 0;
}
