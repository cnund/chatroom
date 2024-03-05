#include "public.h"

void ctseltHistory(CONFAB *confab);

sqlite3 *db;
// extern int clientarr[SOCKSAVESIZE_MAX]; // 用户套接字数组
// extern int uCount;                      // 用户套接字数组中有多少个；

ONLINEUSER onu[SOCKSAVESIZE_MAX]; // 当前在线的用户数组
int onuS = 0;                     // 当前登录在线的用户数量

void work(int cfd, char *buf)
{
    printf("进行了一次选择，当前用户在线数量为 %d\n", onuS);
    // 网络通信结构体转换
    netNode *netbf = (netNode *)buf;
    // 将网络通信的结构体中的buf转换为用户信息，因为传输的是用户信息
    User *u = (User *)netbf->buf;
    char sql[256];
    if (netbf->type == REGIST) // 注册选项
    {
        char selsql[200];

        sprintf(selsql, "select * from user where username = '%s'", u->_account_pwd_.username);
        QUERYRESULT *pqr = query_db(db, selsql);
        if (pqr->row != 0)
        {
            printf("用户名已存在，不可注册\n");
            netbf->type == NOOK;
        }
        else
        {
            // "select * from user"
            sprintf(selsql, "select * from user");
            QUERYRESULT *qr = query_db(db, selsql);
            // 用户注册时候，id自动分配;
            u->id = qr->row + 1;

            printf("注册");
            sprintf(sql, "insert into user values('%d','%s','%d','%s','%s','%s',0,datetime('now','+8 hours'))",
                    u->id, u->name, u->age, u->phoneNumber, u->_account_pwd_.username, u->_account_pwd_.password);

            if (exec_sql(db, sql) == 1)
            {
                printf("插入数据成功，用户注册成功\n");
                netbf->type = OK;
            }
            else
            {
                printf("插入数据失败，用户注册失败\n");
                netbf->type = NOOK;
            }
        }
    }
    else if (netbf->type == LOGIN) // 登录选项
    {
        printf("登录");
        sprintf(sql, "select username password from user where username = '%s' and password = '%s'", u->_account_pwd_.username, u->_account_pwd_.password);
        QUERYRESULT *qr = query_db(db, sql);

        if (qr->row == 0)
        {
            printf("用户名或密码错误\n");
            netbf->type = NOOK;
        }
        else
        {
            printf("用户登录成功\n");
            sprintf(sql, "update user set status = 1 where username = '%s'", u->_account_pwd_.username);
            exec_sql(db, sql);
            printf("用户登录状态变更：已登录 1 \n");

            onu[onuS].ufd = cfd;
            strcpy(onu[onuS++].username, u->_account_pwd_.username);
            netbf->type = OK;
        }
    }
    else if (netbf->type == CHANGE_PWD) // 修改密码
    {
        printf("修改密码\n");
        sprintf(sql, "select * from user where name = '%s' and username = '%s' and password = '%s'", u->name, u->_account_pwd_.username, u->_account_pwd_.password);
        QUERYRESULT *qr = query_db(db, sql);

        if (qr->row == 0)
        {
            printf("用户名或密码错误\n");
            netbf->type = NOOK;
        }
        else
        {
            printf("用户请求修改密码\n");

            // 需要fd,网络通信netbf,数据库db
            cgpwd cpwd;
            cpwd.cfd = cfd;
            cpwd.db = db;
            cpwd.uMsg = u->_account_pwd_;

            pthread_t pt;
            pthread_create(&pt, NULL, changePwd, &cpwd);
            return; // 完成了直接返回
        }
    }
    else if (netbf->type == CHAT) // 聊天.私聊
    {
        printf("聊天\n");
        touser *user = (touser *)netbf->buf;

        printf("%s\n", user->text);
        printf("user = %s\n", user->username);

        sprintf(sql, "select * from user where username = '%s'", user->username);
        QUERYRESULT *qr = query_db(db, sql); // 查找对应用户名的用户
        // printf("数据表中有多少个这个数据 row = %d\n", qr->row);
        int flag = 0, tofd = -1;
        char recu[20];

        for (int i = 0; i < onuS; i++)
        {
            if (strcmp(onu[i].username, user->username) == 0)
            {
                flag = 1;                      // 如果在在线用户数组中找到了目标用户，将flag设置为1否则默认为0未找到
                tofd = onu[i].ufd;             // 记录下目标用户的套接字
                strcpy(recu, onu[i].username); // 记录下目标用户的用户名
                printf("找到了目标用户\n");
            }
        }
        if (qr->row != 0 || flag == 1) // 用户在线
        {

            CONFAB confab; // 记录下发送者名字和接收者的名字还有聊天的内容
            for (int i = 0; i < onuS; i++)
            {
                if (cfd == onu[i].ufd)
                {
                    strcpy(confab.sendUser, onu[i].username);
                }
            }
            strcpy(confab.receptionUser, recu);
            strcpy(confab.text, user->text);

            // 创建数据表，保存聊天内容
            ctseltHistory(&confab);
            if (flag != 1) // 如果用户没有在线
            {
                ctseltHistory(&confab);
                sprintf(sql, "create table %s(text,time datetime)", user->username); // 创建一个目标用户的表，记录离线聊天的信息
                exec_sql(db, sql);
                sprintf(sql, "insert into %s values('%s',datetime('now','+8 hours'))",
                        user->username, user->text);
                exec_sql(db, sql);

                for (int i = 8; i < (qr->row + 1) * qr->col; i++)
                {
                    printf("%s\n", qr->ppResult[i]);
                    if (i % 8 == 0)
                        putchar(10);
                }
            }

            write(tofd, netbf, BUFSIZE_MAX); // 转发给目标信息

            strcpy(user->text, "发送完成");
            write(cfd, netbf, sizeof(netNode)); // 再转发一条信息返回给发送端
            return;                             // 处理完直接返回，不用再转发write一遍了
        }
        else
        {
            netbf->type = NOOK;
            strcpy(user->text, "此用户未上线,或此用户不存在");
            memcpy(netbf->buf, &user, sizeof(touser));
            write(cfd, netbf, sizeof(netNode)); // 再转发一条信息返回给发送端
            printf("此用户未上线,或此用户不存在\n");
        }
    }
    else if (netbf->type == GROUPCHAT) // 群聊
    {
        printf("群聊\n");

        for (int i = 0; i < onuS; i++)
        {

            if (onu[i].ufd != cfd) // 转发除了自己以外的人
            {
                write(onu[i].ufd, buf, BUFSIZE_MAX);
            }
            else // 等于自己了
            {
                // 将群聊的信息存入数据库中
                sprintf(sql, "insert into groupChatHistory values('%s','%s',datetime('now','+8 hours'))", onu[i].username, netbf->buf);
                // printf("%s %s\n", onu[i].username, netbf->buf);
                exec_sql(db, sql);
            }
        }
        return;
    }
    else if (netbf->type == CHAT_HISTORY) // 私聊用户查看历史记录
    {
        CONFAB *cfbu = (CONFAB *)netbf->buf;
        // printf("sendu = %s,recu = %s\n", cfbu->sendUser, cfbu->receptionUser);
        sprintf(sql, "select * from %sAnd%s", cfbu->sendUser, cfbu->receptionUser); // 查找聊天记录查询语句
        // 查找是我发送给别人的信息
        QUERYRESULT *pQr = query_db(db, sql);
        char sqlerr[256];
        sprintf(sqlerr, "no such table: %sAnd%s", cfbu->sendUser, cfbu->receptionUser);
        if (strcmp(sqlerr, (char *)pQr) == 0)
        {
            printf("没有sendUser = %s,receptionUser = %s的表\n", cfbu->sendUser, cfbu->receptionUser);
            goto GOTOTHERE;
            // return; // 忽略掉没有聊天记录表时候的报错，用户之间没有聊天就不会创建聊天记录表
        }

        int retRow = pQr->row; // 记录下有几行
        // memcpy(netbf->buf, &retRow, sizeof(retRow));
        // write(cfd, netbf, BUFSIZE_MAX);//将消息的数量发送给客户端
        historyMsg hmsg;

        // 历史信息表有4列,查出打印出来的行数实际需要加一
        for (int i = 4; i < ((retRow + 1) * 4); i = i + 4)
        {
            strcpy(hmsg.confabMsg.sendUser, pQr->ppResult[i]);
            strcpy(hmsg.confabMsg.receptionUser, pQr->ppResult[i + 1]);
            strcpy(hmsg.confabMsg.text, pQr->ppResult[i + 2]);
            strcpy(hmsg.datetime, pQr->ppResult[i + 3]);
            memcpy(netbf->buf, &hmsg, sizeof(historyMsg));
            write(cfd, netbf, BUFSIZE_MAX);
            // printf("%s %s %-30s %s\n", hmsg.confabMsg.sendUser, hmsg.confabMsg.receptionUser, hmsg.confabMsg.text, hmsg.datetime);
        }
    // 使用goto跳转到这里来
    GOTOTHERE:
        // 查别人发给我的信息
        sprintf(sql, "select * from %sAnd%s", cfbu->receptionUser, cfbu->sendUser);
        pQr = query_db(db, sql);
        sprintf(sqlerr, "no such table: %sAnd%s", cfbu->receptionUser, cfbu->sendUser);
        if (strcmp(sqlerr, (char *)pQr) == 0)
        {
            printf("没有sendUser = %s,receptionUser = %s的表\n", cfbu->receptionUser, cfbu->sendUser);
            return; // 忽略掉没有聊天记录表时候的报错，用户之间没有聊天就不会创建聊天记录表
        }

        retRow = pQr->row; // 记录下有几行
        for (int i = 4; i < ((retRow + 1) * 4); i = i + 4)
        {
            strcpy(hmsg.confabMsg.sendUser, pQr->ppResult[i]);
            strcpy(hmsg.confabMsg.receptionUser, pQr->ppResult[i + 1]);
            strcpy(hmsg.confabMsg.text, pQr->ppResult[i + 2]);
            strcpy(hmsg.datetime, pQr->ppResult[i + 3]);
            memcpy(netbf->buf, &hmsg, sizeof(historyMsg));
            write(cfd, netbf, BUFSIZE_MAX);
        }
    }
    else if (netbf->type == GROUP_CHAT_HISTORY) // 查看群聊的历史聊天记录
    {
        sprintf(sql, "select * from groupChatHistory");
        QUERYRESULT *qr = query_db(db, sql);
        int row = qr->row;
        gcMsg gcmsg;
        for (int i = 3; i < (row + 1) * 3; i = i + 3)
        {
            strcpy(gcmsg.usmg.username, qr->ppResult[i]);
            strcpy(gcmsg.usmg.text, qr->ppResult[i + 1]);
            strcpy(gcmsg.datetime, qr->ppResult[i + 2]);
            memcpy(netbf->buf, &gcmsg, sizeof(gcMsg));
            write(cfd, netbf, BUFSIZE_MAX);
            printf("%s %-20s %s\n", gcmsg.usmg.username, gcmsg.usmg.text, gcmsg.datetime);
        }
    }
    else if (netbf->type == PCENTER) // 用户的个人中心
    {
        printf("有用户退出登录\n");
        sprintf(sql, "select * from user where username = '%s'", u->_account_pwd_.username);
        QUERYRESULT *qr = query_db(db, sql);
        userPCenter upc;
        for (int i = 8; i < ((qr->row + 1) * qr->col); i = i + qr->col)
        {
            strcpy(upc.myself.name, qr->ppResult[i + 1]);
            upc.myself.age = atoi(qr->ppResult[i + 2]);
            // strcpy(upc.myself.age, qr->ppResult[i + 2]);
            strcpy(upc.myself.phoneNumber, qr->ppResult[i + 3]);
            strcpy(upc.myself._account_pwd_.username, qr->ppResult[i + 4]);
            strcpy(upc.myself._account_pwd_.password, qr->ppResult[i + 5]);
            strcpy(upc.datetime, qr->ppResult[i + 7]);
            memcpy(netbf->buf, &upc, sizeof(userPCenter));
            write(cfd, netbf, BUFSIZE_MAX);
            printf("%s %s %d\n", upc.datetime, upc.myself._account_pwd_.username, upc.myself.age);
        }
    }
    else if (netbf->type == EEXIT) // 用户退出登录了
    {
        printf("有用户退出登录\n");
        sprintf(sql, "update user set status = 0 where username = '%s'", u->_account_pwd_.username);
        exec_sql(db, sql);
    }
    else if (netbf->type == GET_CG) // 获取离线聊天记录，谁发了消息给我
    {
        sprintf(sql, "select * from %s", netbf->buf);
        char *selUname = malloc(20);
        strcpy(selUname, netbf->buf);
        printf(">>>>>>>>>>>%s\n", selUname);
        QUERYRESULT *qr = query_db(db, sql);
        char offerr[200];
        sprintf(offerr, "no such table: %s", netbf->buf);
        if (qr->row == 0 || (strcmp((char *)qr, offerr) == 0))
        {
            int x = 0;
            memcpy(netbf->buf, &x, sizeof(x));
            write(cfd, netbf, BUFSIZE_MAX);
        }
        else
        {
            int retRow = qr->row;
            memcpy(netbf->buf, &retRow, sizeof(retRow)); // 将有多少行发送给客户段提示他又多少个未读信息
            write(cfd, netbf, BUFSIZE_MAX);

            Offine ofline;
            for (int i = 2; i < ((qr->row + 1) * qr->col); i = i + 2)
            {
                strcpy(ofline.text, qr->ppResult[i]);
                strcat(ofline.datetime, qr->ppResult[i + 1]);
                memcpy(netbf->buf, &ofline, sizeof(Offine));
                write(cfd, netbf, BUFSIZE_MAX);
                printf("服务器发送了一条离线聊天信息\n");
                printf("%s %s\n", ofline.text, ofline.datetime);
            }

            sprintf(sql, "drop table %s", selUname);
            exec_sql(db, sql);
        }
        return;
    }
    else
    {
        puts("没有这个选项");
    }
    write(cfd, netbf, sizeof(netNode));
}

