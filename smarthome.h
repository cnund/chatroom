#ifndef SMARTHOME_H
#define SMARTHOME_H

#include <QWidget>
#include "public.h"
namespace Ui {
class SmartHome;
}

class SmartHome : public QWidget
{
    Q_OBJECT

public:
    explicit SmartHome(QWidget *parent = nullptr);
    ~SmartHome();

    QTcpSocket *socket;


    void resServerMes(const QString &msg);
signals:
    void back_home();

private:
    Ui::SmartHome *ui;

    void start_connect();
};

#endif // SMARTHOME_H
