#ifndef MYHTTP_H
#define MYHTTP_H

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QThread>

class QFile;
class QNetworkReply;
class QNetworkRequest;
class QThread;
class entryCtrl;
class registrarUI;

class myHttp : public QThread
{
    Q_OBJECT
public:
    myHttp();
    void run();

    int isFinished(){
        return mIsfinished;
    }
public slots:
    void startRequest(void *parent,QString myUrl);
    void startRequestNatDetails(void *parent,QString myUrl);
    void startMapRequest(void *parent,QString myUrl);
    void startMapRequestEnt(void *parent,QString myUrl);
private slots:
    void httpFinished();
    void httpFinishedNat();
    void httpMapFinished();
    void httpMapEntFinished();
    void httpReadyRead();
private:
    QUrl url;
    QNetworkAccessManager *m_pQnam;
    QNetworkReply *reply;
    QNetworkRequest myReq;
    QFile *file;
    entryCtrl *m_pParent;
    registrarUI *m_pParentNat;
    QString mfileName;
    bool httpRequestAborted;
    QString downloadDirectory;
    int mIsfinished;
};

#endif // MYHTTP_H
