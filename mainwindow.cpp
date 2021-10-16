#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mNetManager = new QNetworkAccessManager(this);
    connect(mNetManager, &QNetworkAccessManager::finished,
            this, &MainWindow::handleNetFinished);
//openFile("X:/test.h");

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openFile(const QString &filename)
{
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        while (!f.atEnd())
        {
            QByteArray line = f.readLine();
            qDebug("%s", qPrintable(line));
        }
    }
    else
    {
        qDebug("File open error");
    }

}

void MainWindow::openUrl(const QString &url)
{
    mNetManager->get(QNetworkRequest(QUrl(url)));
}

void MainWindow::testJson(QByteArray data)
{

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    QString url = obj["url"].toString();
    QString facts = obj["facts"].toString();


    qDebug() << url;
    qDebug() << facts;


    ui->url_label->setText(url);
    ui->facts_label->setText(facts);

    //https://axoltlapi.herokuapp.com/ (<- Json API Link)


}

void MainWindow::handleNetFinished(QNetworkReply *reply)
{
    if (reply->error()==QNetworkReply::NoError)
    {

        QByteArray data = reply->readAll();
        testJson(data);
    }
    else
        qDebug("Network error");
}

void MainWindow::on_pushButtonGetUrl_clicked()
{
    openUrl(ui->lineEditUrl->text());

}