// 创建数据表，保存聊天内容
void ctseltHistory(CONFAB *confab)
{
    createTable(db, confab->sendUser, confab->receptionUser); // 创建一个表
    char sql[300];
    // 将聊天信息插入到数据表中记录下来，就是聊天记录了
    sprintf(sql, "insert into %sAnd%s values('%s','%s','%s',datetime('now','+8 hours'))",
            confab->sendUser, confab->receptionUser, confab->sendUser, confab->receptionUser, confab->text);
    exec_sql(db, sql);
}

// 查找发送方和接收方都存在的表是否存在
/* sprintf(sql, "select * from %sAnd%s where sendUser = '%s' receptionUser = '%s'",
          confab->sendUser, confab->receptionUser, confab->sendUser, confab->receptionUser);
  sprintf(sql, "select * from %sAnd%s where sendUser = '%s' receptionUser = '%s'",
          confab->receptionUser, confab->sendUser, confab->receptionUser, confab->sendUser); */

int main()
{
    db = openDatabase("user.db"); // 打开数据库
    // findTableInfo("user.db");
    char sql[256] /* = "select * from user" */;
    // sprintf(sql, "create table user(id, name, age ,phoneNumber,username,password)");//创建一个用户表
    // sprintf(sql, "insert into user values('1','qqq','12','12345678900','qqq','123')");
    // sprintf(sql, "update user set password = 'qqq' where name = 'qqq'");
    // sprintf(sql, "create table groupChatHistory(sendUser,sendTxt,time datetime)");
    // createTable(db, "qqq", "www");
    // exec_sql(db, sql);
    // findTableInfo(db, "select * from qqqAndwww");
    // findTableInfo(db, "select * from wwwAndqqq");
    // QUERYRESULT *qr = query_db(db, sql);
    // printf("%d %d\n", qr->row, qr->col);
    // for (int i = 0; i < (qr->row * qr->col) * 2; i++)
    // {
    //     printf("%12s", qr->ppResult[i]);
    //     if ((i + 1) % 6 == 0)
    //     {
    //         putchar(10);
    //     }
    // }

    int epfd = initServer("192.168.83.135", 9090);
    receptionAndForward(epfd, work);

    return 0;
}