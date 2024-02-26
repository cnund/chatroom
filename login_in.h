#ifndef LOGIN_IN_H
#define LOGIN_IN_H

#include <QWidget>
#include "public.h"

//#include <QPainter>
//#include <QSqlDatabase> //数据驱动
//#include <QSqlQuery>    //数据库执行语句
//#include <QDebug>
//#include <QList>
//#include <QMessageBox>
namespace Ui {
class Login_in;
}

class Login_in : public QWidget
{
    Q_OBJECT

public:
    explicit Login_in(QWidget *parent = nullptr);
    ~Login_in();

    void sqlite_Init();
    void paintEvent(QPaintEvent *event)override;
//    void closeEvent(QCloseEvent *event)override;

    void getCurrentUname(const QString & currentUname);

signals:
    void Login_isClose();
    void toRegister();
    void login_success();
    void returnCurrentUname(const QString & currentUname);

public slots:
    void on_login_btn_clicked();

public:
    QString currentUname;
private:
    Ui::Login_in *ui;


};

#endif // LOGIN_IN_H
