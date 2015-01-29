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
    MessageHandler(QObject *rootObject, QTcpSocket *client, QMutex *mu, QVector<QString> &msgPull);

    /**
     * dtor
     */
    ~MessageHandler();

public slots:

    /**
     * @brief readFromTCP
     * Method to redirect requests and responses from TCP Client to QML
     * and send responses to TCP Client using method "write" if necessary
     */
    void readFromTCP();


    /**
     * @brief writeToTCP
     * @param qMessage - text message to be sent to TCP Client
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

    void process();
    void stop();

signals:
    void disconnect();
    void requestFromTCP(const QString &qMessage);
    void log(const QString &qMessage, int);

private:

    QTcpSocket *clientConnection;
    QObject *rootView;
    QMutex *mutex;
    QVector<QString>& messagePull;

};

#endif // MESSAGEHANDLER_H
