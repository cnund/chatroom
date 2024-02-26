#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "weather_forecast.h"
#include "monitoring.h"
#include "register_in.h"
#include "login_in.h"
#include "musicplayer.h"
#include "smarthome.h"

#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    weather_forecast *wf;
    Monitoring *mo_camera;
    Register_in *reg;
    Login_in *login_in;
    MusicPlayer *musicPlayer;
    SmartHome *smarthome;

    void hide_btns();
    void show_btns();

private:
    Ui::MainWindow *ui;

    QString currentUname;
};
#endif // MAINWINDOW_H
