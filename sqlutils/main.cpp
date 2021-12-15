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

void test_select()
{
    MySqlConn sql("mysql", "627096590", "userdb");
    if (sql.isSqlValid() == false) {
        LOGE("sql init error");
        return;
    }
    int nRetCode = OK;
    // "user_id, user_name, user_password, user_login_ip"
    nRetCode = sql.SelectSql("userinfo", nullptr, "user_name = \"eular\"");
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
}

// 确保本地目录下有 test.jpg, 确保数据库存在表image，确保表结构为image_name, image_data
void test_insert(const char *file)
{
    MySqlConn sql("mysql", "627096590", "userdb");
    if (sql.isSqlValid() == false) {
        LOGE("sql init error");
        return;
    }

    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        LOGE("open %s error. [%d,%s]", file, errno, strerror(errno));
        return;
    }

    uint32_t fileSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    std::shared_ptr<char> ptr(new char[fileSize]);
    LOG_ASSERT(ptr != nullptr, "");
    uint32_t offset = 0;
    while (1) {
        int ret = read(fd, ptr.get() + offset, 1024);
        if (ret == 0) {
            break;
        }
        if (ret < 0) {
            LOGE("read error. [%d,%s]", errno, strerror(errno));
            close(fd);
            return;
        }
        offset += ret;
    }
    close(fd);

    uint32_t fmtSize = 0;
    std::shared_ptr<char> fmtBuf = sql.FormatHexString(ptr.get(), fileSize, fmtSize);
    if (fmtBuf == nullptr) {
        return;
    }
    const char *table = "image(image_name, image_data)";
    std::shared_ptr<char> querySql(new char[fmtSize + 128]); // 图片名字 + 图片数据
    if (ptr == nullptr) {
        return;
    }
    fmtSize = sprintf(querySql.get(), "'%s', '%s'", file, fmtBuf.get());
    if (sql.InsertSqlBin(table, querySql.get(), fmtSize) == Jarvis::Status::OK) {
        LOGI("insert %s to image OK", file);
    } else {
        LOGE("insert %s error. [%d, %s]", file, fmtBuf.get(), sql.getErrno(), sql.getErrorStr());
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage: %s insert/select ...\n"
               "case insert: %s insert path/to/image\n", argv[0], argv[0]);
        return 0;
    }
    const char *query = argv[1];
    if (strcasecmp(query, "insert") == 0) {
        test_insert(argv[2]);
    } else if (strcasecmp(query, "select") == 0) {
        test_select();
    }
    
    return 0;
}
