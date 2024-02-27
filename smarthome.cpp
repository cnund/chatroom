#include "smarthome.h"
#include "ui_smarthome.h"
#include <QTcpSocket>
SmartHome::SmartHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmartHome)
{
    ui->setupUi(this);


    connect(ui->back,&QPushButton::clicked,this,&SmartHome::back_home);
    start_connect();

    this->socket = new QTcpSocket(this);
    ui->disconnect_btn->setEnabled(false);
    ui->port->setText("20000");
    ui->ip->setText("127.0.0.1");

    ui->status->setText("未连接");

    connect(ui->start_btn,&QPushButton::clicked,this,[=](){
        unsigned int port_num = ui->port->text().toUInt();
        QString IP = ui->ip->text();
        this->socket->connectToHost(IP,port_num);
    });

    connect(this->socket,&QTcpSocket::connected,this,[=](){
        ui->textEdit->append("已连接服务器");
        ui->port->setReadOnly(true);
        ui->ip->setReadOnly(true);
        ui->start_btn->setEnabled(false);
        ui->disconnect_btn->setEnabled(true);
        ui->status->setText("已连接");
    });

    connect(this->socket,&QTcpSocket::readyRead,this,[=](){
        QByteArray data = this->socket->readAll();
        ui->textEdit->append("服务器:"+ data);
        QString msg = data;
        resServerMes(msg);
    });

    connect(ui->disconnect_btn,&QPushButton::clicked,this,[=](){
        this->socket->close();
        ui->textEdit->append("断开与服务器的连接");
        ui->port->setReadOnly(false);
        ui->ip->setReadOnly(false);
        ui->start_btn->setEnabled(true);
        ui->disconnect_btn->setEnabled(false);
        ui->status->setText("未连接`");
    });
}

SmartHome::~SmartHome()
{
    delete ui;
}

void SmartHome::resServerMes(const QString &msg)
{
    if(msg == "打开客厅空调" || msg == "关闭客厅空调")
    {
        //如果关闭信息发送过来，空调已经关闭，就什么都不做，同理，打开信息发送过来，也是一样
        if((msg == "关闭客厅空调" && ui->livingroom_airconditioner_pushButton->text()=="开") ||
                (msg == "打开客厅空调" && ui->livingroom_airconditioner_pushButton->text()=="关"))
        {
            return;
        }

        emit ui->livingroom_airconditioner_pushButton->clicked();

    }
    else  if(msg == "打开卧室空调" || msg == "关闭卧室空调")
    {
        if((msg == "关闭卧室空调" && ui->badroom_airconditioner_pushButton->text()=="开")||
        ( msg == "打开卧室空调" && ui->badroom_airconditioner_pushButton->text()==" 关"))
        {
            return;
        }
         emit ui->badroom_airconditioner_pushButton->clicked();

    }
    else  if(msg == "打开客厅风扇" || msg == "关闭客厅风扇")
    {
        if((msg == "关闭客厅风扇" && ui->livingroom_blower_pushButton->text()=="开")||
                ( msg == "打开客厅风扇" &ui->livingroom_blower_pushButton->text()==" 关"))
                {
                    return;
                }
                emit ui->livingroom_blower_pushButton->clicked();

    }
    else  if(msg == "打开客厅灯" || msg == "关闭客厅灯")
    {
        if((msg == "关闭客厅灯" && ui->livingroom_light_pushButton->text()=="开")||
                ( msg == "打开客厅灯" && ui->livingroom_light_pushButton->text() =="关"))
                {
                    return;
                }
               emit ui->livingroom_light_pushButton->clicked();
    }
    else  if(msg == "打开厨房灯" || msg == "关闭厨房灯")
    {
        if((msg ==  "关闭厨房灯" && ui->kitchen_light_pushButton->text()=="开")||
                ( msg == "打开厨房灯" && ui->kitchen_light_pushButton->text() =="关"))
                {
                    return;
                }
               emit ui->kitchen_light_pushButton->clicked();
    }
    else  if(msg == "打开卧室灯" || msg == "关闭卧室灯")
    {
        if((msg ==  "关闭卧室灯" && ui->badroom_light_pushButton_2->text()=="开")||
                ( msg ==  "打开卧室灯" && ui->badroom_light_pushButton_2->text() =="关"))
                {
                    return;
                }
              emit ui->badroom_light_pushButton_2->clicked();
    }
    else  if(msg == "打开厕所灯" || msg == "关闭厕所灯")
    {
        if((msg ==   "关闭厕所灯" && ui->bathroom_light_pushButton->text()=="开")||
                ( msg ==  "打开厕所灯"&& ui->bathroom_light_pushButton->text() =="关"))
                {
                    return;
                }
             emit ui->bathroom_light_pushButton->clicked();
    }
}

