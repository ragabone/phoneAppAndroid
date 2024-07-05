#ifndef TCPCLIENTREAD_H
#define TCPCLIENTREAD_H

#include <QObject>
#include <QTcpSocket>

class QTcpSocket;

class tcpClientRead : public QObject
{
    Q_OBJECT

public:
    tcpClientRead();
    bool connectTcp(char *);

private slots:
    bool readTcpData(int);

private:
    QTcpSocket *m_pSocket;
    char *m_pJpegDataStore;



};

#endif // TCPCLIENT_H
