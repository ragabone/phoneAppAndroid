#ifndef AUDIOHANDLE_H
#define AUDIOHANDLE_H

#include <QTimer>
#include <QObject>
#include <QByteArray>
//#include <SoundTouch.h>
#include <qaudioinput.h>
#include <qaudiooutput.h>
#include <QBuffer>
#include <QThread>
#include <pjmedia/port.h>


class audioHandle: public QObject
{
    Q_OBJECT
public:
    audioHandle();
    ~audioHandle();
    Q_INVOKABLE void start_media();
    Q_INVOKABLE void stop_media();
    static void setMediaPort(pjmedia_port *port);
    void initializeAudio();

signals:
    void sigStartAudioOut();
private:
    QThread *m_thread;

    QAudioDeviceInfo m_Inputdevice;
    QAudioDeviceInfo m_Outputdevice;
    QAudioFormat m_format;
    QAudioInput *m_audioInput;
    QAudioOutput *m_audioOutput;
    QIODevice *m_input;
    QIODevice *m_output;
    QByteArray m_buffer;
    int mTimestampPut;
    int mTimestampGet;
    bool mMediaStarted;

private:
    void createAudioInput();
    void createAudioOutput();
    QTimer *m_pullTimer;
public slots:
    void pullTimerExpired();

private slots:
    void writeSound(void);
    void readMore();

public:
    pjmedia_port *mMediaPort;

};

class audioHandleThread: public QThread{
public:
    explicit audioHandleThread(QObject *parent = 0)
        : QThread(parent)
    {


    }

    ~audioHandleThread()
    {
        quit();
        wait();
    }
protected:
    virtual void run()
    {
        mAudioHandle = new audioHandle;
        exec();

    }
public:
    audioHandle *mAudioHandle;
    //void start_media(){mAudioHandle->start_media(); }
private:

};

#endif // AUDIOHANDLE_H
