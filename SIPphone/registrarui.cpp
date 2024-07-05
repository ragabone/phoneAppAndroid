
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QTimer>
#include <QDir>
#include <ESip.h>
#include <unistd.h>
#include <stdlib.h>
#include "registrarui.h"
#include "xmlwriter.h"
#include <qstandardpaths.h>
#include "myhttp.h"
#include "combobox.h"
registrarUI *self;
QObject *GBLObjectQML;
#define REGISTRATION_INTERVAL_TIME 30000
//registrarUI::registrarUI()
registrarUI::registrarUI(QObject *object,QQmlContext *ctxt):m_pObjectQML(object),mRegistering(false),mCtxt(ctxt),mGetMap(true),mRequestSent(false)
{
    bool status;
    m_pCombo = new ComboBoxModel;
    QStringList ServerList;
    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //QPushButton *regButton = new QPushButton(tr("Register Client"));
    m_pClientName = new char [100];
    m_pDomain = new char [100];
    strcpy(m_pDomain,"192.168.1.200");
    strcpy(m_pClientName,"Concierge");
    self=this;
    GBLObjectQML=m_pObjectQML;
    m_pHttpReq = new myHttp();
    m_pHttpReq->start();
    initSipStack((int)5060, &registrarUI::registrarCallbackFxn);
    //setClient(QString("ClientAndroid"));
    //setDomain(QString("192.168.1.200"));
    m_pTimer = new QTimer();
    m_pTimer->start(5000);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(callRegistration()));
    connect(this, SIGNAL(setGetBuildingList()), this, SLOT(getBuildingList()));
    connect(this, SIGNAL(setGetLocalMap()), this, SLOT(getLocalMap()));
    connect(this, SIGNAL(setGetEntMap()), this, SLOT(getEntMap()));




    ServerList.append("Local");
    m_pCombo->setComboList(ServerList);


    mCtxt->setContextProperty("myRegisterModel",QVariant::fromValue(m_pCombo));


#ifdef ANDROID
    QDir location(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
#else
    QDir location(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
#endif
    QDir root("/");

    mRegConfigFile = location.absolutePath() + "/register.xml";
    //status= QFile::setPermissions(mRegConfigFile,QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);

//    QFile outFile(mRegConfigFile);
//    status= outFile.open(QIODevice::WriteOnly);
//    outFile.close();
   // QFile::remove(mRegConfigFile);
    if(QFile(mRegConfigFile).exists() == false){

        if(location.exists()==false){
            status=root.mkpath(location.absolutePath());
        }


        status= QFile::copy(":/data/register.xml", mRegConfigFile);
        status= QFile::setPermissions(mRegConfigFile,QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);



    }
    //QFile(mRegConfigFile).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    GBLObjectQML->setProperty("regSettingFile",mRegConfigFile);

    XmlWriter myXmlwriter(mRegConfigFile);
    QString val = myXmlwriter.findParamText("Name");
    if(!val.isEmpty())
        setClient(val);
    val = myXmlwriter.findParamText("Domain");
    if(!val.isEmpty())
        this->setDomain(val);




}



void registrarUI::callRegistration(){
    //qDebug() << "Registering" ;

    registerClient();
    m_pTimer->setInterval(REGISTRATION_INTERVAL_TIME);

}


void registrarUI::setRegistrarAddress(QString ip){

    strcpy(m_pDomain,ip.toLatin1().data());

}

void registrarUI::setRegisterName(QString name){
    strcpy(m_pClientName,name.toLatin1().data());

}

void registrarUI::registerClient(){
    if(mRegistering == false){
        GBLObjectQML->setProperty("indicator", "grey");
        GBLObjectQML->setProperty("regState", "Registering");
        //mRegistering = true;
        doRegister(m_pClientName, m_pDomain, (int)5060);
        //mRegistering = true;
    }
}

void registrarUI::setDomain(const QString msg){
    strcpy(m_pDomain,msg.toStdString().c_str());
    qDebug() << "Domain Set " << m_pDomain;
    XmlWriter myXmlwriter(mRegConfigFile);
    myXmlwriter.findReplaceText(tr("Domain"),msg);

}

void registrarUI::setClient(const QString msg){
    strcpy(m_pClientName,msg.toStdString().c_str());
    qDebug() << "mClientName Set " << m_pClientName;
    XmlWriter myXmlwriter(mRegConfigFile);
    myXmlwriter.findReplaceText(tr("Name"),msg);

}




void registrarUI::registrarCallbackFxn(int *status){
    int result = *status;
    self->mRegistering = false;

    if(result<0){
        qDebug("Registrar Callback fail %d",result);
        GBLObjectQML->setProperty("indicator", "red");
        GBLObjectQML->setProperty("regState", "Not Registered");
        //QMetaObject::invokeMethod( GBLObjectQML, "clrSite");


    }
    else{
        //qDebug("Registrar Callback %d",result);
        GBLObjectQML->setProperty("indicator", "green");
        GBLObjectQML->setProperty("regState", "Registered");
        //self->m_pTimer->setInterval(REGISTRATION_INTERVAL_TIME);
        if(self->mRequestSent==false){

//            GBLObjectQML->setProperty("indicator", "yellow");
//            GBLObjectQML->setProperty("regState", "Get Maps");
            self->sendGetBuildingList();

            self->mRequestSent=true;

        }
        else{
            if(self->mGetMap == false){
                GBLObjectQML->setProperty("indicator", "green");
                GBLObjectQML->setProperty("regState", "Registered");
                //self->m_pTimer->setInterval(REGISTRATION_INTERVAL_TIME);
                //self->mRequestSent=false;
                self->mRegistering = false;

            }
        }


    }
}
void registrarUI::sendGetBuildingList(){
     emit setGetBuildingList();
}

void registrarUI::sendGetLocalMap(){
     emit setGetLocalMap();
}

void registrarUI::sendGetEntMap(){
     emit setGetEntMap();
}

void registrarUI::addSite(QStringList textList){
    qDebug() << "adding Site";
    QMetaObject::invokeMethod( GBLObjectQML, "clrSite");
    usleep(10000);
    foreach (QString text,textList){
        QMetaObject::invokeMethod( GBLObjectQML, "addSiteBuffer", Q_ARG(QVariant, text ) );
    }


}

void registrarUI::searchCallBack(int status, QString userData){
    if(status==0){
        //success
        //self->addSite(//parse file);

        QFile file(userData);
        QStringList ServerList;
        ServerList.append("Local");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            //QMessageBox::warning(this, tr("File error"), tr("Failed to open\n%1").arg(filename));
            return;
        }
        mNatServerList.clear();
        while(true){
            QString line =  file.readLine();
            natServer server[4];
            int i=0;
            if(line.isEmpty())
                break;
            else{
                char myLine[120];
                strcpy(myLine,line.toLatin1().data());
                sscanf(myLine,"%s %s %s",server[i].name,server[i].ip,server[i].port);
                mNatServerList.append(server[i]);
                ServerList.append(server[i].name);
                i++;
                if(i>=4)
                    break;
            }

        }

         m_pCombo->setComboList(ServerList);
         if(self->mGetMap){
             self->sendGetLocalMap();
         }
        //mCtxt->setContextProperty("myRegisterModel", QVariant::fromValue(mServerList));
        //addSite(ServerList);
    }


}


