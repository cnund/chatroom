#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int listenFd = 0; // 监听套接字

int clientarr[SOCKSAVESIZE_MAX]; // 用户套接字数组
int uCount = 0;                  // 用户套接字数组中有多少个；

extern ONLINEUSER onu[SOCKSAVESIZE_MAX]; // 当前在线的用户数组
extern int onuS;                         // 当前登录在线的用户数量

// 初始化socket监听
int initTCPSocket(char *IP, int port)
{
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(IP);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        exit(1);
    }
    int opt = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    if (listen(fd, 5) == -1)
    {
        perror("listen");
        exit(1);
    }
    return fd;
}
// 使用epoll初始化服务器
int initServer(char *IP, int port)
{
    listenFd = initTCPSocket(IP, port); // 监听的套接字

    int epfd = epoll_create(SOCKSAVESIZE_MAX); // 创建epoll对象大小，可以接纳多少的客户端

    struct epoll_event evnt;
    evnt.data.fd = listenFd;
    evnt.events = EPOLLIN;

    epoll_ctl(epfd, EPOLL_CTL_ADD, listenFd, &evnt);

    return epfd;
}
struct epoll_event eventarr[SOCKSAVESIZE_MAX]; // 同时可以有多少的事件发生

void receptionAndForward(int epfd, void (*pFun)(int, char *)) // 接受和转发
{
    while (1)
    {
        putchar(10);
        for (int i = 0; i < onuS; i++)
        {
            printf("onu = %d\n", onu[i].ufd);
        }
        putchar(10);
        int ret = epoll_wait(epfd, eventarr, SOCKSAVESIZE_MAX, -1); // 返回有多少可读事件的套接字数量
        if (ret == -1)
        {
            perror("epoll_wait");
            exit(1);
        }

        for (int i = 0; i < ret; i++)
        {
            if (eventarr[i].data.fd == listenFd)
            {
                struct sockaddr_in client_addr;
                socklen_t len = sizeof(client_addr);

                int cfd = accept(listenFd, (struct sockaddr *)&client_addr, &len);
                if (cfd == -1)
                {
                    perror("accept");
                    exit(1);
                }

                struct epoll_event evnt;
                evnt.data.fd = cfd;
                evnt.events = EPOLLIN;

                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &evnt);

                clientarr[uCount++] = cfd;
                printf("有客户端连接，现在已连接客户端数为 %d,当前在线用户%d\n", uCount, onuS);
            }
            else
            {
                char buf[BUFSIZE_MAX];
                int redret = read(eventarr[i].data.fd, buf, BUFSIZE_MAX);
                if (redret == 0 || redret == -1) // 有客户端退出，删除epoll中的检测，清理存放在数组中的套接字
                {
                    struct epoll_event evnt;
                    evnt.data.fd = eventarr[i].data.fd;
                    evnt.events = EPOLLIN;

                    close(eventarr[i].data.fd);

                    epoll_ctl(epfd, EPOLL_CTL_DEL, eventarr[i].data.fd, &evnt);

                    for (int j = 0; j < uCount; j++)
                    {
                        if (eventarr[i].data.fd == clientarr[j])
                        {

                            for (int x = j + 1; x < uCount; x++)
                            {
                                clientarr[x - 1] = clientarr[x];
                            }
                            printf("删除断开连接的套接字与客户端\n");
                            uCount--;
                            printf("当前已连接客户端数为 %d\n", uCount);
                        }
                    }
                    for (int j = 0; j < onuS; j++)
                    {
                        if (eventarr[i].data.fd == onu[j].ufd)
                        {
                            for (int x = j + 1; x < onuS; x++)
                            {
                                onu[x - 1] = onu[x];
                            }
                            onuS--;
                        }
                    }
                }
                else // 接收到了客户端发来的信息
                {
                    putchar(10);
                    User *u = (User *)(((netNode *)buf)->buf);
                    printf("用户选择处理%d\n", ((netNode *)&buf)->type);

                    if (((netNode *)&buf)->type < 3)
                    {
                        printf("fd：%d 用户选择处理%d name: %s age:%d phoneNumber:%s username:%s password: %s\n", eventarr[i].data.fd, ((netNode *)&buf)->type,
                               u->name, u->age, u->phoneNumber, u->_account_pwd_.username, u->_account_pwd_.password);
                    }

                    pFun(eventarr[i].data.fd, buf); // work();
                    printf("\n----------------\n");
                }
                // printf("redret返回值为：%d\n", redret);
            }
        }
    }
}
