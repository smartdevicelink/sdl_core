#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <QMutex>

class MessageHandler : public QObject
{
    Q_OBJECT
    /**
     * Colog enum for loger
     */
    Q_ENUMS(Color)

public:

    /**
     * @brief MessageHandler
     * @param client
     *
     * ctor
     * Initialized with QTcpSocket of newly connected client
     */
    MessageHandler(QTcpSocket *client, QMutex *mu, QVector<QString> &msgPull);

    /**
     * dtor
     */
    ~MessageHandler();

public slots:

    /**
     * @brief readFromTCP
     * Method to redirect requests and responses from TCP Client to external interface
     */
    void readFromTCP();


    /**
     * @brief writeToTCP
     * @param qMessage - text message to be sent to TCP Client
     * Method to send message to TCP client
     */
    void writeToTCP(const QString &qMessage);

    /**
     * @brief disconnected
     * Handler for disconnected TCP Client connection
     */
    void disconnected();

    /**
     * @brief displayError
     * @param socketError - TCP Soocket error mesage
     * Redirect error message to QML using Loger
     */
    void displayError(QAbstractSocket::SocketError socketError);

    /**
     * @brief process
     *
     * Main process of Server's thread that send's messages to TCP client
     */
    void process();

    void stop();

signals:

    /**
     * @brief logMessageHandler
     * @param qMessage
     * @param color
     *
     * Signal invoked to notify that MessageHandler forwarding a log message
     */
    void logMessageHandler(const QString &qMessage, int color);

    /**
     * @brief disconnect
     *
     * Signal invoked when TCP client disconnects
     */
    void disconnect();

    /**
     * @brief requestFromTCP
     * @param qMessage
     *
     * Signal emits from thread when message came to TCP server from client
     */
    void requestFromTCP(const QString &qMessage);

private:

    QTcpSocket *clientConnection;
    QMutex *mutex;
    QVector<QString>& messagePull;

};

#endif // MESSAGEHANDLER_H
