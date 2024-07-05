#include <QtCore>
#include "myhttp.h"
#include <QFile>
#include <QFileInfo>
#include <qstandardpaths.h>
#include "entryctrl.h"
#include "registrarui.h"

void myHttp::run()
{

    exec();
}
myHttp::myHttp():mIsfinished(0)
{

    m_pQnam = new QNetworkAccessManager;
    moveToThread(this);



}


void myHttp::startRequest(void *parent,QString myUrl)
{
    m_pParent=(entryCtrl *)parent;
    downloadDirectory = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    mfileName = downloadDirectory + "/userData.txt";
    QFile::remove(mfileName);
    file = new QFile(mfileName);
    file->open(QIODevice::WriteOnly);
    url.setUrl(myUrl);
    httpRequestAborted = false;
    myReq.setUrl(url);
    reply = m_pQnam->get(myReq);

    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));

}

void myHttp::startMapRequest(void *parent,QString myUrl)
{
     m_pParentNat=(registrarUI *)parent;
    downloadDirectory = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    mfileName = downloadDirectory + "/data.csv";
    QFile::remove(mfileName);
    file = new QFile(mfileName);
    file->open(QIODevice::WriteOnly);
    url.setUrl(myUrl);
    httpRequestAborted = false;
    myReq.setUrl(url);
    reply = m_pQnam->get(myReq);

    connect(reply, SIGNAL(finished()), this, SLOT(httpMapFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));

}

void myHttp::startMapRequestEnt(void *parent,QString myUrl)
{
     m_pParentNat=(registrarUI *)parent;
    downloadDirectory = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    mfileName = downloadDirectory + "/dataEntrance.csv";
    QFile::remove(mfileName);
    file = new QFile(mfileName);
    file->open(QIODevice::WriteOnly);
    url.setUrl(myUrl);
    httpRequestAborted = false;
    myReq.setUrl(url);
    reply = m_pQnam->get(myReq);

    connect(reply, SIGNAL(finished()), this, SLOT(httpMapEntFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));

}

void myHttp::startRequestNatDetails(void *parent,QString myUrl)
{
    m_pParentNat=(registrarUI *)parent;
    downloadDirectory = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    mfileName = downloadDirectory + "/nat.txt";
    QFile::remove(mfileName);
    file = new QFile(mfileName);
    file->open(QIODevice::WriteOnly);
    url.setUrl(myUrl);
    httpRequestAborted = false;
    myReq.setUrl(url);
    reply = m_pQnam->get(myReq);

    connect(reply, SIGNAL(finished()), this, SLOT(httpFinishedNat()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));

}


void myHttp::httpReadyRead()
{
    qint64 ret;
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file){
        QByteArray contents = reply->readAll();
        ret =file->write(contents);
    }
    qDebug("http read ret %d",ret);
}

void myHttp::httpFinishedNat( )
{
    QFileInfo fi;
    mIsfinished = 0;
    if (file) {
        fi.setFile(file->fileName());
        file->close();
        delete file;
        file = Q_NULLPTR;
    }

    if (httpRequestAborted) {
        reply->deleteLater();
        reply = Q_NULLPTR;
        mIsfinished = -1;

    }

    if (reply->error()) {
        QFile::remove(fi.absoluteFilePath());
        reply->deleteLater();
        reply = Q_NULLPTR;

         mIsfinished = -1;

    }

    //tell inform parent
    this->m_pParentNat->searchCallBack(mIsfinished,mfileName);
    //delete m_pQnam;
    delete file;
    //this->exit();
    //QStringList entryCtrl::readFile(QString fullFileName);
}



void myHttp::httpFinished( )
{
    QFileInfo fi;
    mIsfinished = 0;
    if (file) {
        fi.setFile(file->fileName());
        file->close();
        delete file;
        file = Q_NULLPTR;
    }

    if (httpRequestAborted) {
        reply->deleteLater();
        reply = Q_NULLPTR;
        mIsfinished = -1;

    }

    if (reply->error()) {
        QFile::remove(fi.absoluteFilePath());
        reply->deleteLater();
        reply = Q_NULLPTR;

         mIsfinished = -1;

    }

    //tell inform parent
    this->m_pParent->searchCallBack(mIsfinished,mfileName);
    //delete m_pQnam;
    delete file;
    //this->exit();
    //QStringList entryCtrl::readFile(QString fullFileName);
}

void myHttp::httpMapFinished( )
{
    QFileInfo fi;
    mIsfinished = 0;
    if (file) {
        fi.setFile(file->fileName());
        file->close();
        delete file;
        file = Q_NULLPTR;
    }

    if (httpRequestAborted) {
        reply->deleteLater();
        reply = Q_NULLPTR;
        mIsfinished = -1;

    }

    if (reply->error()) {
        QFile::remove(fi.absoluteFilePath());
        reply->deleteLater();
        reply = Q_NULLPTR;

         mIsfinished = -1;

    }

    //tell inform parent
    this->m_pParentNat->mapCallBack(mIsfinished,mfileName);
    //delete m_pQnam;
    delete file;
    //this->exit();
    //QStringList entryCtrl::readFile(QString fullFileName);
}

void myHttp::httpMapEntFinished( )
{
    QFileInfo fi;
    mIsfinished = 0;
    if (file) {
        fi.setFile(file->fileName());
        file->close();
        delete file;
        file = Q_NULLPTR;
    }

    if (httpRequestAborted) {
        reply->deleteLater();
        reply = Q_NULLPTR;
        mIsfinished = -1;

    }

    if (reply->error()) {
        QFile::remove(fi.absoluteFilePath());
        reply->deleteLater();
        reply = Q_NULLPTR;

         mIsfinished = -1;

    }

    //tell inform parent
    this->m_pParentNat->mapCallBackEnt(mIsfinished,mfileName);
    //delete m_pQnam;
    delete file;
    //this->exit();
    //QStringList entryCtrl::readFile(QString fullFileName);
}

