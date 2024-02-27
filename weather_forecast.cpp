#include "weather_forecast.h"
#include "ui_weather_forecast.h"

weather_forecast::weather_forecast(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::weather_forecast)
{
    ui->setupUi(this);

    read_file();

    /*bool support = QSslSocket::supportsSsl();//是否支持SSL
    QString buildVerson = QSslSocket::sslLibraryBuildVersionString();
    QString DownLoad = QSslSocket::sslLibraryVersionString();
    qDebug()<<support<<endl<<buildVerson<<endl<<DownLoad;*/


    this->http = new QNetworkAccessManager(this);

    connect(ui->pushButton,&QPushButton::clicked,this,&weather_forecast::send_request);

    connect(this->http,&QNetworkAccessManager::finished,this,&weather_forecast::handle_mes);

    connect(ui->back,&QPushButton::clicked,this,[=](){
        emit weather_isclose();
    });

    void (QComboBox::*combobox_changed)(const QString &text) = &QComboBox::currentIndexChanged;
    ui->lineEdit->setText(ui->comboBox->currentText());
    connect(ui->comboBox,combobox_changed,this,[=](){
        ui->lineEdit->setText(ui->comboBox->currentText());
    });

    //状态栏当前时间
//    QTimer *timer = new QTimer(this);
//    this->statusBar()->showMessage(QDateTime::currentDateTime().toString("yyyy年-MM月-dd日 hh:mm:ss"));
//    connect(timer,&QTimer::timeout,this,[=](){
//        this->statusBar()->showMessage(QDateTime::currentDateTime().toString("yyyy年-MM月-dd日 hh:mm:ss"));
//    });
//    timer->start(1000);
}

weather_forecast::~weather_forecast()
{
    delete ui;
}
//读取文件资源信息，获取城市id与城市名
void weather_forecast::read_file()
{
    QFile file(":/new/prefix2/resou/city.txt"); //打开文件
    if(!file.open(QIODevice::ReadOnly))         //判断文件是否打开成功
    {
        QMessageBox::critical(this,"错误","信息读取失败");
        return;
    }
    while(!file.atEnd())        //判断文件是否读取到末尾
    {
        QByteArray data = file.readLine();  //每次读一行
        QString mes = data;                 //直接转换为QString
        int ipos = mes.indexOf('\t');       //定位'\t'的位置
        QString city_id = mes.mid(0,ipos);  //从
        QString city_name = mes.mid(ipos+1);
        city_name.replace("\r\n","");
        this->city_info.insert(city_name,city_id);

        ui->comboBox->addItem(city_name);
    }
    file.close();
}

void weather_forecast::send_request()
{
    QString url = "https://query.asilu.com/weather/weather?action=weather/weather/&id=";
    url += this->city_info[ui->lineEdit->text()];
    this->http->get(QNetworkRequest(QUrl(url)));
    qDebug()<<url;
}