void registrarUI::mapCallBack(int status, QString userData){
    if(status==0){
        //success
        //self->addSite(//parse file);

        QFile file(userData);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            //QMessageBox::warning(this, tr("File error"), tr("Failed to open\n%1").arg(filename));
            return;
        }
        QString line =  file.readLine();
        sendGetEntMap();
    }

}

void registrarUI::mapCallBackEnt(int status, QString userData){
    if(status==0){
        //success


        QFile file(userData);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            //QMessageBox::warning(this, tr("File error"), tr("Failed to open\n%1").arg(filename));
            return;
        }
        QString line =  file.readLine();
        //mRequestSent=false;
        mGetMap=false;
        mRegistering = false;
        //m_pTimer->setInterval(REGISTRATION_INTERVAL_TIME);
    }


}


bool registrarUI::getSiteDetails(char *name,char *ip,int *port){
    for(int i=0;i<mNatServerList.count();i++){
        if(!strcmp(name,mNatServerList.at(i).name)){
            strcpy(ip,mNatServerList.at(i).ip);
            *port = atoi(mNatServerList.at(i).port);
            return true;
        }

    }
    return false;

}

void registrarUI::getBuildingList(){
    QString sBuf = tr("http://") + m_pDomain + tr("/cgi-bin/natPortData.txt");
    QMetaObject::invokeMethod(m_pHttpReq, "startRequestNatDetails",Qt::QueuedConnection,Q_ARG(void *, this ),Q_ARG(QString, sBuf));

}

void registrarUI::getLocalMap(){
    QString sBuf = tr("http://") + m_pDomain + tr("/cgi-bin/data.csv");
    QMetaObject::invokeMethod(m_pHttpReq, "startMapRequest",Qt::QueuedConnection,Q_ARG(void *, this ),Q_ARG(QString, sBuf));

}

void registrarUI::getEntMap(){
    QString sBuf = tr("http://") + m_pDomain + tr("/cgi-bin/dataEntrance.csv");
    QMetaObject::invokeMethod(m_pHttpReq, "startMapRequestEnt",Qt::QueuedConnection,Q_ARG(void *, this ),Q_ARG(QString, sBuf));

}
