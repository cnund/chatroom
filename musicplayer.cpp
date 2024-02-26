#include "musicplayer.h"
#include "ui_musicplayer.h"

MusicPlayer::MusicPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    this->music_current_play_time = 0;  //初始化一下音乐播放的时间
    sqlite_init();  //直接初始化一下数据库连接

    connect(ui->back,&QPushButton::clicked,this,&MusicPlayer::music_isback);    //点击返回时发送返回的信号给主程序


    //QT+= multimedia 多媒体音频，播放音乐和声音
    //QT+= multmediawidets 多媒体视频，用来播放视频和摄像头录像的

    this->player = new QMediaPlayer(this);
    this->playList = new QMediaPlaylist(this);

    this->player->setPlaylist(this->playList);


    this->playList->setPlaybackMode(QMediaPlaylist::Loop);//设置播放模式
    ui->verticalSlider->setValue(50);
    this->player->setVolume(ui->verticalSlider->value());


    connect(ui->verticalSlider,&QSlider::valueChanged,this->player,&QMediaPlayer::setVolume);

    //    this->playList->addMedia(QMediaContent(QUrl("qrc:/new/prefix4/resou/music_res/浪帝 - 再也没有.mp3")));
    //    this->playList->addMedia(QMediaContent(QUrl("qrc:/new/prefix4/resou/music_res/陈一发儿 - 童话镇.mp3")));
    //    this->playList->addMedia(QMediaContent(QUrl("qrc:/new/prefix4/resou/music_res/李荣浩-老街 (2015“天生李荣浩”巡演上海站)(标清).mp4")));
    //    this->playList->addMedia(QMediaContent(QUrl("qrc:/new/prefix4/resou/music_res/胡歌-指纹 (《轩辕剑之天之痕》电视剧插曲)(标清).mp4")));
    //    this->playList->addMedia(QMediaContent(QUrl("qrc:/new/prefix4/resou/music_res/茄子蛋-浪子回头(标清).mp4")));

    //    ui->listWidget->addItem("再也没有.mp3");
    //    ui->listWidget->addItem("童话镇.mp3");
    //    ui->listWidget->addItem("老街.mp4");
    //    ui->listWidget->addItem("指纹.mp4");
    //    ui->listWidget->addItem("浪子回头.mp4");

    //计时器，计算音乐开始时间过了 多久
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        ui->horizontalSlider->setValue(this->music_current_play_time);
        this->music_current_play_time++;

        QTime tm = QTime::fromMSecsSinceStartOfDay(music_current_play_time*1000);
        QString ttt = tm.toString("'%1':'%2'").arg(tm.minute(),2,10,QChar('0')).arg(tm.second(),2,10,QChar('0'));
        ui->music_playcurrentime_label->setText(ttt);
        qDebug()<<this->music_current_play_time;

    });

    connect(this,&MusicPlayer::newPlay,this,[=](){  //当我播放了新的歌曲的时候，重新开始计时
        this->music_current_play_time = 0;
        timer->start(1000);

    });
    connect(ui->listWidget,&QListWidget::doubleClicked,this,[=](){  //双击歌单
        int sel = ui->listWidget->currentRow();
        int curr = this->playList->currentIndex();
        if(sel != curr)    //如果当前选中的歌和播放的歌不同
        {
            this->playList->setCurrentIndex(sel);


            if(ui->stop_btn->text() == "播放")
            {
                this->player->play();   //直接开始播放
                emit this->newPlay();   //发送新播放歌曲的信号
                ui->stop_btn->setText("暂停");
            }

        }
        else    //暂停
        {
            if(ui->stop_btn->text() == "暂停")
            {
                this->player->pause();
                ui->stop_btn->setText("播放");
                timer->stop();
            }
            else
            {
                this->player->play();
                emit this->newPlay();
                ui->stop_btn->setText("暂停");
            }

        }
    });

    connect(ui->stop_btn,&QPushButton::clicked,this,[=](){  //歌曲的播放

        int sel = ui->listWidget->currentRow();
        int playing = this->playList->currentIndex();
        if(sel != playing)
        {
            this->playList->setCurrentIndex(sel);
            this->player->play();
        }

        if(ui->stop_btn->text() == "播放")
        {
            this->player->play();
            ui->stop_btn->setText("暂停");
            this->newPlay();
        }
        else if(ui->stop_btn->text() == "暂停")
        {
            this->player->pause();
            ui->stop_btn->setText("播放");
            timer->stop();
        }
    });

    connect(ui->privious_btn,&QPushButton::clicked,this,[=](){  //上一首歌曲
        this->playList->previous();
        if(ui->stop_btn->text() == "播放")
        {
            emit ui->stop_btn->clicked();
        }
    });
    connect(ui->next_btn,&QPushButton::clicked,this,[=](){  //下一首歌曲
        this->playList->next();
        if(ui->stop_btn->text() == "播放")
        {
            emit ui->stop_btn->clicked();
        }
    });

    if(ui->listWidget->count() > 0) //设置默认选择
    {
        ui->listWidget->setCurrentRow(0);
    }

    void (QListWidget::*current_changeRow)(int row) = &QListWidget::setCurrentRow;
    connect(this->playList,&QMediaPlaylist::currentIndexChanged,ui->listWidget,current_changeRow);  //当当前播放的歌曲改变的时候，当前显示高亮的歌曲也改变

    connect(ui->comboBox,&QComboBox::currentTextChanged,this,[=](const QString &text){  //改变播放模式
        qDebug()<<"playlist mode is changed";
        if(text == "列表循环")
        {
            this->playList->setPlaybackMode(QMediaPlaylist::Loop);
        }
        else if(text == "单曲循环")
        {
            this->playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        }
        else if(text == "随机播放")
        {
            this->playList->setPlaybackMode(QMediaPlaylist::Random);
        }
        else if(text == "顺序播放")
        {
            this->playList->setPlaybackMode(QMediaPlaylist::Sequential);
        }
        else if(text == "播放单首")
        {
            this->playList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        }

    });

    connect(ui->add_local_music_btn,&QPushButton::clicked,this,[=](){   //添加本地音乐或视频
        qDebug()<<"add local music or video";
        QStringList data = QFileDialog::getOpenFileNames(this,"添加音乐","D:\\Users\\0.0\\Desktop\\qrs\\qtdir\\my_item","*.mp3 *.mp4");

        for(int i = 0; i < data.size();i++)
        {
            this->playList->addMedia(QMediaContent(QUrl::fromLocalFile(data[i])));
            QFileInfo fileinfo(data[i]);
            ui->listWidget->addItem(fileinfo.fileName());
            data_insert_base(fileinfo.fileName(),fileinfo.filePath());
        }
    });

    connect(ui->delete_btn,&QPushButton::clicked,this,[=](){    //删除音乐列表的音乐
        qDebug()<<"delete music_list_wiget music or video";
        int sel = ui->listWidget->currentRow();
        if(this->playList->removeMedia(sel))
        {
            QListWidgetItem *nm = ui->listWidget->currentItem();
            delete_music(nm->text());
            ui->listWidget->takeItem(sel);
            return ;
        }
        QMessageBox::information(this,"删除文件","删除失败");
    });

    this->player->setVideoOutput(ui->widget);   //设置视频输出控件

    connect(this->player,&QMediaPlayer::durationChanged,this,[=](qint64 duration){  //获取视频音频时长
        qDebug() << duration;
        if(duration !=0 )
        {
            this->music_current_play_time = 0;
            ui->horizontalSlider->setValue(0);
            //简单查看一下时间的转换
            QDateTime music_time = QDateTime::fromMSecsSinceEpoch(duration);
            QTime time = QTime::fromMSecsSinceStartOfDay(duration);

            qDebug() <<time.hour()<<":"<<time.minute()<<":"<<time.second();
            qDebug() << music_time.toString("mm:ss");
            qDebug() << music_time.toString();

            ui->horizontalSlider->setMaximum(duration/1000);    //设置滑块的最大值为音视频的时长
            // 显示总时长文本
            ui->music_alltime_label->setText(QString("%1:%2").arg(duration/1000/60, 2, 10, QChar('0')).arg(duration/1000%60,2, 10, QChar('0')));
            ui->music_playcurrentime_label->setText(QString("00:00"));
        }
    });


