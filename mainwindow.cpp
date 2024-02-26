#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/new/prefix1/resou/res_img/lbxxr.jpg"));
    this->setWindowTitle("低能家居系统");

//    this->hide();
    this->login_in = new Login_in(this);
    this->wf = new weather_forecast(this);
    this->reg = new Register_in(this);
    this->mo_camera = new Monitoring(this);
    this->musicPlayer = new MusicPlayer(this);
    this->smarthome = new SmartHome(this);
    //先隐藏所有的界面
    this->wf->hide();
    this->mo_camera->hide();
    this->reg->hide();
    this->musicPlayer->hide();
    this->smarthome->hide();
    hide_btns();
    //显示登录界面
    this->login_in->show();
//    this->login_in->hide();

    connect(this->smarthome,&SmartHome::back_home,this,[=](){
        this->smarthome->hide();
        show_btns();
    });
    connect(ui->smart_home_btn,&QPushButton::clicked,this,[=](){
        this->smarthome->show();
        hide_btns();
    });

    connect(this->login_in,&Login_in::returnCurrentUname,[=](const QString & currentUname){
        qDebug()<<"现在的登录的用户："<<currentUname;
        this->currentUname = currentUname;
        this->musicPlayer->setcurrentUname(currentUname);
    });

    connect(ui->music_btn,&QPushButton::clicked,this,[=](){ //点击音乐模块按钮
        this->musicPlayer->show();
        hide_btns();
    });

    connect(this->musicPlayer,&MusicPlayer::music_isback,this,[=](){
        show_btns();
        this->musicPlayer->hide();
    });

    connect(this->login_in,&Login_in::login_success,this,[=](){ //登录成功进入主界面
        this->musicPlayer->query_database_music();
        this->login_in->close();
        show_btns();
    });

    connect(this->reg,&Register_in::cancel_register,this,[=](){ //注册界面取消注册返回登录界面
        this->reg->hide();
        this->login_in->show();
    });

    connect(this->login_in,&Login_in::toRegister,this,[=](){    //登录界面进入注册界面
        this->reg->show();
        this->login_in->hide();
    });

    connect(this->mo_camera,&Monitoring::call_back,this,[=](){  //摄像头界面返回主界面
        show_btns();
        this->mo_camera->hide();
    });

    connect(this->wf,&weather_forecast::weather_isclose,this,[=](){ //天气界面返回主界面
        show_btns();
        this->wf->hide();
    });

    connect(ui->weather_mainbtn,&QPushButton::clicked,this,[=](){   //进入天气界面
//        this->wf.setAttribute(Qt::WA_DeleteOnClose);
        this->wf->resize(this->window()->size());
        this->wf->show();
        hide_btns();
    });

    connect(ui->monitoring_btn,&QPushButton::clicked,this,[=](){    //进入摄像头界面
//        this->mo_camera.setAttribute(Qt::WA_DeleteOnClose);
        this->mo_camera->resize(this->window()->size());
        this->mo_camera->show();
        hide_btns();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hide_btns()
{
//    ui->weather_mainbtn->hide();
//    ui->music_btn->hide();
//    ui->monitoring_btn->hide();
//    ui->smart_home_btn->hide();

    ui->groupBox->hide();
    ui->groupBox_2->hide();
    ui->groupBox_3->hide();
    ui->groupBox_4->hide();
}

void MainWindow::show_btns()
{
//    ui->weather_mainbtn->show();
//    ui->music_btn->show();
//    ui->monitoring_btn->show();
//    ui->smart_home_btn->show();
    ui->groupBox->show();
    ui->groupBox_2->show();
    ui->groupBox_3->show();
    ui->groupBox_4->show();
}




