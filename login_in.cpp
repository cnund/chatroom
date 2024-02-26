#include "login_in.h"
#include "ui_login_in.h"


Login_in::Login_in(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login_in)
{
    ui->setupUi(this);

    //发送注册信号
    connect(ui->regist_btn,&QPushButton::clicked,this,[=](){
        emit toRegister();
    });
}

Login_in::~Login_in()
{
    delete ui;
}
//数据库初始化
void Login_in::sqlite_Init()
{   
    QSqlDatabase db /*= QSqlDatabase::addDatabase("QSQLITE")*/; //连接数据库
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
//重写绘图事件
void Login_in::paintEvent(QPaintEvent *event)
{
    this->resize(780,520);

    QWidget::paintEvent(event);

    QPainter painter(this);
    QPixmap pixmap(":/new/prefix1/resou/res_img/scenery_bg.jpg");


    if(pixmap.size() != this->size())
    {
        pixmap = pixmap.scaled(this->size(),Qt::KeepAspectRatio);
    }
    painter.drawPixmap(0,0,pixmap);
}

void Login_in::getCurrentUname(const QString & currentUname)
{
    qDebug()<<currentUname;
}

void Login_in::on_login_btn_clicked()
{
    sqlite_Init();
    QString username = ui->username_line->text();
    QString password = ui->password_line->text();
    qDebug()<<"账号:"<<username<<" 密码:"<<password;
    QString sql = QString("select *from user where username = '%1' and password = '%2'").arg(username).arg(password);
    this->currentUname = username;
    emit returnCurrentUname(currentUname);

    QSqlQuery query(sql);

    if(!query.next())
    {
        qDebug()<<"login error";
        QMessageBox::critical(this,"登录认证","登录失败，账号或者密码错误");
    }
    else
    {
        qDebug()<<"login success";
        QMessageBox::information(this,"登录认证","登录成功");
        emit login_success();
    }

}
//void Login_in::closeEvent(QCloseEvent *event)
//{
//    if(event->type() == QEvent::Close)
//    {
//        emit Login_isClose();
//        event->accept();
//            event->ignore();
//        qDebug()<<"XXXXXXXXXXXXX";
//    }
//    else
//    {
//        QWidget::event(event);
//    }


//}
