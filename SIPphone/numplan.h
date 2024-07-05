#ifndef NUMPLAN_H
#define NUMPLAN_H

#include <QObject>
class QTimer;
class QMutex;

class numPlan : public QObject
{
    Q_OBJECT
public:
    numPlan(char *,int);
    QString lookupNum(char *num);
private slots:
    void getNumberPlan();
private:
    QTimer *mTimer;
    QMutex *mMutex;
    void setBuffer(char *);
    char *getNumberPlanBuffer();
    char numBuffer[16][16][5];
    bool parseCSV();
    char m_pFilename[200];
    int mGrid;

};

#endif // NUMPLAN_H
