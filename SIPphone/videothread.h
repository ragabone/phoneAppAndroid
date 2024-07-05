#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H
#include <QMutex>
#include <QObject>
#include <QThread>

class QMutex;

#define NUMBUFFERS 3
typedef struct videoManager{
    int bufferType;
    char buffer[480][272][3];

}videoManager;

enum {
    NOVALIDDATA,
    RENDERING,
    LATEST
};

class videoThread : public QThread
{
    Q_OBJECT
public:
    videoThread();
    void run();
    char *getLatestVideobuffer();
    void releaseLatestBuffer(char *);
public slots:
    void threadStart();
private:
    char *m_pLatestBuffer;
    QMutex videoBufferMutex;
    videoManager mBuffer[NUMBUFFERS];
private:
    void setLatestBuffer(char *buffer);

};

#endif // VIDEOTHREAD_H
