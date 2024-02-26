#include "monitoring.h"
#include "ui_monitoring.h"


Monitoring::Monitoring(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Monitoring)
{
    ui->setupUi(this);

    QList<QCameraInfo> cinfo = QCameraInfo::availableCameras();
    ui->camera->resize(ui->groupBox_2->size());

    for(int i = 0 ;i < cinfo.size(); i++)
    {
        ui->Camera_sel->addItem(cinfo.value(i).deviceName());
    }

    ui->cutCamera_btn->setEnabled(false);
    ui->del_btn->setEnabled(false);

    connect(ui->openCamera_btn,&QPushButton::clicked,this,&Monitoring::open_camera_view);//打开摄像头

    connect(ui->cutCamera_btn,&QPushButton::clicked,this,&Monitoring::take_photo);//拍照

    connect(ui->del_btn,&QPushButton::clicked,this,&Monitoring::delete_photo);//删除照片

    connect(ui->back,&QPushButton::clicked,this,[=](){  //点击返回键发送call_back信号
        emit call_back();
    });

    this->pix.load(":/new/prefix1/resou/res_img/OIP-C.jpg");//设置默认照片
    ui->photo->setPixmap(this->pix);

}

Monitoring::~Monitoring()
{
    delete ui;
}

void Monitoring::open_camera_view()
{
    if(ui->openCamera_btn->text() == "打开摄像头")
    {
        QString current_camera = ui->Camera_sel->currentText();
        this->camera = new QCamera(current_camera.toUtf8(),this);
        ui->camera->show();
        this->view = new QCameraViewfinder(ui->camera);

        view->setAspectRatioMode(Qt::IgnoreAspectRatio);

        view->resize(ui->camera->size());

        this->camera->setViewfinder(view);
        view->show();
        this->camera->start();
        ui->openCamera_btn->setText("关闭摄像头");

        ui->cutCamera_btn->setEnabled(true);
    }
    else if(ui->openCamera_btn->text() == "关闭摄像头")
    {
        ui->openCamera_btn->setText("打开摄像头");
        this->camera->stop();
        delete this->camera;
        this->camera = nullptr;
        delete this->view;
        this->view = nullptr;
        ui->cutCamera_btn->setEnabled(false);
        ui->del_btn->setEnabled(false);
    }
}

void Monitoring::take_photo()
{
    this->capture = new QCameraImageCapture(this->camera);
    QDateTime time;
    QString T = time.currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");

    qDebug()<<T;

    QString c_path = "D:/my_item_img/";
    c_path += T + "img.jpg";

    qDebug()<<c_path;

    QDir dir("D:/my_item_img");
    if(!dir.exists())
    {
        qDebug()<<"文件夹不存在";

        if(dir.mkdir("D:/my_item_img/"))
        {
            qDebug()<<"新建文件夹";
        }
        else
        {
            QMessageBox::critical(this,"拍摄错误","保存照片出错，没有该文件夹创建失败");
            return;
        }

    }
    capture->capture(c_path);

    QMessageBox::information(this,"拍照","拍摄完成");
    this->current_photo_tempPath = c_path;

    this->pix.load(c_path);
    ui->photo->setPixmap(this->pix);
    ui->del_btn->setEnabled(true);
}

void Monitoring::delete_photo()
{
    QFile file(this->current_photo_tempPath);
    if(file.exists())
    {
        if(file.remove())
        {
            QMessageBox::information(this,"删除","删除成功！");
        }
        else
        {
            QMessageBox::information(this,"删除","删除失败！");
        }
    }
    else
    {
        QMessageBox::critical(this,"删除","文件不存在");
    }
}
