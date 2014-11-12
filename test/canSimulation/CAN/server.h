#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork>
#include <QTcpSocket>
class QTcpServer;
class QNetworkSession;

class Server : public QObject
{
    Q_OBJECT

public:
    Server(QObject *parent);
    Server();

public slots:
    void write(QString);
    void readyRead();
    void connected();
    void disconnected();
    void displayError(QAbstractSocket::SocketError);
    void createConection(QString, int);

private:

    QTcpServer *tcpServer;
    QString currentFortune;
    quint16 blockSize;
    QTcpSocket *clientConnection;
};

#endif // NETWORK_H
