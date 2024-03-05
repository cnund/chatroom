#include "thread.h"

void *changePwd(void *changepwdinfo)
{
    cgpwd *cpwd = (cgpwd *)changepwdinfo;
    netNode netbf;
    netbf.type = CHANGE_PWD;

    char sql[256];
    write(cpwd->cfd, &netbf, sizeof(netNode));

    printf("username = %s,password = %s\n", cpwd->uMsg.username, cpwd->uMsg.password);
    // printf("阻塞在这个read上\n");
    if (read(cpwd->cfd, &netbf, BUFSIZE_MAX) == 0) // 等待输入
    {
        printf("用户中途退出了\n");
        pthread_exit(NULL); // 用户中途退出
    }
    User *u = (User *)netbf.buf;
    sprintf(sql, "update user set password = '%s' where username = '%s'", u->_account_pwd_.password, u->_account_pwd_.username);
    puts(sql);
    if (exec_sql(cpwd->db, sql) == 1)
    {
        netbf.type = OK;
        printf("修改密码完成\n");
        write(cpwd->cfd, &netbf, sizeof(netNode));
    }
    else
    {
        printf("修改密码失败\n");
    }
}

// void *chat(void *uSocket)
// {
//     CONFAB *sok = (CONFAB *)uSocket;
//     netNode buf;
//     while (1)
//     {
//         if (read(sok->cfd, &buf, BUFSIZE_MAX) == 0)
//         {
//             pthread_exit(NULL); // 退出线程
//         }
//         else
//         {
//             printf("meishi");
//         }
//     }
// }