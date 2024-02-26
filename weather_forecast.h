#ifndef WEATHER_FORECAST_H
#define WEATHER_FORECAST_H

#include <QWidget>
#include "public.h"
//#include <QNetworkAccessManager>//http管理类
//#include <QNetworkRequest>      //http请求类
//#include <QNetworkReply>        //http响应类

//#include <QJsonArray>       //Json解析等
//#include <QJsonDocument>
//#include <QJsonObject>
//#include <QJsonValue>
//#include <QMap>
//#include <QString>
//#include <QList>
//#include <QLabel>

//#include <QFile>
//#include <QMessageBox>
//#include <QSslSocket>
//#include <QTime>
//#include <QTimer>
namespace Ui {
class weather_forecast;
}

class weather_forecast : public QWidget
{
    Q_OBJECT

public:
    explicit weather_forecast(QWidget *parent = nullptr);
    ~weather_forecast();

    QNetworkAccessManager *http;
    QMap<QString,QString> city_info;
    QMap<int,QLabel> day;
//    QList<QLabel> Day;

    void read_file();
    void send_request();
    void handle_mes(QNetworkReply *reply);
    QString Judge_weather(QString weather);

    void closeEvent(QCloseEvent *event)override;
signals:
    void weather_isclose();
private:
    Ui::weather_forecast *ui;
};

#endif // WEATHER_FORECAST_H
