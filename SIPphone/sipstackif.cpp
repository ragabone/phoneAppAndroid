#include <QQuickView>
#include <QTimer>
#include <ESip.h>
#include <QThread>
#include "sipstackif.h"
#include "tcpclientRead.h"
#include "audiohandle.h"
#include "entryctrl.h"
#include "registrarui.h"

extern QObject *GBLObjectQML;

static sipStackIf *self;


sipStackIf::sipStackIf(QObject *object, registrarUI *registrar):m_pRegistrar(registrar),mTimeOut(10),mTimeOutSent(false),mIsEntrance(false)
{
    //QThread *objThread = new QThread;

    self=this;
     self->mState = DICONNECTED;
    initSipCallback(&sipStackIf::sipCallbackFxn);
    m_pTimer = new QTimer();
   // m_pTimer->setSingleShot(true);
    connect(this, SIGNAL(clearClrUISig()), this, SLOT(setClearUI()));
    m_pEntryCtrl = new entryCtrl(this);
    mAudioHandle = new audioHandleThread;
    mAudioHandle->start();

    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(setTimeOut()));
    m_pTimer->start(5000);
    //mAudio = new audioHandle();
    //mAudio->start();
    //connect(objThread,SIGNAL(started()),this,SLOT(mThreadStarted()));

}


sipStackIf::~sipStackIf()
{

}


void sipStackIf::setProxyAddress(char *arg){
    sipSetProxyAddress(arg);
}
 void  sipStackIf::setTimeOut(){
     if(self->mState != DICONNECTED){
         if(self->mTimeOut-- <= 0){
            if(self->mTimeOutSent == false){
                if(self->mState == CALLANSWERED)
                    endCurrentCall();
                else
                    endCurrentCallTimeout();
                self->mTimeOutSent=true;
            }
         }
     }
     else{
         self->mTimeOutSent = false;
         self->mTimeOut = 10;
     }

 }

void sipStackIf::sipCallbackFxn(callState state,char *arg){
    switch (state){
    case DICONNECTED:{
        qDebug() << "sipCallbackFxn::DICONNECTED" ;
        if(self->mState == INCOMINGCALL){
            //QMetaObject::invokeMethod(GBLObjectQML, "stopRingTone");
        }
        if(self->mState == CALLANSWERED){
            //QMetaObject::invokeMethod(self->mAudioHandle->mAudioHandle, "stop_media");

        }
        if(strlen(arg)){
            QMetaObject::invokeMethod(GBLObjectQML, "setBusyDefaultMain", Q_ARG(QVariant, self->mType) );
        }
        else
            QMetaObject::invokeMethod(GBLObjectQML, "setDefaultMain");
        self->mIsEntrance = false;
        self->mState = DICONNECTED;
        self->mTimeOutSent = false;
        self->mTimeOut =10;


        break;
    }
    case INCOMINGCALL:{
        if(self->mState == DICONNECTED){
            self->mState = INCOMINGCALL;
            if(strlen(arg)){
                char *ptr= strstr(arg,"Entrance");
                if(ptr)
                  self->mIsEntrance = true;
               else
                  self->mIsEntrance = false;
            }
            else
                self->mIsEntrance = false;

             QMetaObject::invokeMethod(GBLObjectQML, "setIncoming", Q_ARG(QVariant, arg ) );
//            tcpClientRead tRead;
//            tRead.connectTcp(arg);
//            QMetaObject::invokeMethod(GBLObjectQML, "loadPersonAtDoor");
             QMetaObject::invokeMethod(GBLObjectQML, "playRingTone");
             self->mTimeOut =6;
        }

        break;
    }
    case CALLRINGING:{
        if(self->mState == DICONNECTED){
            self->mState = CALLRINGING;
            QMetaObject::invokeMethod(GBLObjectQML, "setCallRinging");
            self->mTimeOut =6;
        }

        break;
    }
    case INCOMINGCALLTIMEOUT:{
        if(self->mState == INCOMINGCALL){
            self->mState = INCOMINGCALLTIMEOUT;
        }
        break;
    }
    case CALLANSWERED:{
        qDebug() << "CALLANSWERED::startVideoStreamUI" ;
        if(self->mState == INCOMINGCALL){
            qDebug() << "INCOMINGCALL:CALLANSWERED::startVideoStreamUI" ;
            if(self->mIsEntrance){
                QMetaObject::invokeMethod(GBLObjectQML, "setActive");
                //self->setTextUI(tr("Call Answered"));

            }
            else
                QMetaObject::invokeMethod(GBLObjectQML, "setActiveTele");

            //QMetaObject::invokeMethod(GBLObjectQML, "stopRingTone");
            //QMetaObject::invokeMethod(GBLObjectQML, "startVideoStreamUI");
           // QMetaObject::invokeMethod(self->mAudioHandle->mAudioHandle, "start_media");
            //self->mAudioHandle->start_media();
            self->mTimeOut = 8;
            self->mState = CALLANSWERED;
        }
        else if(self->mState == CALLRINGING){
            qDebug() << "CALLRINGING:CALLANSWERED::startVideoStreamUI" ;
            if(self->mIsEntrance)
                QMetaObject::invokeMethod(GBLObjectQML, "setActive");
            else
                QMetaObject::invokeMethod(GBLObjectQML, "setActiveTele");
            self->mTimeOut = 8;
            self->mState = CALLANSWERED;
        }
        break;
    }
    case CALLDECLINED:{
        self->setTextUI("Call Declined");
        self->mTimeOut = 4;
        self->mState = CALLDECLINED;
        break;
    }
    case CALLTIMEOUT:{
        self->setTextUI("Call Timeout");
        self->mTimeOut = 4;
        self->mState = CALLTIMEOUT;
        break;
    }
    case CALLCLEARED:{
        self->setTextUI("Call Cleared");
        self->mTimeOut = 4;
        self->mState = CALLCLEARED;
        break;
    }
    default:
        self->mTimeOut = 4;
        break;

    }
}


