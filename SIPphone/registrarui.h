#ifndef REGISTRARUI_H
#define REGISTRARUI_H

#include <QObject>
#include <QTimer>
#include <ESip.h>

class QObject;
class myHttp;
class QQmlContext;
class ComboBoxModel;

class registrarUI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString domain WRITE setDomain)
    Q_PROPERTY(QString client WRITE setClient)


public:
    //registrarUI();
    registrarUI(QObject *object,QQmlContext *);
    static void registrarCallbackFxn(int *status);
    void setDomain(const QString );
    void setClient(const QString );
    void addSite(QStringList text);
    QStringList mServerList;
    ComboBoxModel *m_pCombo;
    void searchCallBack(int status, QString userData);
    void mapCallBack(int status, QString userData);
    void mapCallBackEnt(int status, QString userData);

    bool getSiteDetails(char *name,char *ip,int *port);
    char *getRegistrar(){
        return m_pDomain;
    }

    Q_INVOKABLE void registerClient(void);
    Q_INVOKABLE void setRegistrarAddress(QString ip);
    Q_INVOKABLE void setRegisterName(QString name);
signals:
    void setGetBuildingList();
    void setGetLocalMap();
    void setGetEntMap();
public slots:
    void getBuildingList();
    void getLocalMap();
    void getEntMap();
private slots:
    void callRegistration();

private:
    QObject *m_pObjectQML;
    char *m_pDomain;
    char *m_pClientName;
    bool mRegistering;
    QTimer *m_pTimer;
    QQmlContext *mCtxt;
    QString mRegConfigFile;
    myHttp *m_pHttpReq;
    QList<natServer> mNatServerList;
    bool mGetMap;
    bool mRequestSent;
    void sendGetBuildingList();
    void sendGetLocalMap();
    void sendGetEntMap();
signals:
    void deviceChanged();

};

#endif // REGISTRARUI_H
