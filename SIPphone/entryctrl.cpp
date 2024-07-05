#include <QString>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <qstandardpaths.h>
#include <unistd.h>
#include "entryctrl.h"
#include "sipstackif.h"

#include "myhttp.h"
#include "numplan.h"

static char sem_name_setproxy[] = "SEMSETPROXY" ;
static char sem_name_setproxyresp[] = "SEMSETPROXYRESP" ;
#define LOCALREGISTRARFILE "/IgnitionPro/web/cgi-bin/registrarIP.cfg"

entryCtrl::entryCtrl(sipStackIf *parent):mNumGroups(0),mNumHighRiseGroups(0),mGroupName("FlatStarting Now"){
    m_pParent = (sipStackIf*)parent;
    m_pHttpReq = new myHttp();
    m_pHttpReq->start();
}

QStringList entryCtrl::searchEntries(QString user){
    int i=0;
    QString groupName;
    QStringList tmp,retList;
    tmp=user.split("-");
    groupName=tmp[0];

    while((!mGroup[i].entries.isEmpty()) && i<mNumGroups){

        if(mGroup[i].groupName.contains(groupName)){
            bool res;
            int ref1;
            int ref2;
            int cnt;
            char dump[100];
            QByteArray textline;
            const char *text;

            qDebug() << "searchEntries: is a searching " + groupName;
            qDebug() << "searchEntries: IN " + mGroup[i].groupName;
            QRegExp rx("(\\d+)");
            rx.indexIn(mGroup[i].groupName);


            qDebug() << "ref1 " + rx.cap(1);

            if(groupName == rx.cap(1)){
                qDebug("searchEntries: Match Found");
                retList = mGroup[i].entries;
                //Used for information Line edit
                retList << mGroup[i].groupName;
                setProxyAddress(readRegistrarFile());
                break;
            }
        }
        i++;
    };
    return retList;
}
void entryCtrl::clrAllEntries(){
    for(int i=0;i<mNumGroups;i++){
        while(!mGroup[i].entries.isEmpty())
            mGroup[i].entries.removeFirst();
    }
    mNumGroups=0;
}

int entryCtrl::addEntry(QString user){
    int i=0;
    int ret=0;
    bool added = false;
    QString groupName;
    QStringList tmp,retList;
    tmp=user.split("-");
    groupName=tmp[0];
    if(groupName.contains("Entrance",Qt::CaseInsensitive))
        return ret;
    while((!mGroup[i].entries.isEmpty()) && i<mNumGroups){
        if(mGroup[i].groupName.contains(groupName)){

            mGroup[i].entries << user;
            added = true;
            break;
        }
        i++;
    };
    if(added == false){
        if(i<MAXENTRIES){
            mGroup[i].entries << user;
            mGroup[i].groupName = groupName;
            ret=1;
            mNumGroups++;
        }
        else
            ret = -1;
    }
    return ret;
}

void entryCtrl::searchCallBack(int status, QString userData){
    if(status==0){
        //success
        QStringList ret = searchHighRiseEntries(userData);
        if(ret.length()){
            //found entry - make Call
            m_pParent->setCallUI(true);
            m_pParent->setTextUI("Calling " + mGroupName);

            //for(int i=0;i<ret.length();i++)
            m_pParent->makeSipCall(ret[0]);

        }
        else{
            //no entry inform UI
            m_pParent->setTextUIInfo("No Entry Found");
        }
    }
    else{
        // inform UI Failed call
        m_pParent->setTextUIInfo("No Entry Found");
    }


}


//void entryCtrl::beginSearch(QString user){
//    numPlan lNumplan;
//    QString ret;
//    QString sBuf = "http://192.168.1.2";
//    QString sBufappend = "/cgi-bin/userData.txt";
//    QStringList tmp;



//    ret=lNumplan.lookupNum(user.toLatin1().data());
//    if(ret.isEmpty()){
//        if(user.length()!=4){
//            searchCallBack(1,ret);
//            return;
//        }
//         QString floor = user.left(2);
//         QString entrance = user.right(2);
//        //check for entrance
//        if((entrance.toInt()>16) && (entrance.toInt()<21)){

//            int val = entrance.toInt()-16;
//            mGroupName="Entrance" + QString::number(val);
//            QString proxyAddress = "192.168.1.2" + floor;
//            setProxyAddress(proxyAddress);
//            sBuf.append(floor + sBufappend);
//            QMetaObject::invokeMethod(m_pHttpReq, "startRequest",Qt::QueuedConnection,Q_ARG(void *, this ),Q_ARG(QString, sBuf));
//        }
//        else
//           searchCallBack(1,ret);
//    }
//    else{

//        QString floor = ret.left(2);

//        QStringList tmp=   user.split("-");


//        mGroupName=tmp[0];

//        QString proxyAddress = "192.168.1.2" + floor;
//        setProxyAddress(proxyAddress);
//        sBuf.append(floor + sBufappend);
//        QMetaObject::invokeMethod(m_pHttpReq, "startRequest",Qt::QueuedConnection,Q_ARG(void *, this ),Q_ARG(QString, sBuf));
//    }

//}