//    connect(ui->horizontalSlider,&QSlider::valueChanged,this,[=](int value){
//        this->player->setPosition(value);
//        qDebug()<<"the current progress is:" << ui->horizontalSlider->value();
//    });

    this->http = new QNetworkAccessManager(this);
    connect(ui->search_btn,&QPushButton::clicked,this,&MusicPlayer::search_music_request);  //点击搜索发送请求

    connect(this->http,&QNetworkAccessManager::finished,this,&MusicPlayer::handle_request);

}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}

void MusicPlayer::search_music_request()
{
    if(ui->music_search_line->text() == "")
    {
        QMessageBox::critical(this,"错误","搜索栏为空");
        return;
    }
    QString url = "https://dataiqs.com/api/kgmusic/?msg=" + ui->music_search_line->text() + "&type=&n=0";
    this->http->get(QNetworkRequest(QUrl(url)));
}

void MusicPlayer::handle_request(QNetworkReply *reply)  //处理返回来的Json数据
{
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(status != 200)
    {
        QMessageBox::critical(this,"网络错误","网络连接中断");
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(doc.isObject())
    {
        QJsonObject obj = doc.object();
        QJsonObject obj_data = obj["data"].toObject();

        QString song_name =  obj_data["name"].toString();
        qDebug()<<obj_data["name"].toString();

        //QString duration = obj_data["duration"].toString(); //获取时长
        //QTime duration_time = QTime::fromString(duration);  //将时长转换为QTime，但是有点问题是转换的时间变成了小时:分钟不是分钟:秒
        //qDebug()<<duration_time.minute()<<" "<<duration_time.hour();
        QString song_url = obj_data["song_url"].toString();
        QString mv_url = obj_data["mv_url"].toString();

        if(mv_url == "")
        {
            qDebug() << "mv_url is null" ;
        }

        if(ui->search_mode->currentText() == "搜索歌曲")
        {
            if(song_url == "付费歌曲暂时无法获取歌曲下载链接")
            {
                if(mv_url != "")
                {
                    QMessageBox::information(this,"警告","付费歌曲，不提供播放支持,有mv可供播放");
                    return;
                }
                QMessageBox::information(this,"警告","付费歌曲，不提供播放支持");
                return;
            }

            this->playList->addMedia(QMediaContent(QUrl(song_url)));    //将歌曲url加入到playlist中

            ui->listWidget->addItem(song_name + ".mp3");
            data_insert_base(song_name,song_url);   //加入列表的同时放入数据库之中
        }
        else    //搜索mv
        {
            if(mv_url == "")
            {
                QMessageBox::critical(this,"错误","此歌曲无mv");
                return;
            }
            this->playList->addMedia(QMediaContent(QUrl(mv_url)));
            ui->listWidget->addItem(song_name + ".mp4");
            data_insert_base(song_name,mv_url);
        }
    }
}

void MusicPlayer::sqlite_init()
{
    QSqlDatabase db ;/*= QSqlDatabase::addDatabase("QSQLITE");*/
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        qDebug() << "默认连接";
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        qDebug() << "新连接";
        db= QSqlDatabase::addDatabase("QSQLITE"); //连接数据库
    }
    db.setDatabaseName("user.db");
    if(!db.open())
    {
        qDebug() <<"open database error";
        return;
    }
    QSqlQuery query;
    QString create_musicsql = QString("create table if not exists music_table("
                                      "username text not null,"
                                      "music_name text not null,"
                                      "music_url text not null)");

    if(!query.exec(create_musicsql))
    {
        qDebug() << "error create table";
    }
    else
    {
        qDebug() << "create table success";
    }
}
//将查到的歌的数据插入到数据库数据表之中
void MusicPlayer::data_insert_base(QString music_name, QString music_url)
{
//    sqlite_init();
    qDebug()<<"uname:"<<this->currentUname<<" musicname:"<<music_name<<" musicurl:"<<music_url;
    QString sql = QString("insert into music_table(username,music_name,music_url) values("
                          "'%1','%2','%3')").arg(this->currentUname).arg(music_name).arg(music_url);
    QSqlQuery query;
    if(query.exec(sql))
    {
        qDebug()<< "insert music success";
    }
    else
    {
        qDebug()<< "insert music error";
    }
}
//从数据库中查询出当前用户的历史歌曲记录
void MusicPlayer::query_database_music()
{
//    sqlite_init();
    QString sql = QString("select * from music_table where username = '%1'").arg(this->currentUname);
    QSqlQuery query(sql);

    if(query.exec())
    {
        while (query.next()) {
               // 获取每一列的数据
               QString username = query.value(0).toString(); // username是第一列
               QString musicName = query.value(1).toString(); // music_name是第二列
               QString musicUrl = query.value(2).toString(); // music_url是第三列

               ui->listWidget->addItem(musicName);
               this->playList->addMedia(QMediaContent(QUrl(musicUrl)));
               // 在这里使用获取到的数据
               // 例如，打印到控制台
               qDebug() << "Username:" << username;
               qDebug() << "Music Name:" << musicName;
               qDebug() << "Music URL:" << musicUrl;
           }
    }
    else
    {
        // 处理查询错误
           qDebug() << "Error executing query:";
    }
}
//删除音乐从数据库中移除
void MusicPlayer::delete_music(const QString & music_name)
{
//    sqlite_init();
    QString sql = QString("delete from music_table where music_name = '%1'").arg(music_name);
    QSqlQuery query(sql);
    if(query.exec())
    {
        qDebug()<< "delete success:music";
    }
    else
    {
        qDebug()<<"delete error:music";
    }
}

void MusicPlayer::setcurrentUname(QString currentUname)
{
    this->currentUname = currentUname;
}
