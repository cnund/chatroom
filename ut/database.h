#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "public.h"

sqlite3 *openDatabase(char *dbName);

int exec_sql(sqlite3 *db, char *sql); // 执行sql语句

QUERYRESULT *query_db(sqlite3 *db, char *sql); // 进行查找

int findTableInfo(sqlite3 *db, char *sql);

int delTable(char *dbName);

int createTable(sqlite3 *db, char *sendUser, char *receptionUser);
#endif