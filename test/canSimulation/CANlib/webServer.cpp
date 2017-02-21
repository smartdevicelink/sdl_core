#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include "webServer.h"
#include "color.h"

QT_USE_NAMESPACE

WebServer::WebServer() : m_client(NULL), QObject() {

    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Web Server"), QWebSocketServer::NonSecureMode, this);

    connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebServer::onNewConnection);
}

WebServer::~WebServer()
{
    m_pWebSocketServer->close();
    delete m_client;
    m_client = NULL;
}

void WebServer::onNewConnection()
{
    emit log("WS connected...", RED);

    // New connected client is only one client TCP Server is working with
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebServer::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebServer::socketDisconnected);

    m_client = pSocket;

    emit newConnection();
}

void WebServer::processTextMessage(QString qMessage)
{

    emit log("WS Received..." + qMessage, GREEN);

    emit readFromWS(qMessage);
}

void WebServer::socketDisconnected()
{
    m_client->deleteLater();
    m_client = NULL;
}

bool WebServer::write(const QString &qMessage)
{
    QByteArray qb = qMessage.toUtf8();
    char *cMessage = qb.data();

    if ((m_client != NULL) && (m_client->state() == QAbstractSocket::ConnectedState)){

        m_client->sendTextMessage(cMessage);
        emit log("WS Send:" + qMessage, BLUE);

        return true;
    } else {
        emit log("WS Client is not connected yet...", RED);

        return false;
    }
}

bool WebServer::createConection(const QString &IP, int port)
{

    QHostAddress hostadd(IP);

    if (m_pWebSocketServer->listen(hostadd, port)) {
        emit log("Start listening WebSocket:" + IP + ":" + QString::number(port), RED);

        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebServer::socketDisconnected);

        return true;
    } else {
        emit log("Unable to start the WS server: " + m_pWebSocketServer->errorString(), RED);

        return false;
    }

}
