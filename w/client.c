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

#include "client.h"

int cfd; // 套接字
char username[20];

int lookHistory(pthread_t *pt, char *lsel);
void chatConfab(pthread_t *pt);
void lgIndex();
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

    while (1)
    {

        char sel[10];
        printf("*********欢迎来到简陋的聊天室**********\n");
        putchar(10);
        printf("   1.注册 2.登录 3.修改密码 4.退出     \n");
        putchar(10);
        printf("***************************************\n");
        scanf("%s", sel);

        if (strcmp(sel, "1") == 0) // 注册
        {
            buf.type = REGIST;

            printf("****************注册*******************\n");

            printf("输入你的真实姓名：");
            scanf("%s", user.name);
            printf("输入你的年龄：");
            scanf("%d", &user.age);
            printf("输入你的手机号：");
            scanf("%s", user.phoneNumber);
            user._account_pwd_ = inputup();

            memcpy(buf.buf, &user, sizeof(User));
            write(cfd, &buf, sizeof(netNode));

            read(cfd, &buf, BUFSIZE_MAX);
            if (buf.type == OK)
            {
                printf("注册成功，请登录\n");
                continue;
            }
            else
            {
                printf("注册失败，用户名已被使用\n");
            }
        }
        else if (strcmp(sel, "2") == 0) // 登录
        {
            system("clear");
            buf.type = LOGIN;
            printf("****************登录*******************\n");
            user._account_pwd_ = inputup();

            memcpy(buf.buf, &user, sizeof(User));

            write(cfd, &buf, sizeof(netNode));

            read(cfd, &buf, BUFSIZE_MAX);
            if (buf.type == OK)
            {
                strcpy(username, user._account_pwd_.username);
                printf("登录成功\n");
                printf("当前用户是：%s\n", username);
                lgIndex();
                continue;
            }
            else
            {
                printf("账号或密码错误，请检查你的账号密码\n");
            }
        }
        else if (strcmp(sel, "3") == 0) // 修改密码
        {
            buf.type = CHANGE_PWD;

            puts("**修改密码信息校验，请输入你的账号信息**");
            printf("输入你的姓名：");
            scanf("%s", user.name);

            user._account_pwd_ = inputup();

            memcpy(buf.buf, &user, sizeof(User));
            write(cfd, &buf, sizeof(netNode));

            read(cfd, &buf, BUFSIZE_MAX); // 读取到了服务器返回的信息
            if (buf.type == NOOK)
            {
                printf("用户名或密码错误，不允许修改\n");
                continue;
            }
            else if (buf.type == CHANGE_PWD)
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
            else
            {
                printf("%d\n", buf.type);
                printf("出现了其他问题\n");
            }
        }
        else if (strcmp(sel, "4") == 0)
        {
            puts("退出");
            exit(0);
        }
        else
        {
            printf("************没有这个选项***************\n");
            printf("\n");
            sleep(1);
        }

        // write(cfd, &buf, sizeof(netNode));
    }
}

void *threadWorkRead() // 接收消息的线程
{
    netNode netbf;
    touser *u;
    while (1)
    {
        read(cfd, &netbf, BUFSIZE_MAX);
        if (netbf.type == CHAT)
        {
            u = (touser *)netbf.buf;
            printf("%s\n", u->text);
        }
        else if (netbf.type == GROUPCHAT)
        {
            printf("%s\n", netbf.buf);
        }
        else if (netbf.type == CHAT_HISTORY)
        {
            historyMsg *hmsg = (historyMsg *)netbf.buf;
            printf("%s发给%s %-30s %s\n", hmsg->confabMsg.sendUser, hmsg->confabMsg.receptionUser, hmsg->confabMsg.text, hmsg->datetime);
        }
        else if (netbf.type == GROUP_CHAT_HISTORY)
        {
            gcMsg *gcmsg = (gcMsg *)netbf.buf;
            printf("%-20s %s\n", gcmsg->usmg.text, gcmsg->datetime);
        }
        else if (netbf.type == GET_CG)
        {
            Offine *ofline = (Offine *)netbf.buf;
            printf("%s %30s\n", ofline->text, ofline->datetime);
        }
    }
}

