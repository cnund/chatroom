#include "database.h"

sqlite3 *openDatabase(char *dbName) // 创建数据库和打开数据库
{
    sqlite3 *db = NULL;
    int data = -1;

    data = sqlite3_open(dbName, &db);
    if (data < 0)
    {
        printf("创建database失败:%s\n", sqlite3_errmsg(db));
        return NULL;
    }
    else
    {
        printf("创建database成功\n");
        // sqlite3_close(db);
        return db;
    }
}

int exec_sql(sqlite3 *db, char *sql) // 执行sql语句
{
    char *pErrMsg = NULL;

    int ret = sqlite3_exec(db, sql, NULL, NULL, &pErrMsg);
    if (ret != SQLITE_OK)
    {
        if (strcmp(pErrMsg, "table user already exists") == 0)
        {
            return 1;
        }
        else if (strcmp(pErrMsg, "table qqq already exists") == 0)
        {
        }
        printf("执行sql语句失败:%s\n", pErrMsg);
        return 0;
    }
    printf("执行了一次sql命令\n");
    return 1;
}

int createTable(sqlite3 *db, char *sendUser, char *receptionUser) // 创建用户聊天记录数据表
{
    char *pErrMsg = NULL;
    char sql[400], err[300];
    sprintf(sql, "create table %sAnd%s(sendUser,receptionUser,text,time DATETIME)", sendUser, receptionUser);
    sprintf(err, "table %sAnd%s already exists", sendUser, receptionUser);
    int ret = sqlite3_exec(db, sql, NULL, NULL, &pErrMsg);

    if (ret != SQLITE_OK)
    {
        if (strcmp(pErrMsg, err) == 0)
        {
            return 1;
        }
        printf("执行sql语句失败:%s\n", pErrMsg);
        return 0;
    }
    printf("执行了一次sql命令\n");
    return 1;
}

QUERYRESULT *query_db(sqlite3 *db, char *sql)
{
    QUERYRESULT *qr = malloc(sizeof(QUERYRESULT));
    char *pErrMsg;
    int ret = sqlite3_get_table(db, sql, &qr->ppResult, &qr->row, &qr->col, &pErrMsg);
    if (ret != SQLITE_OK)
    {
        printf("执行查询语句时出错:%s\n", pErrMsg);
        return (QUERYRESULT *)pErrMsg;
    }
    return qr;
}

int findTableInfo(sqlite3 *db, char *sql)
{
    // sqlite3 *db = NULL;
    int data;
    char *msg = 0;
    int row = 0, column = 0; // 行   列
    char **Result = NULL;    // 结果
    // char *sql;
    // 打开指定的数据库文件，如果不存在将创建一个同名的数据库文件

    // data = sqlite3_open(dbName, &db);
    // if (data)
    // {
    //     printf("create testsqlite failure:%s\n", sqlite3_errmsg(db));
    //     exit(1);
    // }

    printf("create testsqlite successfuly\n");

    // sql = "SELECT * FROM user ";
    // sqlite3_get_table获取数据库表格的内容，通常用来执行数据库查询SQL语句
    // 参数
    // 1. 数据库的指针
    // 2. 要执行的SQL查询语句
    // 3. 查询的结果
    // 4. 查询结果的行数
    // 5. 查询结果的列数
    // 6. 错误信息
    data = sqlite3_get_table(db, sql, &Result, &row, &column, &msg);

    printf("row = %d\n", row);

    if (data == 0)
    {
        printf("row:%d column=%d \n", row, column);

        printf("the result of querying is:\n");

        int i = 0, j = 0;
        // 查询的结果存放在Result中，它是一个一维数组，包含了列名，所以在遍历数据的时候，可以跳过列名
        for (i = 3; i < (row + 1) * column; i++)
        {

            printf("%s", Result[i]);
            printf("\n");
        }
    }
    else if (data)
    {
        printf("SQL error!:%s\n", msg); // 打印错误信息
        sqlite3_free(msg);              // 释放掉azResult的内存空间
    }
    sqlite3_close(db); // 关闭数据库
    return 0;
}

int delTable(char *dbName)
{
    sqlite3 *db = NULL;
    int data;
    char *msg = 0;
    char *sql;
    char *data1;

    // 打开指定的数据库文件，如果不存在将创建一个同名的数据库文件
    data = sqlite3_open(dbName, &db);
    if (data)
    {
        printf("create tetssqlite failure:%s\n", sqlite3_errmsg(db));
        exit(1);
    }

    printf("create testsqlite successfuly\n");
    sql = "DELETE FROM stu";

    data = sqlite3_exec(db, sql, NULL, (void *)data1, &msg);
    if (data)
    {
        printf("SQL error!:%s\n", msg); // 打印错误信息
        sqlite3_free(msg);              // 释放掉zerrmsg的内存空间
    }
    else
    {
        printf("delete recodes successfuly\n");
    }
    sqlite3_close(db); // 关闭数据库
    return 0;
}