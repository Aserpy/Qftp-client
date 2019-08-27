#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QProgressBar>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_uploadButton_clicked();
    void on_downloadButton_clicked();

    void readContent();
    void replyFinished(QNetworkReply*);
    void loadError(QNetworkReply::NetworkError);
    void loadProgress(qint64 bytesSent,qint64 bytesTotal);

    void on_setPara_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkReply *reply;
    //QProgressBar *progressBar;
    QFile *file;

    //ftp parameter
    QString serverIp;
    int portNumebr;
    QString userName;
    QString passWord;
};

#endif // MAINWINDOW_H
