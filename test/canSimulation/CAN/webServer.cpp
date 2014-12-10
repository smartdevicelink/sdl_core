#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include "webServer.h"
#include "loger.h"
#include "color.h"
//#include <QtCore/QDebug>

//QT_USE_NAMESPACE

WebServer::WebServer(QObject *rootObject) : rootView(rootObject), m_client(NULL), QObject() {

    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Web Server"), QWebSocketServer::NonSecureMode, this);

    connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebServer::onNewConnection);
}

WebServer::~WebServer()
{
    //m_pWebSocketServer->close();
    delete m_client;
}

void WebServer::onNewConnection()
{
    Loger::loger("WS connected...", RED);

    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebServer::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebServer::socketDisconnected);

    m_client = pSocket;
}

void WebServer::processTextMessage(QString qMessage)
{

    Loger::loger("WS Received..." + qMessage, GREEN);

    QMetaObject::invokeMethod(rootView, "incomingWS",
            Q_ARG(QVariant,  qMessage));

//    qb = returnedValue.toString().toUtf8();

//    char *ch = qb.data();

//    write(ch);
}

void WebServer::socketDisconnected()
{
    m_client->deleteLater();
}

void WebServer::write(QString name)
{
    QByteArray qb = name.toUtf8();
    char *cName = qb.data();

    if ((m_client != NULL) && (m_client->state() == QTcpSocket::ConnectedState)){

        m_client->sendTextMessage(cName);
        Loger::loger("WS Send:" + name, BLUE);
    } else {
        Loger::loger("WS Client is not connected yet...", RED);
    }
}

void WebServer::createConection(QString IP, int port)
{

    QHostAddress hostadd(IP);

    if (m_pWebSocketServer->listen(hostadd, port)) {
        Loger::loger("Start listening WebSocket:" + IP + ":" + QString::number(port), RED);

        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebServer::closed);
    } else {
        Loger::loger("Unable to start the WS server: " + m_pWebSocketServer->errorString(), RED);
    }

}