void SmartHome::start_connect()
{
    //↓灯
    connect(ui->kitchen_light_pushButton,&QPushButton::clicked,this,[=](){
        if(ui->kitchen_light_pushButton->text() == "开")
        {
            ui->kitchen_light_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/lamp_open.png"));
            ui->kitchen_light_pushButton->setText("关");
        }
        else
        {
            ui->kitchen_light_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/lamp_close.png"));
            ui->kitchen_light_pushButton->setText("开");
        }

    });
    connect(ui->livingroom_light_pushButton,&QPushButton::clicked,this,[=](){
        if(ui->livingroom_light_pushButton->text() == "开")
        {
            ui->livingroom_light_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/lamp_open.png"));
            ui->livingroom_light_pushButton->setText("关");
        }
        else
        {
            ui->livingroom_light_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/lamp_close.png"));
            ui->livingroom_light_pushButton->setText("开");
        }

    });
    connect(ui->badroom_light_pushButton_2,&QPushButton::clicked,this,[=](){
        if(ui->badroom_light_pushButton_2->text() == "开")
        {
            ui->badroom_light_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/lamp_open.png"));
            ui->badroom_light_pushButton_2->setText("关");
        }
        else
        {
            ui->badroom_light_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/lamp_close.png"));
            ui->badroom_light_pushButton_2->setText("开");
        }

    });
    connect(ui->bathroom_light_pushButton,&QPushButton::clicked,this,[=](){
        if(ui->bathroom_light_pushButton->text() == "开")
        {
            ui->bathroom_light_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/lamp_open.png"));
            ui->bathroom_light_pushButton->setText("关");
        }
        else
        {
            ui->bathroom_light_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/lamp_close.png"));
            ui->bathroom_light_pushButton->setText("开");
        }

    });
    //↑灯

    //空调
    connect(ui->livingroom_airconditioner_pushButton,&QPushButton::clicked,this,[=](){
        if(ui->livingroom_airconditioner_pushButton->text() == "开")
        {
            ui->livingroom_airconditioner_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/air_open.png"));
            ui->livingroom_airconditioner_pushButton->setText("关");
        }
        else
        {
            ui->livingroom_airconditioner_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/air_close.png"));
            ui->livingroom_airconditioner_pushButton->setText("开");
        }

    });
    connect(ui->badroom_airconditioner_pushButton,&QPushButton::clicked,this,[=](){
        if(ui->badroom_airconditioner_pushButton->text() == "开")
        {
            ui->badroom_airconditioner_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/air_open.png"));
            ui->badroom_airconditioner_pushButton->setText("关");
        }
        else
        {
            ui->badroom_airconditioner_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/air_close.png"));
            ui->badroom_airconditioner_pushButton->setText("开");
        }

    });
    //风扇
    connect(ui->livingroom_blower_pushButton,&QPushButton::clicked,this,[=](){
        if(ui->livingroom_blower_pushButton->text() == "开")
        {
            ui->livingroom_blower_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/fan_open.png"));
            ui->livingroom_blower_pushButton->setText("关");
        }
        else
        {
            ui->livingroom_blower_label->setPixmap(QPixmap(":/new/prefix1/resou/res_img/Icon/fan_close.png"));
            ui->livingroom_blower_pushButton->setText("开");
        }
    });

}
