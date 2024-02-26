#include "register_in.h"
#include "ui_register_in.h"

Register_in::Register_in(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register_in)
{
    ui->setupUi(this);


    connect(ui->cancel_regist,&QPushButton::clicked,this,&Register_in::cancel_register);//取消注册

    connect(ui->regist_btn,&QPushButton::clicked,this,&Register_in::register_clicked); //确认注册
}

Register_in::~Register_in()
{
    delete ui;
}

void Register_in::register_clicked()
{
    sqlite_Init();    //调用登录类中的初始化数据库
    QString username = ui->username_line->text();
    QString password = ui->password_line->text();
    QString password1 = ui->password_line_2->text();

    if(password == password1)
    {
        QString sql = QString("insert into user(username,password) values('%1','%2');").arg(username).arg(password);
        QSqlQuery query;

        if(!query.exec(sql))
        {
            qDebug()<<"insert into table is error";
            QMessageBox::information(this,"注册认证","注册失败");
        }
        else
        {
            qDebug()<<"insert into tabel is success";
            QMessageBox::information(this,"注册认证","注册成功，自动为你跳转登录界面");
            emit cancel_register();
        }
    }
    else
    {
        QMessageBox::critical(this,"注册认证","注册失败，两次密码输入不一致");
    }
}

void Register_in::sqlite_Init()
{
    QSqlDatabase db;/* = QSqlDatabase::addDatabase("QSQLITE"); *///连接数据库

    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        qDebug() << "默认连接";
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        qDebug() << "新连接";
        db= QSqlDatabase::addDatabase("QSQLITE"); //连接数据库
    }

    db.setDatabaseName("user.db");                          //设置连接数据库的名字
    if(!db.open())                                          //打开数据库判断是否打开成功
    {
        qDebug()<<"open database error";
        return;
    }

    QString createsql = QString("create table if not exists user(id integer primary key autoincrement,"
                                "username ntext unique not NULL,"
                                "password ntext not NULL)");
    QSqlQuery query;
    if(!query.exec(createsql))
    {
        qDebug()<<"table create error";
    }
    else
    {
        qDebug()<<"table create success";
    }

}