void lgIndex() // 登录完成之后的页面
{
    netNode buf;
    User u;
    printf("\n");
    printf("****************欢迎*******************\n");
    printf("\n");
    char sel[10];

    pthread_t pt;

    buf.type = GET_CG;
    memcpy(buf.buf, &username, sizeof(username));
    write(cfd, &buf, BUFSIZE_MAX); // 发送消息给服务器获取我的离线消息
    read(cfd, &buf, BUFSIZE_MAX);
    int *ino = (int *)buf.buf;

    while (1)
    {
        printf("\n");
        printf("1.聊天 2.群聊 3.查看群聊记录 4.个人中心 5.退出登录  你有(%d)未读信息\n", *ino);
        printf("\n");
        printf("****************************************\n");
        scanf("%s", sel);
        if (strcmp(sel, "1") == 0)
        {
            chatConfab(&pt);
        }
        else if (strcmp(sel, "2") == 0) // 群聊
        {
            printf("已进入群聊模式，退出请输入  <<exit>>\n");

            pthread_create(&pt, NULL, threadWorkRead, NULL);

            buf.type = GROUPCHAT;
            char gbuf[800], pbuf[800];
            while (1)
            {
                scanf("%s", pbuf);

                if (strcmp(pbuf, "exit") == 0)
                {
                    printf("退出群聊模式\n");
                    pthread_cancel(pt);
                    break;
                }
                sprintf(gbuf, "群聊消息：[%s]:%s", username, pbuf);
                memcpy(buf.buf, &gbuf, sizeof(gbuf));
                write(cfd, &buf, BUFSIZE_MAX);
            }
        }
        else if (strcmp(sel, "3") == 0) // 查看群聊历史记录
        {
            buf.type = GROUP_CHAT_HISTORY;
            write(cfd, &buf, BUFSIZE_MAX);
            pthread_create(&pt, NULL, threadWorkRead, NULL);
            sleep(2);
            pthread_cancel(pt);
        }
        else if (strcmp(sel, "4") == 0) // 个人中心
        {
            system("clear");
            printf("*****************个人中心*******************\n");
            buf.type = PCENTER;
            strcpy(u._account_pwd_.username, username);
            memcpy(buf.buf, &u, sizeof(User));
            write(cfd, &buf, BUFSIZE_MAX);

            read(cfd, &buf, BUFSIZE_MAX);
            userPCenter *upc = (userPCenter *)buf.buf;
            printf("名字：%s \n年龄：%d \n手机号：%s \n账号：%s \n密码：%s \n注册时间:%s\n",
                   upc->myself.name, upc->myself.age, upc->myself.phoneNumber, upc->myself._account_pwd_.username, upc->myself._account_pwd_.password, upc->datetime);
            printf("*****************************************\n");
        }
        else if (strcmp(sel, "5") == 0) // 退出登录，向服务器发送退出登录信息
        {
            printf("退出登录\n");
            pthread_cancel(pt);
            buf.type = EEXIT;
            strcpy(u._account_pwd_.username, username);
            memcpy(buf.buf, &u, sizeof(User));
            write(cfd, &buf, BUFSIZE_MAX);
            return;
        }
        else
        {
            printf("没有这个选项，请重新输入\n");
        }
    }
}

// 聊天
void chatConfab(pthread_t *pt)
{
    netNode buf;
    User u;

    char sel[20];

    while (1)
    {
        putchar(10);
        printf("      1.找人聊天 2.查看信息 3.返回\n");
        printf("****************************************\n");
        scanf("%s", sel);
        if (strcmp(sel, "1") == 0) // 找人聊天
        {
            char friendName[20];
            buf.type = CHAT;
            printf("输入你要聊天的用户名：");
            scanf("%s", u._account_pwd_.username);

            touser ctuser; // 保存自己需要聊天的目标用户
            strcpy(ctuser.username, u._account_pwd_.username);

            memcpy(buf.buf, &ctuser, sizeof(touser));
            write(cfd, &buf, BUFSIZE_MAX); // 将需要和谁聊天的目标用户发送到服务器

            read(cfd, &buf, BUFSIZE_MAX);
            if (buf.type == NOOK)
            {
                printf("此用户未上线,或此用户不存在\n");
            }
            else
            {
                char cbf[800];
                printf("私聊模式，退出请输入<< exit >>\n");
                while (1)
                {
                    scanf("%s", cbf);
                    if (strcmp(cbf, "exit") == 0)
                    {
                        // pthread_cancel(pt);
                        printf("退出私聊\n");
                        break;
                    }
                    sprintf(ctuser.text, "私聊：[%s]：%s", username, cbf);
                    memcpy(buf.buf, &ctuser, sizeof(touser));
                    write(cfd, &buf, BUFSIZE_MAX);
                }
            }
        }
        else if (strcmp(sel, "2") == 0) // 查看信息
        {
            while (1)
            {
                printf("1.查看当前有谁发了信息给我 2.聊天历史记录 3.返回\n");
                char lsel[10];
                scanf("%s", lsel); // 输入选择
                int ret = lookHistory(pt, lsel);
                if (ret == 1)
                {
                    system("clear");
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
        else if (strcmp(sel, "3") == 0)
        {
            printf("返回\n");
            system("clear");
            return;
        }
    }
}

int lookHistory(pthread_t *pt, char *lsel) // 查看私聊的历史聊天记录
{
    netNode ch;
    if (strcmp(lsel, "1") == 0)
    {
        pthread_create(pt, NULL, threadWorkRead, NULL); // 开启读线程
        printf("退出请输入<< exit >>\n");
        while (1)
        {
            char eexit[10];
            scanf("%s", eexit);
            if (strcmp(eexit, "exit") == 0)
            {
                pthread_cancel(*pt);
                break;
            }
        }
    }
    else if (strcmp(lsel, "2") == 0) // 查看聊天记录
    {
        ch.type = CHAT_HISTORY;
        CONFAB cfbu;
        strcpy(cfbu.sendUser, username);
        printf("查看聊天记录-------退出请输入<< exit >>\n");
        pthread_create(pt, NULL, threadWorkRead, NULL); // 开启线程
        while (1)
        {
            printf("你需要找和谁的聊天记录：");
            scanf("%s", cfbu.receptionUser);
            if (strcmp(cfbu.receptionUser, "exit") == 0)
            {
                pthread_cancel(*pt);
                break;
            }
            memcpy(ch.buf, &cfbu, sizeof(CONFAB));
            write(cfd, &ch, BUFSIZE_MAX);
            sleep(1);
        }
    }
    else if (strcmp(lsel, "3") == 0) // 正常返回
    {
        return 1;
    }
    else
    {
        return 0;
    }
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