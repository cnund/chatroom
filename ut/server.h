#ifndef _MAIN_H_
#define _MAIN_H_

#include "public.h"

// 初始化socket监听
int initTCPSocket(char *IP, int port);
// 使用epoll初始化服务器
int initServer(char *IP, int port);

void receptionAndForward(int epfd, void (*pFun)(int, char *)); // 接受和转发

#endif