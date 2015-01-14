#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork>
#include <QTcpSocket>
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
     * dtor
     */
    ~Server();

public slots:
    /**
     * @brief write
     * @param qMessage - text message to be sent to TCP Client
     */
    void write(QString qMessage);

    /**
     * @brief readyRead
     * Method to redirect requests and responses from TCP Client to QML
     * and send responses to TCP Client using method "write" if necessary
     */
    void readyRead();

    /**
     * @brief connected
     * Handler for new connected clients by TCP
     * Note: only last connetced one is active on Sever
     */
    void connected();

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
     * @brief createConection
     * @param IP - IP adres v4
     * @param port
     * Parameters to be used for connection to TCP Server
     */
    void createConection(QString IP, int port);

private:

    QTcpServer *tcpServer;
    QTcpSocket *clientConnection;
    QObject *rootView;
};

#endif // NETWORK_H
