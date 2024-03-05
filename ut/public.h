#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#define BUFSIZE_MAX 1024      // 输入输出数据大小
#define SOCKSAVESIZE_MAX 1024 // 可接收客户端的大小

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <pthread.h>

typedef struct // 通信的结构体
{
    char type;
    char buf[BUFSIZE_MAX - 1];
} netNode;

typedef struct // 用户信息，账号密码结构体
{
    char username[20];
    char password[20];
} UserMessage;

typedef struct user // 用户结构体
{
    int id;
    char name[20];
    int age;
    char phoneNumber[12];
    UserMessage _account_pwd_;
} User;

typedef struct createTrendsArr // 数组结构体
{
    void **pArr;
    int arrSize;
    int arrUse;
} ctarr;

typedef struct node // 链表结构体
{
    void *data;
    struct node *next;
    struct node *front;
} node;

typedef struct queryResult
{
    char **ppResult;
    int row;
    int col;
} QUERYRESULT;

typedef struct onlineU // 当前在线用户
{
    int ufd;           // 用户套接字
    char username[20]; // 用户名
} ONLINEUSER;

typedef struct confab // 私聊
{
    char sendUser[20];
    char receptionUser[20];
    char text[800];
} CONFAB;

typedef struct historyMsg // 私聊历史信息查看结构体
{
    CONFAB confabMsg;
    char datetime[32];
} historyMsg;

typedef struct
{
    int cfd;
    UserMessage uMsg;
    sqlite3 *db;
} cgpwd;

typedef struct // 用户私聊信息结构体
{
    char username[20];
    char text[800];
} touser;
typedef struct // 群聊历史记录需要的结构体
{
    touser usmg;
    char datetime[32];
} gcMsg;

typedef struct // 个人中心
{
    User myself;
    char datetime[32];
} userPCenter;

typedef struct
{
    char text[800];
    char datetime[20];
} Offine;

#define REGIST 1             // 注册
#define LOGIN 2              // 登录
#define CHANGE_PWD 3         // 修改密码
#define CHAT 4               // 聊天
#define GROUPCHAT 5          // 群聊
#define EEXIT 6              // 退出
#define ADDFRIEND 7          // 加好友
#define CHAT_HISTORY 8       // 查看私聊聊天记录
#define GROUP_CHAT_HISTORY 9 // 查看群聊的历史聊天记录
#define PCENTER 10           // 个人中心
#define GET_CG 11            // 获取离线消息

#define OK 101
#define NOOK 102

#include "list.h"
#include "server.h"
#include "database.h"
#include "thread.h"

#endif