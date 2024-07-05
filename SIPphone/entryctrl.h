#ifndef ENTRYCTRL_H
#define ENTRYCTRL_H

#define MAXENTRIES 100

#include <QObject>

class QString;
class QStringList;
class semCtrl;
class myHttp;
class sipStackIf;

class entryCtrl: public QObject
{
public:
    entryCtrl(sipStackIf *);
    int addEntry(QString name);
    void clrAllEntries();
    QStringList searchHighRiseEntries(QString fullFileName);
    QStringList searchEntries(QString name);
    void setProxyAddress(QString val);
    QString readRegistrarFile();
    void beginSearch(QString user,QString type);
    void searchCallBack(int status, QString userData);
private:
    struct group{
        QString groupName;
        QStringList entries;
    };
    int mNumGroups;
    QString mGroupName;
    sipStackIf *m_pParent;
    group mGroup[MAXENTRIES];
    int mNumHighRiseGroups;
    group mGroupHighRise[MAXENTRIES];
    void clrAllHighRiseEntries();
    int createGroups(QString name);
    QStringList readFile(QString);
    semCtrl *m_PsemSetProxy;
    semCtrl *m_PsemSetProxyResp;
    myHttp *m_pHttpReq;


};

#endif // ENTRYCTRL_H
