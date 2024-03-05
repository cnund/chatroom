#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#include "../ut/public.h"
#include "client.h"

int cfd; // 套接字

int changePwd();
UserMessage inputup();
int main()
{
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("192.168.83.135");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);

    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1)
    {
        perror("socket");
        exit(1);
    }

    if (connect(cfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("连接失败");
        exit(1);
    }

    puts("连接成功");
    netNode buf;
    User user;

    pid_t pid = fork();
    if (pid > 0)
    {
        while (1)
        {

            char sel[10];
            printf("1.注册 2.登录 3.修改密码 4.退出\n");
            scanf("%s", sel);

            if (strcmp(sel, "1") == 0)
            {
                buf.type = REGIST;

                printf("注册\n");

                printf("输入你的真实姓名：");
                scanf("%s", user.name);
                printf("输入你的年龄：");
                scanf("%d", &user.age);
                printf("输入你的手机号：");
                scanf("%s", user.phoneNumber);
                user._account_pwd_ = inputup();

                memcpy(buf.buf, &user, sizeof(User));
            }
            else if (strcmp(sel, "2") == 0)
            {
                buf.type = LOGIN;
                printf("登录\n");
                user._account_pwd_ = inputup();

                memcpy(buf.buf, &user, sizeof(User));
            }
            else if (strcmp(sel, "3") == 0) // 修改密码
            {
                buf.type = CHANGE_PWD;
                puts("修改密码信息校验，请输入你的账号信息");
                printf("输入你的姓名：");
                scanf("%s", user.name);

                user._account_pwd_ = inputup();

                memcpy(buf.buf, &user, sizeof(User));
                write(cfd, &buf, sizeof(netNode));
            }
            else if (strcmp(sel, "4") == 0)
            {
                puts("修改密码");
                exit(0);
            }

            write(cfd, &buf, sizeof(netNode));
        }
    }
    else if (pid == 0)
    {
        while (1)
        {
            read(cfd, &buf, BUFSIZE_MAX); // 读取到了服务器返回的信息
            if (buf.type == NOOK)
            {
                printf("用户名或密码错误，不允许修改\n");
                continue;
            }
            else
            {
                printf("信息校验成功，允许修改密码\n");
                char pwd[20], npwd[20];
                printf("%s\n", user._account_pwd_.password);
                printf("输入你的新密码：");
                scanf("%s", pwd);
                printf("再次输入你的新密码：");
                scanf("%s", npwd);
                if (strcmp(pwd, npwd) == 0)
                {
                    strcpy(user._account_pwd_.password, npwd);
                    memcpy(buf.buf, &user, sizeof(User));
                    write(cfd, &buf, sizeof(netNode)); // 发送到服务器上去

                    read(cfd, &buf, BUFSIZE_MAX); // 读服务器返回的数据，确认是否修改完成
                    if (buf.type == OK)
                    {
                        printf("修改完成！\n");
                        continue;
                    }
                    else
                    {
                        printf("服务器出错了，请稍后再试\n");
                    }
                }
                else
                {
                    printf("两次密码不一致\n");
                }
            }
        }
    }
    else
    {
        puts("error");
    }
}

void lgIndex()
{
}

UserMessage inputup()
{
    UserMessage u;
    printf("输入账号：");
    scanf("%s", u.username);
    printf("输入密码：");
    scanf("%s", u.password);
    scanf("%*[^\n]%*c");
    return u;
}