#include <ESip.h>
#include <QTimer>
#include <QDebug>
#include "audiohandle.h"

#define NUMSAMPLES 160
#define RAWBYTESIZE 160*2

static audioHandle *self;
audioHandle::audioHandle(): QObject(0)
  ,   m_Inputdevice(QAudioDeviceInfo::defaultInputDevice())
  ,   m_Outputdevice(QAudioDeviceInfo::defaultOutputDevice())
  ,   m_audioInput(0)
  ,   m_audioOutput(0)
  ,   m_input(0)
  ,   mTimestampPut(0)
  ,   mTimestampGet(0)
  ,   mMediaPort(NULL)
  ,   mMediaStarted(false)
  ,   m_buffer(RAWBYTESIZE, 0)
  ,   m_pullTimer(NULL)
  ,   m_thread(new QThread(this))
{
    this->setParent(0);
    initializeAudio();

}

audioHandle::~audioHandle()
{

}
static pj_thread_desc handler_desc;

//Initialize audio
void audioHandle::initializeAudio()
{
    pj_thread_t *thread;

    pj_thread_register("audio_thread", handler_desc, &thread);

    self=this;
    initMediaPortCallback(&audioHandle::setMediaPort);


    if(m_pullTimer == NULL){
        m_pullTimer= new QTimer;
        connect(m_pullTimer, SIGNAL(timeout()),this, SLOT(pullTimerExpired()));
    }

    //m_pullTimer->start(20);

    m_format.setSampleRate(8000); //set frequency to 8000
    m_format.setChannelCount(1); //set channels to mono
    m_format.setSampleSize(16); //set sample sze to 16 bit
    m_format.setSampleType(QAudioFormat::UnSignedInt ); //Sample type as usigned integer sample
    m_format.setByteOrder(QAudioFormat::LittleEndian); //Byte order
    m_format.setCodec("audio/pcm"); //set codec as simple audio/pcm

    QAudioDeviceInfo infoIn(QAudioDeviceInfo::defaultInputDevice());
    if (!infoIn.isFormatSupported(m_format))
    {
        //Default format not supported - trying to use nearest
        m_format = infoIn.nearestFormat(m_format);
    }

    QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());

    if (!infoOut.isFormatSupported(m_format))
    {
       //Default format not supported - trying to use nearest
        m_format = infoOut.nearestFormat(m_format);
    }
    createAudioInput();
    createAudioOutput();
}

void audioHandle::pullTimerExpired()
{
    if (m_audioOutput && m_audioOutput->state() != QAudio::StoppedState) {
        writeSound();
    }
}


void audioHandle::createAudioOutput()
{
    m_audioOutput = new QAudioOutput(m_Outputdevice, m_format, this);
}
void audioHandle::createAudioInput()
{
    if (m_input != 0) {
        disconnect(m_input, 0, this, 0);
        m_input = 0;
    }

    m_audioInput = new QAudioInput(m_Inputdevice, m_format, this);

}

void audioHandle::stop_media()
{
    mMediaStarted = false;
    m_pullTimer->stop();
    //Audio output device
    m_audioOutput->stop();
     //Audio input device
    m_audioInput->stop();


}

void audioHandle::start_media()
{
    //Audio output device
    m_output = m_audioOutput->start();

    m_pullTimer->start(20);
     //Audio input device
    m_input = m_audioInput->start();
    //connect readyRead signal to readMre slot.
    //Call readmore when audio samples fill in inputbuffer
    connect(m_input, SIGNAL(readyRead()),this, SLOT(readMore()));
    //connect(this, SIGNAL(sigStartAudioOut()),this, SLOT(writeSound()));

    //QTimer::singleShot(10, this, SLOT(writeSound()));
    mMediaStarted = true;
    //emit sigStartAudioOut();
}

void audioHandle::setMediaPort(pjmedia_port *port){
    self->mMediaPort = port;
}

void audioHandle::readMore()
{
    pjmedia_frame frame;



    //Return if audio input is null
    if(!m_audioInput)
        return;

    if(mMediaPort == NULL)
        return;
    //media_port=(pjmedia_port *)thrArg->ptr;
    //Check the number of samples in input buffer
    qint64 len = m_audioInput->bytesReady();

    //Limit sample size
    if(len > RAWBYTESIZE)
        len = RAWBYTESIZE;
    //Read sound samples from input device to buffer
    qint64 l = m_input->read(m_buffer.data(), len);
    if(l > 0)
    {
        //Assign sound samples to short array
        short* resultingData = (short*)m_buffer.data();


        frame.buf=resultingData;

        //Send to RTP stack
        frame.size = RAWBYTESIZE;
        frame.type = PJMEDIA_FRAME_TYPE_AUDIO;
        frame.timestamp.u32.hi = 0;
        frame.timestamp.u32.lo = mTimestampPut;
        pjmedia_port_put_frame(mMediaPort, &frame);
        mTimestampPut+=1;


    }

}
void audioHandle::writeSound(void){
    short outdata[NUMSAMPLES];
    pjmedia_frame frame;
    //while(mMediaStarted){
        frame.buf = (void*)outdata;
        frame.size = RAWBYTESIZE;
        //frame.size = -1;
        frame.type = PJMEDIA_FRAME_TYPE_AUDIO;
        frame.timestamp.u32.hi = 0;
        frame.timestamp.u32.lo = mTimestampGet;
        pjmedia_port_get_frame(mMediaPort, &frame);
        mTimestampGet+=1;
        if(frame.size){
            qint64 status;
            status = m_output->write((char *)outdata, RAWBYTESIZE);
            //qDebug() << "m_audio written bytes " + status;
        }
//        else
//             qDebug() << "m_audio not written bytes ";


}

