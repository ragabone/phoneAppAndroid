#include <QtCore>
#include <QtGui>
#include <QTcpSocket>
#include "tcpclientRead.h"
//#define SERVERIP "192.168.1.17"


#define SERVERPORT 3550
#define JPEGMAXSIZE 16364*2

tcpClientRead::tcpClientRead()
{
m_pJpegDataStore = new char[JPEGMAXSIZE];
m_pSocket = new QTcpSocket( this ); // <-- needs to be a member variable: QTcpSocket * _pSocket;

}


bool tcpClientRead::connectTcp(char *serverIp)
{
    bool ret=false;
    QByteArray data; // <-- fill with data

    //connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()));
    m_pSocket->connectToHost(serverIp, SERVERPORT);
    if( m_pSocket->waitForConnected(20000) ) {
        if( m_pSocket->waitForReadyRead(20000) ) {
            //read size of packet
            data = m_pSocket->read(sizeof(int));
            int size = (int)data[0]+((int)data[1]<<8)+((int)data[2]<<16)+((int)data[3]<<24);

            qDebug("bytesAvailable readTcpData %d ",size);
            ret=readTcpData(size);
        }

    }
    else{
        qDebug("Failed to connect socket timeout");
        m_pSocket->close();
    }
    return ret;

}

bool tcpClientRead::readTcpData(int totSize)
{
    int size=0;
    bool ret=false;
    char *jpegData = m_pJpegDataStore;
    qDebug("jpegDataStore 0x%x",jpegData);
    if(totSize>JPEGMAXSIZE){
        qDebug("readTcpData: JPEG image is too big %d",totSize);
        m_pSocket->close();
        return ret;
    }
    QFile file("images/screenshot.jpg");
    //qDebug("readTcpData: Open file");
    if(file.exists())
        file.remove();
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug("Failed to open file");
        //QMessageBox::warning(this, tr("File error"), tr("Failed to open\n%1").arg(filename));
        m_pSocket->close();
        return false;
    }
    while(m_pSocket->waitForReadyRead(2000)){
        while(m_pSocket->bytesAvailable()){
             int bytes;
            //int bytes = m_pSocket->bytesAvailable();
            qDebug("jpegData 0x%x",jpegData);
            bytes=m_pSocket->read(jpegData,2048);
            //size+=m_pSocket->read(jpegData,2048);
            size+=bytes;
            jpegData+=bytes;
            qDebug("bytes read %d",bytes);
            if(size>=totSize){
                file.write(m_pJpegDataStore,size);
                file.close();
                m_pSocket->close();
                //delete m_pSocket;
                //delete jpegDataStore;
                qDebug("readTcpData: Socket Closed 1");
                return true;
            }

        }

    }
    file.close();
    m_pSocket->close();
    //delete m_pSocket;
    //qDebug("readTcpData: Socket Closed 2");
    return false;
}
