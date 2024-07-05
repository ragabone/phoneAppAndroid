#ifndef SIPSTACKIF_H
#define SIPSTACKIF_H


#include "callstates.h"

#include <QObject>
#include <QTimer>

class QObject;
class audioHandle;
class audioHandleThread;
class entryCtrl;
class registrarUI;

enum registeredState{
    ISREGISTERED = 0,
    NOTREGISTERED
};

class sipStackIf: public QObject
{
    Q_OBJECT
public:
    sipStackIf(QObject *object,registrarUI *registrar);
    ~sipStackIf();
    static void sipCallbackFxn(callState,char *);
    void setTextUI(QString text);
    void setTextUIInfo(QString text);
    void makeSipCall(QString name);
    void setProxyAddress(char *arg);
    void addSite(QString text);
    void setCallUI(bool val);



signals:
    void clearClrUISig();

public:
    Q_INVOKABLE void answerCurrentCall(void);
    Q_INVOKABLE void endCurrentCall(void);
    Q_INVOKABLE void endCurrentCallCancel(void);
    Q_INVOKABLE void endCurrentCallDecline(void);
    Q_INVOKABLE void endCurrentCallTimeout(void);
    Q_INVOKABLE void endCurrentCallMsg(void);
    Q_INVOKABLE void opendoorCurrentCall(void);
    Q_INVOKABLE void make_call(QString,QString,QString);
public:
    audioHandle *mAudio;
    entryCtrl *m_pEntryCtrl;
    bool mIsEntrance;
    bool mTimeOutSent;
public slots:
    void setTimeOut();

private slots:
    void clearUI();
    void setClearUI();

private:
    audioHandleThread *mAudioHandle;
    QString mSite;
    QString mType;
    registrarUI *m_pRegistrar;
    QTimer *m_pTimer;
    int mTimerId;
    callState mState;
    int mTimeOut;
    //QTimer *m_pTimer;
};

#endif // SIPSTACKIF_H
