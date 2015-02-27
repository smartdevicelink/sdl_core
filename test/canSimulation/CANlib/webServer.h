#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebServer : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief WebServer
     * @param rootObject
     * ctor
     *
     * To get access to QML from C++
     */
    WebServer();

    /**
     * dtor
     */
    ~WebServer();

public slots:

    /**
     * @brief onNewConnection
     *
     * Handler for new connected clients by WebSocket
     * Note: only last connetced one is active on Sever
     */
    void onNewConnection();

    /**
     * @brief processTextMessage
     * @param message - text messages came from WebSocket client
     *
     * Method to redirect requests and responses from WebSocket Client to QML
     */
    void processTextMessage(QString message);

    /**
     * @brief socketDisconnected
     * Handler for disconnected WebSocket Client connection
     */
    void socketDisconnected();

    /**
     * @brief write
     * @param qMessage - text message to be sent to WS Client
     */
    bool write(const QString &qMessage);

    /**
     * @brief createConection
     * @param IP - IP adres v4
     * @param port
     * Parameters to be used for WS Server for listening the WS port
     */
    bool createConection(const QString &IP, int port);

signals:

    /**
     * @brief log
     * @param qMessage
     * @param color
     *
     * Signal for external interface with log messages to be displayed for user
     */
    void log(const QString &qMessage, int color);

    /**
     * @brief readFromWS
     * @param qMessage
     *
     * Signal for external interface with data came from WS client
     */
    void readFromWS(const QString &qMessage);

    /**
     * @brief newConnection
     *
     * Signal to notify that new client is connected to server
     */
    void newConnection();

private:
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket *m_client;
};

#endif // WEBSERVER_H