void sipStackIf::make_call(QString site, QString type, QString name){
    qDebug() << "sipStackIf::make_call " << name << " Site " << site ;
    mSite=site;
    mType=type;
    if(site == tr("Local")){
       m_pEntryCtrl->beginSearch(name,type);
    }
    else{
        char ipaddress[40];
        int  port;
        int typeCall;

        if(type.contains("Telephone")){
            mIsEntrance = false;
            typeCall=0;
        }
        else{
            typeCall=1;
            mIsEntrance = true;
        }



        if(m_pRegistrar->getSiteDetails(site.toLatin1().data(),ipaddress,&port)){
            setTextUI("Calling " + name);
            setProxyAddress(m_pRegistrar->getRegistrar());
            make_sip_site_call(name.toLatin1().data(),ipaddress,port,typeCall);
        }
        //make_sip_call(site.toLatin1().data(),name.toLatin1().data());
    }

}



void sipStackIf::makeSipCall(QString name){
    //make_sip_call(mSite.toLatin1().data(),name.toLatin1().data());
    mIsEntrance= name.contains("Entrance");

    setCallName(mSite.toLatin1().data(),name.toLatin1().data());


}

void sipStackIf::setCallUI(bool val){

    if(val)
        QMetaObject::invokeMethod( GBLObjectQML, "setCallState" );
    else
        QMetaObject::invokeMethod( GBLObjectQML, "setReadyState" );

}

void sipStackIf::setTextUI(QString text){
    QMetaObject::invokeMethod( GBLObjectQML, "updateUI", Q_ARG(QVariant, text ) );


}
void sipStackIf::addSite(QString text){
    QMetaObject::invokeMethod( GBLObjectQML, "addSite", Q_ARG(QVariant, text ) );

}
void sipStackIf::setTextUIInfo(QString text){
    setTextUI(text);
    emit clearClrUISig();


}

void sipStackIf::setClearUI(){

   QTimer::singleShot(3000, this, SLOT(clearUI()));
}

void sipStackIf::clearUI(){
   setTextUI("");
}

void sipStackIf::opendoorCurrentCall(void){
    qDebug() << "sipStackIf::opendoorCurrentCall" ;
    QMetaObject::invokeMethod(GBLObjectQML, "setDoorFlash");
    sipOpenDoor();

}


void sipStackIf::endCurrentCall(void){
    qDebug() << "sipStackIf::endCurrentCall" ;
    sipEndCall(NORMAL);
    setCallUI(false);

}

void sipStackIf::endCurrentCallTimeout(void){
    qDebug() << "sipStackIf::endCurrentCall" ;
    sipEndCall(TIMEOUT);
    setCallUI(false);

}

void sipStackIf::endCurrentCallDecline(void){
    qDebug() << "sipStackIf::endCurrentCall" ;
    sipEndCall(DECLINE);
    setCallUI(false);

}
void sipStackIf::endCurrentCallCancel(void){
    qDebug() << "sipStackIf::endCurrentCall" ;
    sipEndCall(CANCEL);
    setCallUI(false);

}

void sipStackIf::endCurrentCallMsg(void){
    qDebug() << "sipStackIf::endCurrentCallMsg" ;
    sipEndCallMsg();
    setCallUI(false);

}

void sipStackIf::answerCurrentCall(void){
    qDebug() << "sipStackIf::answerCurrentCall" ;
    sipAnswerCurrentCall();
    setCallUI(false);

}