void weather_forecast::handle_mes(QNetworkReply *reply)
{
    int statu = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<statu;
    if(statu != 200)
    {
        QMessageBox::critical(this,"网络错误","网络错误请求失败");
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(doc.isObject())
    {
        QJsonObject obj = doc.object();

        if(obj.find("list") != obj.end())
        {
            QJsonArray arr_data = obj.find("list")->toArray();
            if(arr_data.size() != 0)
            {
                for(int i = 0; i < arr_data.size(); i++)    //循环设置好ui界面上的内容和图片
                {
//                    QString text = (arr_data[i].toObject())["date"].toString();
//                    text += " " + (arr_data[i].toObject())["weather"].toString();
//                    text += " " + (arr_data[i].toObject())["temp"].toString();
//                    text += " " + (arr_data[i].toObject())["wind"].toString();
                    if(i == 0)
                    {
                        ui->day_0->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_0->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_1->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_0->text());
                        if(path == "")
                        {
                            ui->img_0->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_0->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 1)
                    {
                        ui->day_1->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_1->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_2->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_1->text());
                        if(path == "")
                        {
                            ui->img_1->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_1->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 2)
                    {
                        ui->day_2->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_2->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_3->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_2->text());
                        if(path == "")
                        {
                            ui->img_2->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_2->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 3)
                    {
                        ui->day_3->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_3->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_4->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_3->text());
                        if(path == "")
                        {
                            ui->img_3->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_3->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 4)
                    {
                        ui->day_4->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_4->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_5->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_4->text());
                        if(path == "")
                        {
                            ui->img_4->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_4->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 5)
                    {
                        ui->day_5->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_5->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_6->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_5->text());
                        if(path == "")
                        {
                            ui->img_5->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_5->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 6)
                    {
                        ui->day_6->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_6->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_7->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_6->text());
                        if(path == "")
                        {
                            ui->img_6->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_6->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 7)
                    {
                        ui->day_7->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_7->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_8->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_7->text());
                        if(path == "")
                        {
                            ui->img_7->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_7->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 8)
                    {
                        ui->day_8->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_8->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_9->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_8->text());
                        if(path == "")
                        {
                            ui->img_8->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_8->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 9)
                    {
                        ui->day_9->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_9->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_10->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_9->text());
                        if(path == "")
                        {
                            ui->img_9->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_9->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 10)
                    {
                        ui->day_10->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_10->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_11->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_10->text());
                        if(path == "")
                        {
                            ui->img_10->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_10->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 11)
                    {
                        ui->day_11->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_11->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_12->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_11->text());
                        if(path == "")
                        {
                            ui->img_11->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_11->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 12)
                    {
                        ui->day_12->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_12->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_13->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_12->text());
                        if(path == "")
                        {
                            ui->img_12->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_12->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 13)
                    {
                        ui->day_13->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_13->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_14->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_13->text());
                        if(path == "")
                        {
                            ui->img_13->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_13->setPixmap(QPixmap(path));
                        }
                    }
                    else if(i == 14)
                    {
                        ui->day_14->setText((arr_data[i].toObject())["date"].toString());
                        ui->weather_14->setText((arr_data[i].toObject())["weather"].toString());
                        ui->T_15->setText((arr_data[i].toObject())["temp"].toString());
                        QString path = Judge_weather(ui->weather_14->text());
                        if(path == "")
                        {
                            ui->img_14->setText("找不到对应图片");
                        }
                        else
                        {
                            ui->img_14->setPixmap(QPixmap(path));
                        }
                    }
                }
            }
        }

    }
    else
    {
        QMessageBox::critical(this,"错误","获取不到信息，请稍后再试");
        return;
    }
}

QString weather_forecast::Judge_weather(QString weather)
{
    if(weather.contains("小雨")||weather.contains("雨"))
    {
        return ":/new/prefix3/resou/img/lit_rain.png";

    }
    else if(weather.contains("中雨"))
    {
        return ":/new/prefix3/resou/img/mid_rain.png";
    }
    else if(weather.contains("大雨"))
    {
        return ":/new/prefix3/resou/img/big_rain.png";
    }
    else if(weather.contains("小雪"))
    {
        return ":/new/prefix3/resou/img/lit_snow.png";
    }
    else if(weather.contains("中雪"))
    {
        return ":/new/prefix3/resou/img/mid_snow.png";
    }
    else if(weather.contains("大雪"))
    {
        return ":/new/prefix3/resou/img/big_snow.png";
    }
    else if(weather.contains("雨夹雪"))
    {
        return ":/new/prefix3/resou/img/rian_snow.png";
    }
    else if(weather.contains("冻雨")||weather.contains("冻"))
    {
        return ":/new/prefix3/resou/img/rian_snow.png";
    }
    else if(weather.contains("阴天")||weather.contains("阴"))
    {
        return ":/new/prefix3/resou/img/cloudy.png";
    }
    else if(weather.contains("多云"))
    {
        return ":/new/prefix3/resou/img/many_cloudy.png";
    }
    else if(weather.contains("晴天")||weather.contains("晴"))
    {
        return ":/new/prefix3/resou/img/sun.png";
    }
    else
    {
        qDebug()<<weather;
        return "";
    }
}

void weather_forecast::closeEvent(QCloseEvent *event)
{
    if(event->type() == QEvent::Close)
    {
        emit weather_isclose();
        event->accept();
//        event->ignore();
        qDebug()<<"XXXXXXXXXXXXX";
        return;
    }

    QWidget::event(event);

}
