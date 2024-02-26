#ifndef MONITORING_H
#define MONITORING_H

#include <QWidget>
#include "public.h"

//#include <QCamera>
//#include <QCameraInfo>
//#include <QCameraImageCapture>
//#include <QCameraViewfinder>

//#include <QPixmap>
//#include <QMessageBox>

//#include <QList>
//#include <QTime>
//#include <QDir>

namespace Ui {
class Monitoring;
}

class Monitoring : public QWidget
{
    Q_OBJECT

public:
    explicit Monitoring(QWidget *parent = nullptr);
    ~Monitoring();

    QCamera *camera;
    QCameraImageCapture *capture;
    QCameraViewfinder *view;
    QPixmap pix;
    QString current_photo_tempPath;


    void open_camera_view();
    void take_photo();
    void delete_photo();

signals:
    void call_back();

private:
    Ui::Monitoring *ui;
};

#endif // MONITORING_H
