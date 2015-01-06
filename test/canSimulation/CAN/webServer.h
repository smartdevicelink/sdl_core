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
     * To get access to QML from C++ rootView must be main QObject of QML view
     */
    WebServer(QObject *rootObject);

    /**
     * dtor
     */
    ~WebServer();

private Q_SLOTS:

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
     * @param qMessage - text message to be sent to TCP Client
     */
    void write(QString qMessage);

    /**
     * @brief createConection
     * @param IP - IP adres v4
     * @param port
     * Parameters to be used for connection to TCP Server
     */
    void createConection(QString IP, int port);

private:
    QWebSocketServer *m_pWebSocketServer;
    QObject *rootView;
    QWebSocket *m_client;
};

#endif // WEBSERVER_H
