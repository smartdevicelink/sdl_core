#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork>
#include <QTcpSocket>
#include "messageHandler.h"

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
     * To get access to QML from C++
     */
    Server();

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
     */
    void connected();

    /**
     * @brief createConection
     * @param IP - IP adres v4
     * @param port
     * Parameters to be used for connection to TCP Server
     */
    void createConection(const QString &IP, int port);

    /**
     * @brief write
     * @param qMessage
     * @return
     *
     * Method stores messages in TCP message pull and than
     * ru the process in thread to send all data in message pull to client
     */
    bool write(const QString &qMessage);

    /**
     * @brief requestFromTCP
     * @param qMessage
     *
     * Method emit's signal readFromTCP to notify that message came
     * from TCP client to server
     */
    void requestFromTCP(const QString &qMessage);

    /**
     * @brief logMessageHandler
     * @param qMessage
     * @param color
     *
     * Method to redirect log messages to external interface
     */
    void logMessageHandler(const QString &qMessage, int color);


signals:

    /**
     * @brief stopAllThreads
     *
     * Signal to stop all running threads
     */
    void stopAllThreads();

    /**
     * @brief startAgaing
     *
     * Signal to start send messages from TCP message pull to TCP client
     */
    void startAgaing();

    /**
     * @brief newConnection
     *
     * Signal to notify that new client is connected to server
     */
    void newConnection();

    /**
     * @brief readFromTCP
     * @param qMessage
     *
     * Signal for external interface with data came from TCP client
     */
    void readFromTCP(const QString &qMessage);

    /**
     * @brief log
     * @param qMessage
     * @param color
     *
     * Signal for external interface with log messages to be displayed for user
     */
    void log(const QString &qMessage, int color);

private:

    QTcpServer *tcpServer;
    QMutex *mutex;
    MessageHandler *messageHandler;
    QThread *thread;
};

#endif // NETWORK_H
