#ifndef SMARTDEVICELINK_H
#define SMARTDEVICELINK_H

#include <QQuickItem>
#include <QStringListModel>
#include "QtWebsocket/QWsSocket.h"
#include "sdlappslistmodel.h"

class SmartDeviceLink : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString show1 READ getShow1 NOTIFY show1Changed)
    Q_PROPERTY(QString show2 READ getShow2 NOTIFY show2Changed)

public:
    SmartDeviceLink(QQuickItem *parent = 0);
    ~SmartDeviceLink();

    static SmartDeviceLink& getInstance() {
        static SmartDeviceLink instance;
        return instance;
    }

    SdlAppsListModel* getMediaApps() { return &m_media_apps; }
    QString getShow1() { return m_show1; }
    QString getShow2() { return m_show2; }

private:
    QWsSocket m_ws_basic;
    QWsSocket m_ws_tts;
    QWsSocket m_ws_buttons;
    QWsSocket m_ws_vehicle;
    QWsSocket m_ws_ui;
    QWsSocket m_ws_vr;

    SdlAppsListModel m_media_apps;

    QString m_show1;
    QString m_show2;

    void basic_send(QString message);
    void tts_send(QString message);
    void buttons_send(QString message);
    void vehicle_send(QString message);
    void ui_send(QString message);
    void vr_send(QString message);
    void setShow1(QString);
    void setShow2(QString);

public slots:
    void basic_connected();
    void tts_connected();
    void buttons_connected();
    void vehicle_connected();
    void ui_connected();
    void vr_connected();

    void basic_receive(QString);
    void tts_receive(QString);
    void buttons_receive(QString);
    void vehicle_receive(QString);
    void ui_receive(QString);
    void vr_receive(QString);
    void registerApp(int, QString);

    void disconnected();

signals:
    void show1Changed(QString);
    void show2Changed(QString);
    void newAppAvailable(int, QString);
    void alert(QString message);
};

#endif // SMARTDEVICELINK_H
