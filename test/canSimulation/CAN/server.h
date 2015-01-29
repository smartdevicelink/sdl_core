#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork>
#include <QTcpSocket>
#include "messageHandler.h"
class QTcpServer;
class QNetworkSession;

class Server : public QObject
{
    Q_OBJECT
    /**
     * Colog enum for loger
     */
    Q_ENUMS(Color)

public:
    /**
     * @brief Server
     * @param rootObject
     * ctor
     * To get access to QML from C++ rootView must be main QObject of QML view
     */
    Server(QObject *rootObject);

    /**
     * @brief addThread
     * @param client
     */
    void addThread(QTcpSocket *client);

    QVector<QString> messagePull;

    /**
     * dtor
     */
    ~Server();

public slots:

    /**
     * @brief connected
     * Handler for new connected clients by TCP
     * Note: only last connetced one is active on Sever
     */
    void connected();

    /**
     * @brief createConection
     * @param IP - IP adres v4
     * @param port
     * Parameters to be used for connection to TCP Server
     */
    void createConection(const QString &IP, int port);

    void write(const QString &message);

    void requestFromTCP(const QString &qMessage);

    void log(const QString &qMessage, int color);

signals:

    void stopAllThreads();
    void startAgaing();

private:

    QTcpServer *tcpServer;
    QObject *rootView;
    QMutex *mutex;
};

#endif // NETWORK_H
