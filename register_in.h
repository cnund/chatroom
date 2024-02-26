#ifndef REGISTER_IN_H
#define REGISTER_IN_H

#include <QWidget>

#include "public.h"

//#include <QPainter>
//#include <QSqlDatabase> //数据驱动
//#include <QSqlQuery>    //数据库执行语句

//#include <QDebug>
//#include <QMessageBox>
namespace Ui {
class Register_in;
}

class Register_in : public QWidget
{
    Q_OBJECT

public:
    explicit Register_in(QWidget *parent = nullptr);
    ~Register_in();

    void register_clicked();
    void sqlite_Init();
signals:
    void cancel_register();
private:
    Ui::Register_in *ui;
};

#endif // REGISTER_IN_H