void entryCtrl::beginSearch(QString user,QString type){

    QString ret;
    QString sBuf = "http://192.168.1.2";
    QString sBufappend = "/cgi-bin/userData.txt";
    QStringList tmp;

    if(type == tr("Telephone")){
        QString mfileName = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/data.csv";
        numPlan lNumplan(mfileName.toLatin1().data(),16);
        ret=lNumplan.lookupNum(user.toLatin1().data());
    }
    else{
        QString mfileName = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/dataEntrance.csv";
        numPlan lNumplan(mfileName.toLatin1().data(),4);
        ret=lNumplan.lookupNum(user.toLatin1().data());


    }
    if(ret.isEmpty()){
        searchCallBack(1,ret);
    }
    else{

        QString floor = ret.left(2);

        if(type == tr("Telephone")){
            QStringList tmp=   user.split("-");
            mGroupName=tmp[0];
        }
        else{
            QString entrance = ret.right(2);
            //check for entrance
            mGroupName="Entrance" + QString::number(entrance.toInt());

        }


        QString proxyAddress = "192.168.1.2" + floor;
        setProxyAddress(proxyAddress);
        sBuf.append(floor + sBufappend);
        QMetaObject::invokeMethod(m_pHttpReq, "startRequest",Qt::QueuedConnection,Q_ARG(void *, this ),Q_ARG(QString, sBuf));
    }


}



QStringList entryCtrl::searchHighRiseEntries(QString fullFileName){


    QStringList retList;
    QStringList mAllUsers=readFile(fullFileName);
    qDebug("readFile %d",mAllUsers.size());
    int m_numUsers = mAllUsers.size();
    if(m_numUsers == 0)
        return retList;
    qDebug("Clearing Entries");
    clrAllHighRiseEntries();

    for(int i=0;i<m_numUsers;i++){
        qDebug("Creating Groups %d",m_numUsers);
        createGroups(mAllUsers[i]);

    }

    int i=0;

    qDebug() << "searchEntries: is a searching 1 " << mGroupHighRise[i].entries;
    qDebug() << "searchEntries: IN 1 " << mGroupHighRise[i].groupName;

    while((!mGroupHighRise[i].entries.isEmpty()) && i<mNumHighRiseGroups){
        qDebug() << "searchEntries: IN 2 highrise name  " << mGroupHighRise[i].groupName << "compared to " << mGroupName;

        if(mGroupHighRise[i].groupName.contains(mGroupName)){
            bool res;
            int ref1;
            int ref2;
            int cnt;
            char dump[100];
            QByteArray textline;
            const char *text;


            if(mGroupHighRise[i].groupName.contains("Entrance")){
                if(mGroupName == mGroupHighRise[i].groupName ){
                    qDebug("searchEntries: Match Found");

                    retList = mGroupHighRise[i].entries;

                    break;
                }
            }
            else{
                //check for Telephone
                if(mGroupHighRise[i].groupName.contains(mGroupName)){
                    QRegExp rx("(\\d+)");

                    QStringList list;
                    int pos = 0;

                    while ((pos = rx.indexIn(mGroupHighRise[i].groupName, pos)) != -1) {
                        list << rx.cap(1);
                        pos += rx.matchedLength();
                    }
                    if(list.size() && (mGroupName == list[0])){
                        retList = mGroupHighRise[i].entries;
                        qDebug("Split numeber" );
                        break;
                    }

                }
            }
        }
        i++;
    };

    return retList;

}

void entryCtrl::setProxyAddress(QString val){
    char buf[80];
    memset(buf,0,sizeof(buf));
    strncpy(buf, val.toLatin1().data(), val.length() + 1);
    qDebug("setProxyAddress %s",buf);
    m_pParent->setProxyAddress(buf);

}

void entryCtrl::clrAllHighRiseEntries(){
    for(int i=0;i<mNumHighRiseGroups;i++){
        while(!mGroupHighRise[i].entries.isEmpty())
            mGroupHighRise[i].entries.removeFirst();
    }
    mNumHighRiseGroups=0;
}



int entryCtrl::createGroups(QString user){
    int i=0;
    int ret=0;
    bool added = false;
    QString groupName;
    QStringList tmp,retList;
    tmp=user.split("-");
    groupName=tmp[0];
    qDebug() << "groupName " << groupName;
    //    if(groupName.contains("Entrance",Qt::CaseInsensitive))
    //        return ret;
    while((!mGroupHighRise[i].entries.isEmpty()) && i<mNumHighRiseGroups){
        if(mGroupHighRise[i].groupName.contains(groupName)){

            mGroupHighRise[i].entries << user;
            added = true;
            break;
        }
        i++;
    };
    if(added == false){
        if(i<MAXENTRIES){
            mGroupHighRise[i].entries << user;
            mGroupHighRise[i].groupName = groupName;
            ret=1;
            mNumHighRiseGroups++;
            qDebug("createGroups size %d",mNumHighRiseGroups);
            qDebug() << "mGroupHighRise:groupName " << mGroupHighRise[i].groupName;
            qDebug() << "mGroupHighRise:entries " << mGroupHighRise[i].entries;
        }
        else
            ret = -1;
    }

    return ret;
}

QString entryCtrl::readRegistrarFile(){
    QFile file(LOCALREGISTRARFILE);
    QString ip;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //QMessageBox::warning(this, tr("File error"), tr("Failed to open\n%1").arg(filename));
        return ip;
    }
    ip=file.readLine().simplified();

    qDebug() << "readRegistrarFile " << ip;
    return ip;
}

QStringList entryCtrl::readFile(QString fullFileName){

    QStringList mAllUsers;

    QFile file(fullFileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //QMessageBox::warning(this, tr("File error"), tr("Failed to open\n%1").arg(filename));
        return mAllUsers;
    }
    QString line;
    int size;
    while (!file.atEnd()) {
        line = file.readLine();
        line = line.trimmed();
        size=line.size();
        if(line.contains("<option value=\"", Qt::CaseInsensitive)){
            line.remove(QRegExp("<option value=\""));
            line.remove(QRegExp("\">"));
            line.remove(QRegExp("</option>"));
            size=line.size();
            mAllUsers<< line.left(size/2);

        }
    }

    file.close();
    mAllUsers.sort();
    return mAllUsers;
}

