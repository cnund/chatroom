#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include "public.h"

//#include <QMediaPlayer>
//#include <QMediaPlaylist>
//#include <QUrl>

//#include <QNetworkAccessManager>
//#include <QNetworkReply>
//#include <QNetworkRequest>

//#include <QJsonArray>
//#include <QJsonDocument>
//#include <QJsonObject>
//#include <QJsonValue>

//#include <QSqlDatabase>
//#include <QSqlQuery>

//#include <QFileDialog>
//#include <QMessageBox>
//#include <QTime>
//#include <QTimer>
namespace Ui {
class MusicPlayer;
}

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();

    QMediaPlayer *player;
    QMediaPlaylist *playList;

    QNetworkAccessManager *http;


    void search_music_request();
    void handle_request(QNetworkReply *reply);
    void sqlite_init();
    void data_insert_base(QString music_name,QString music_url);
    void query_database_music();
    void delete_music(const QString &music_name);

    void setcurrentUname(QString currentUname);
signals:
    void newPlay();
    void music_isback();
private:
    Ui::MusicPlayer *ui;

    QString currentUname;

    qint64 music_current_play_time;
};

#endif // MUSICPLAYER_H
