#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_uploadButton_clicked()
{
    file = new QFile("C:\\Users\\cxw\\Downloads\\nes_16khz.wav");
    file->open(QIODevice::ReadOnly);
    QByteArray byte_file = file->readAll();

    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    QString path = "ftp://" + serverIp + "/home/test/";
    //QUrl url("ftp://192.168.2.12/home/test/" + ui->lineEdit->text()+ ".wav");
    QUrl url(path + ui->lineEdit->text()+ ".wav");
    url.setPort(portNumebr);
    url.setUserName(userName);
    url.setPassword(passWord);

    QNetworkRequest request(url);
    reply = accessManager->put(request, byte_file);

    //ui->progressBar = new QProgressBar();
    ui->progressBar->setValue(0);
    ui->progressBar->show();

    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));

}

void MainWindow::on_downloadButton_clicked()
{
    //file = new QFile(QApplication::applicationDirPath() + "C:\\Users\\cxw\\Downloads\\" + ui->lineEdit->text() + ".mp3");
    file = new QFile(QApplication::applicationDirPath() + "/" + ui->lineEdit->text() + ".mp3");
    file->open(QIODevice::WriteOnly);

    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    //QString serverIp = "192.168.2.12";
    QString path = "ftp://" + serverIp + "/home/test/xxx.mp3";
    QUrl url(path);
    url.setPort(portNumebr);
    url.setUserName(userName);
    url.setPassword(passWord);

    QNetworkRequest request(url);
    reply = accessManager->get(request);

    //progressBar = new QProgressBar();
    ui->progressBar->setValue(0);
    ui->progressBar->show();

    connect((QObject *)reply, SIGNAL(readyRead()), this, SLOT(readContent()));
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
}

void MainWindow::readContent()    //下载时向本地文件中写入数据
{
    file->write(reply->readAll());
}

void MainWindow::replyFinished(QNetworkReply*)    //删除指针，更新和关闭文件
{
    if(reply->error() == QNetworkReply::NoError)
        {
            reply->deleteLater();
            file->flush();
            file->close();
        }
        else
        {
            QMessageBox::critical(NULL, tr("Error"), "Failed!!!");
        }
}

void MainWindow::loadProgress(qint64 bytesSent, qint64 bytesTotal)    //更新进度条
{
       qDebug() << "loaded" << bytesSent << "of" << bytesTotal;
       ui->progressBar->setMaximum(bytesTotal); //最大值
       ui->progressBar->setValue(bytesSent);  //当前值
}

void MainWindow::loadError(QNetworkReply::NetworkError)    //传输中的错误输出
{
     qDebug()<<"Error: "<<reply->error();
}


void MainWindow::on_setPara_clicked()
{
    //ftp parameter
    serverIp = ui->serverIP->text();
    portNumebr = ui->port->text().toInt();
    userName = ui->userName->text();
    passWord = ui->passWord->text();

    qDebug()<<"serverIp："<<serverIp<<endl;
    qDebug()<<"port："<<portNumebr<<endl;
    qDebug()<<"userName："<<userName<<endl;
    qDebug()<<"passWord："<<passWord<<endl;

    //server ip
    if(serverIp == NULL)
    {
        qDebug()<<"input serverIp invalid"<<endl;
        return ;
    }

    if(portNumebr == NULL)
    {
        qDebug()<<"input portNumebr invalid"<<endl;
        return ;
    }

    if(userName == NULL)
    {
        qDebug()<<"input userName invalid"<<endl;
        return ;
    }

    if(passWord == NULL)
    {
        qDebug()<<"input passWord invalid"<<endl;
        return ;
    }

    ui->setPara->setEnabled(false);
}
