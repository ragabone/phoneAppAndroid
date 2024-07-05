#include <QMutex>
#include <QDebug>
#include "videothread.h"
#include "mpeg4decoder.h"


videoThread::videoThread(){

    moveToThread(this);
}

void videoThread::run()
{

    initializeMpeg4();
    for(int i=0;i<NUMBUFFERS;i++)
        mBuffer[i].bufferType = NOVALIDDATA;
    exec();
}




void videoThread::threadStart(){
    while(1){
        setLatestBuffer(getNextBufferRGBA());
        //usleep(40000);
    }
}

char *videoThread::getLatestVideobuffer(){
    char *latestBuffer=NULL;
    videoBufferMutex.lock();
    for(int i=0;i<NUMBUFFERS;i++){
        if(mBuffer[i].bufferType == LATEST){
            latestBuffer=(char *)mBuffer[i].buffer;
            mBuffer[i].bufferType = RENDERING;
            break;
        }
    }
     qDebug("Get latest Buffer 0x%x",latestBuffer);
    videoBufferMutex.unlock();
    return latestBuffer;
}

void videoThread::releaseLatestBuffer(char *buffer){

    videoBufferMutex.lock();
    qDebug("release 0x%x",buffer);
    for(int i=0;i<NUMBUFFERS;i++){
        qDebug("release ref 0x%x",mBuffer[i].buffer);
        if(mBuffer[i].buffer == (char(*)[272][3])buffer){
            mBuffer[i].bufferType=NOVALIDDATA;
            qDebug("Release 0x%x",mBuffer[i].buffer);
            break;
        }
    }

    videoBufferMutex.unlock();
}


void videoThread::setLatestBuffer(char *buffer){
    videoBufferMutex.lock();
    int found=0;
    for(int i=0;i<NUMBUFFERS;i++){
        if(mBuffer[i].bufferType == NOVALIDDATA){
            for(int j=0;j<NUMBUFFERS;j++){
                if(mBuffer[j].bufferType == LATEST)
                    mBuffer[j].bufferType = NOVALIDDATA;
            }
            memcpy(mBuffer[i].buffer,buffer,sizeof(mBuffer[i].buffer));
            mBuffer[i].bufferType=LATEST;
            m_pLatestBuffer=(char *)mBuffer[i].buffer;
            qDebug("Set 0x%x",m_pLatestBuffer);
            found =1;
            break;
        }
    }
    //Try OverWrite Latest Buffer
    if(found == 0){
        for(int i=0;i<NUMBUFFERS;i++){
            if(mBuffer[i].bufferType == LATEST){
                //Set All buffers to NOVALIDDATA
                for(int j=0;j<NUMBUFFERS;j++){
                    if(mBuffer[j].bufferType == LATEST)
                        mBuffer[j].bufferType = NOVALIDDATA;
                }
                memcpy(mBuffer[i].buffer,buffer,sizeof(mBuffer[i].buffer));
                mBuffer[i].bufferType=LATEST;
                m_pLatestBuffer=(char *)mBuffer[i].buffer;
                qDebug("Set 0x%x",m_pLatestBuffer);
                break;
            }
        }

    }


    videoBufferMutex.unlock();


}



