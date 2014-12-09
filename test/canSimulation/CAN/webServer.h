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
    WebServer(QObject *rootObject);
    ~WebServer();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();
    void write(QString);
    void createConection(QString, int);

private:
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket *m_client;
    QObject *rootView;
};

#endif // WEBSERVER_H
