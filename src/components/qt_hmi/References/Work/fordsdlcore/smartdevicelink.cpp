#include "smartdevicelink.h"
#include <QDebug>
#include "QtWebsocket/QWsSocket.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <unistd.h>

SmartDeviceLink::SmartDeviceLink(QQuickItem *parent):
    QObject(parent)
{
    connect(&m_ws_basic, SIGNAL(connected()), this, SLOT(basic_connected()));
    connect(&m_ws_basic, SIGNAL(frameReceived(QString)), this, SLOT(basic_receive(QString)));
    connect(&m_ws_basic, SIGNAL(disconnected()), this, SLOT(disconnected()));

    connect(&m_ws_buttons, SIGNAL(connected()), this, SLOT(buttons_connected()));
    connect(&m_ws_buttons, SIGNAL(frameReceived(QString)), this, SLOT(buttons_receive(QString)));
    connect(&m_ws_buttons, SIGNAL(disconnected()), this, SLOT(disconnected()));

    connect(&m_ws_tts, SIGNAL(connected()), this, SLOT(tts_connected()));
    connect(&m_ws_tts, SIGNAL(frameReceived(QString)), this, SLOT(tts_receive(QString)));
    connect(&m_ws_tts, SIGNAL(disconnected()), this, SLOT(disconnected()));

    connect(&m_ws_vehicle, SIGNAL(connected()), this, SLOT(vehicle_connected()));
    connect(&m_ws_vehicle, SIGNAL(frameReceived(QString)), this, SLOT(vehicle_receive(QString)));
    connect(&m_ws_vehicle, SIGNAL(disconnected()), this, SLOT(disconnected()));

    connect(&m_ws_ui, SIGNAL(connected()), this, SLOT(ui_connected()));
    connect(&m_ws_ui, SIGNAL(frameReceived(QString)), this, SLOT(ui_receive(QString)));
    connect(&m_ws_ui, SIGNAL(disconnected()), this, SLOT(disconnected()));

    connect(&m_ws_vr, SIGNAL(connected()), this, SLOT(vr_connected()));
    connect(&m_ws_vr, SIGNAL(frameReceived(QString)), this, SLOT(vr_receive(QString)));
    connect(&m_ws_vr, SIGNAL(disconnected()), this, SLOT(disconnected()));

    connect(this, SIGNAL(newAppAvailable(int, QString)), this, SLOT(registerApp(int,QString)));

    m_ws_buttons.connectToHost( QLatin1String("127.0.0.1"), 8087 );
    m_ws_tts.connectToHost( QLatin1String("127.0.0.1"), 8087 );
    m_ws_vr.connectToHost( QLatin1String("127.0.0.1"), 8087 );
    m_ws_basic.connectToHost( QLatin1String("127.0.0.1"), 8087 );
    m_ws_vehicle.connectToHost( QLatin1String("127.0.0.1"), 8087 );
    m_ws_ui.connectToHost( QLatin1String("127.0.0.1"), 8087 );
}

SmartDeviceLink::~SmartDeviceLink()
{
    qDebug() << "SmartDeviceLink destroyed";
}

void SmartDeviceLink::basic_connected() {
    basic_send("{\"jsonrpc\":\"2.0\",\"id\":600,\"method\":\"MB.registerComponent\",\"params\":{\"componentName\":\"BasicCommunicationClient\"}}");
}

void SmartDeviceLink::buttons_connected() {
    buttons_send("{\"jsonrpc\":\"2.0\",\"id\":200,\"method\":\"MB.registerComponent\",\"params\":{\"componentName\":\"Buttons\"}}");
}

void SmartDeviceLink::tts_connected() {
    tts_send("{\"jsonrpc\":\"2.0\",\"id\":300,\"method\":\"MB.registerComponent\",\"params\":{\"componentName\":\"TTS\"}}");
}

void SmartDeviceLink::vehicle_connected() {
    vehicle_send("{\"jsonrpc\":\"2.0\",\"id\":700,\"method\":\"MB.registerComponent\",\"params\":{\"componentName\":\"VehicleInfo\"}}");
}

void SmartDeviceLink::ui_connected() {
    ui_send("{\"jsonrpc\":\"2.0\",\"id\":400,\"method\":\"MB.registerComponent\",\"params\":{\"componentName\":\"UI\"}}");
}

void SmartDeviceLink::vr_connected() {
    vr_send("{\"jsonrpc\":\"2.0\",\"id\":500,\"method\":\"MB.registerComponent\",\"params\":{\"componentName\":\"VR\"}}");
}

void SmartDeviceLink::basic_send(QString message) {
    qDebug() << "Sending (basic):" << message;
    m_ws_basic.write(message.append('\n'));
}

void SmartDeviceLink::buttons_send(QString message) {
    qDebug() << "Sending (buttons):" << message;
    m_ws_buttons.write(message.append('\n'));
}

void SmartDeviceLink::tts_send(QString message) {
    qDebug() << "Sending (tts):" << message;
    m_ws_tts.write(message.append('\n'));
}

void SmartDeviceLink::vehicle_send(QString message) {
    qDebug() << "Sending (vehicle):" << message;
    m_ws_vehicle.write(message.append('\n'));
}

void SmartDeviceLink::ui_send(QString message) {
    qDebug() << "Sending (ui):" << message;
    m_ws_ui.write(message.append('\n'));
}

void SmartDeviceLink::vr_send(QString message) {
    qDebug() << "Sending (vr):" << message;
    m_ws_vr.write(message.append('\n'));
}

void SmartDeviceLink::basic_receive(QString message) {
    qDebug() << "Receiving(basic):" << message;
    QJsonObject root = QJsonDocument::fromJson(message.toLocal8Bit()).object();
    int id = root.value("id").toDouble();
    int result = root.value("result").toDouble();
    QString method = root.value("method").toString();

    if (id == 600) {
        char *reg, *unreg, *update;

        asprintf(&reg, "{\"jsonrpc\":\"2.0\",\"id\":%i,\"method\":\"MB.subscribeTo\",\"params\":{\"propertyName\":\"BasicCommunication.OnAppRegistered\"}}", result+1);
        basic_send(reg);
        free(reg);

        asprintf(&unreg, "{\"jsonrpc\":\"2.0\",\"id\":%i,\"method\":\"MB.subscribeTo\",\"params\":{\"propertyName\":\"BasicCommunication.OnAppUnregistered\"}}", result+2);
        basic_send(unreg);
        free(unreg);

        asprintf(&update, "{\"jsonrpc\":\"2.0\",\"id\":%i,\"method\":\"MB.subscribeTo\",\"params\":{\"propertyName\":\"BasicCommunication.OnDeviceListUpdated\"}} ", result+3);
        basic_send(update);
        free(update);
    }
    else if (method.compare("BasicCommunication.OnAppRegistered") == 0) {
        QJsonObject params = root.value("params").toObject();
        QJsonObject application = params.value("application").toObject();
        QString appName = application.value("appName").toString();
        int appId = application.value("appId").toDouble();

        char * update;
        asprintf(&update, "{\"jsonrpc\":\"2.0\",\"id\":3000,\"method\":\"BasicCommunication.ActivateApp\",\"params\":{\"appName\":\"%s\",\"appId\":%i}}", appName.toLocal8Bit().data(), appId);
        basic_send(update);
        free(update);
        emit newAppAvailable(appId, appName);
    }
    else if (method.compare("BasicCommunication.OnAppUnregistered") == 0) {
        m_media_apps.clear();
    }
}

void SmartDeviceLink::buttons_receive(QString message) {
    qDebug() << "Receiving(buttons):" << message;
    QJsonObject root = QJsonDocument::fromJson(message.toLocal8Bit()).object();
    int id = root.value("id").toDouble();
    QString method = root.value("method").toString();

    if (method.compare("Buttons.GetCapabilities") == 0) {
        buttons_send("{\"jsonrpc\":\"2.0\",\"id\":19,\"result\":{\"capabilities\":[{\"name\":\"PRESET_0\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"PRESET_1\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"PRESET_2\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"PRESET_3\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"PRESET_4\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"PRESET_5\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"PRESET_6\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"PRESET_7\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"PRESET_8\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"PRESET_9\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"OK\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"SEEKLEFT\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"SEEKRIGHT\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"TUNEUP\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true},{\"name\":\"TUNEDOWN\",\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true}],\"presetBankCapabilities\":{\"onScreenPresetsAvailable\":true},\"method\":\"Buttons.GetCapabilitiesResponse\",\"resultCode\":\"SUCCESS\"}}");
    }
}

void SmartDeviceLink::tts_receive(QString message) {
    qDebug() << "Receiving(tts):" << message;
    QJsonObject root = QJsonDocument::fromJson(message.toLocal8Bit()).object();
    int id = root.value("id").toDouble();
    QString method = root.value("method").toString();

    if (method.compare("TTS.GetCapabilities") == 0) {
        tts_send("{\"jsonrpc\":\"2.0\",\"id\":18,\"result\":{\"capabilities\":[\"TEXT\"],\"resultCode\":\"SUCCESS\",\"method\":\"TTS.GetCapabilitiesResponse\"}}");
    }
    else if (method.compare("TTS.GetLanguage") == 0) {
        tts_send("{\"jsonrpc\":\"2.0\",\"id\":26,\"result\":{\"resultCode\":\"SUCCESS\",\"method\":\"TTS.GetLanguageResponse\",\"language\":\"EN-US\"}}");
    }
    else if (method.compare("TTS.GetSupportedLanguages") == 0) {
        tts_send("{\"jsonrpc\":\"2.0\",\"id\":22,\"result\":{\"resultCode\":\"SUCCESS\",\"method\":\"TTS.GetSupportedLanguagesResponse\",\"languages\":[\"EN-US\",\"ES-MX\",\"FR-CA\",\"DE-EU\",\"ES-EU\",\"EN-EU\",\"RU-RU\",\"TR-TR\",\"PL-EU\",\"FR-EU\",\"IT-EU\",\"SV-EU\",\"PT-EU\",\"NL-EU\",\"EN-AU\",\"ZH-CN\",\"ZH-TW\",\"JA-JP\",\"AR\",\"KO-KR\"]}}");
    }

}

void SmartDeviceLink::vehicle_receive(QString message) {
    qDebug() << "Receiving(vehicle):" << message;
    QJsonObject root = QJsonDocument::fromJson(message.toLocal8Bit()).object();
    int id = root.value("id").toDouble();
    QString method = root.value("method").toString();

    if (method.compare("VehicleInfo.GetVehicleType") == 0) {
        vehicle_send("{\"jsonrpc\":\"2.0\",\"id\":20,\"result\":{\"resultCode\":\"SUCCESS\",\"method\":\"VehicleInfo.GetVehicleTypeResponse\",\"vehicleType\":{\"make\":\"Ford\",\"model\":\"Fiesta\",\"modelYear\":\"2013\",\"trim\":\"SE\"}}}");
    }
}

void SmartDeviceLink::ui_receive(QString message) {
    qDebug() << "Receiving(ui):" << message;
    QJsonObject root = QJsonDocument::fromJson(message.toLocal8Bit()).object();
    int id = root.value("id").toDouble();
    int result = root.value("result").toDouble();
    QString method = root.value("method").toString();

    if (id == 400) {
        char *onchoise;
        asprintf(&onchoise, "{\"jsonrpc\":\"2.0\",\"id\":%i,\"method\":\"MB.subscribeTo\",\"params\":{\"propertyName\":\"VR.OnChoise\"}}", result + 1);
        ui_send(onchoise);
        free(onchoise);

        ui_send("{\"jsonrpc\":\"2.0\",\"method\":\"UI.OnReady\"}");
    }
    else if (method.compare("UI.GetCapabilities") == 0) {
        ui_send("{\"jsonrpc\":\"2.0\",\"id\":16,\"result\":{\"displayCapabilities\":{\"displayType\":\"GEN2_8_DMA\",\"textFields\":[{\"name\":\"mainField1\",\"characterSet\":\"TYPE2SET\",\"width\":1,\"rows\":1},{\"name\":\"mainField2\",\"characterSet\":\"TYPE2SET\",\"width\":1,\"rows\":1},{\"name\":\"statusBar\",\"characterSet\":\"TYPE2SET\",\"width\":1,\"rows\":1},{\"name\":\"mediaClock\",\"characterSet\":\"TYPE2SET\",\"width\":1,\"rows\":1},{\"name\":\"mediaTrack\",\"characterSet\":\"TYPE2SET\",\"width\":1,\"rows\":1},{\"name\":\"alertText1\",\"characterSet\":\"TYPE2SET\",\"width\":1,\"rows\":1},{\"name\":\"alertText2\",\"characterSet\":\"TYPE2SET\",\"width\":1,\"rows\":1}],\"mediaClockFormats\":[\"CLOCK1\",\"CLOCK2\",\"CLOCKTEXT1\",\"CLOCKTEXT2\",\"CLOCKTEXT3\"]},\"hmiZoneCapabilities\":[\"FRONT\",\"BACK\"],\"softButtonCapabilities\":[{\"shortPressAvailable\":true,\"longPressAvailable\":true,\"upDownAvailable\":true,\"imageSupported\":true}],\"method\":\"UI.GetCapabilitiesResponse\",\"resultCode\":\"SUCCESS\"}}");
    }
    else if (method.compare("UI.GetSupportedLanguages") == 0) {
        ui_send("{\"jsonrpc\":\"2.0\",\"id\":21,\"result\":{\"resultCode\":\"SUCCESS\",\"method\":\"UI.GetSupportedLanguagesResponse\",\"languages\":[\"EN-US\",\"ES-MX\",\"FR-CA\",\"DE-EU\",\"ES-EU\",\"EN-EU\",\"RU-RU\",\"TR-TR\",\"PL-EU\",\"FR-EU\",\"IT-EU\",\"SV-EU\",\"PT-EU\",\"NL-EU\",\"EN-AU\",\"ZH-CN\",\"ZH-TW\",\"JA-JP\",\"AR\",\"KO-KR\"]}}");
    }
    else if (method.compare("UI.GetLanguage") == 0) {
        ui_send("{\"jsonrpc\":\"2.0\",\"id\":24,\"result\":{\"resultCode\":\"SUCCESS\",\"method\":\"UI.GetLanguageResponse\",\"hmiDisplayLanguage\":\"EN-US\"}}");
    }
    else if (method.compare("UI.Show") == 0) {
        QJsonObject params = root.value("params").toObject();
        setShow1(params.value("mainField1").toString());
        setShow2(params.value("mainField2").toString());
        char* response;
        asprintf(&response, "{\"jsonrpc\":\"2.0\",\"id\":%i,\"result\":{\"resultCode\":\"SUCCESS\",\"method\":\"UI.ShowResponse\"}}", id);
        ui_send(response);
        free(response);
    }
    else if (method.compare("UI.Alert") == 0) {
        QJsonObject params = root.value("params").toObject();
        QString message(params.value("AlertText1").toString());
        message.append("\n").append(params.value("AlertText2").toString());
        char * response;
        asprintf(&response, "{\"jsonrpc\":\"2.0\",\"id\":%i,\"result\":{\"resultCode\":\"SUCCESS\",\"method\":\"UI.AlertResponse\"}}", id);
        ui_send(response);
        free(response);
        emit alert(message);
    }

}

void SmartDeviceLink::vr_receive(QString message) {
    qDebug() << "Receiving(vr):" << message;
    QJsonObject root = QJsonDocument::fromJson(message.toLocal8Bit()).object();
    int id = root.value("id").toDouble();
    QString method = root.value("method").toString();

    if (method.compare("VR.GetCapabilities") == 0) {
        vr_send("{\"jsonrpc\":\"2.0\",\"id\":17,\"result\":{\"capabilities\":[\"TEXT\"],\"method\":\"VR.GetCapabilitiesResponse\",\"resultCode\":\"SUCCESS\"}}");
    }
    else if (method.compare("VR.GetSupportedLanguages") == 0) {
        vr_send("{\"jsonrpc\":\"2.0\",\"id\":23,\"result\":{\"resultCode\":\"SUCCESS\",\"method\":\"VR.GetSupportedLanguagesResponse\",\"languages\":[\"EN-US\",\"ES-MX\",\"FR-CA\",\"DE-EU\",\"ES-EU\",\"EN-EU\",\"RU-RU\",\"TR-TR\",\"PL-EU\",\"FR-EU\",\"IT-EU\",\"SV-EU\",\"PT-EU\",\"NL-EU\",\"EN-AU\",\"ZH-CN\",\"ZH-TW\",\"JA-JP\",\"AR\",\"KO-KR\"]}}");
    }
    else if (method.compare("VR.GetLanguage") == 0) {
        vr_send("{\"jsonrpc\":\"2.0\",\"id\":25,\"result\":{\"resultCode\":\"SUCCESS\",\"method\":\"VR.GetLanguageResponse\",\"language\":\"EN-US\"}}");
    }
}

void SmartDeviceLink::registerApp(int appId, QString appName) {
    qDebug() << "New app registered:" << appName;
    m_media_apps.append(appId, appName);
}

void SmartDeviceLink::setShow1(QString show) {
    m_show1 = show;
    emit show1Changed(show);
}

void SmartDeviceLink::setShow2(QString show) {
    m_show2 = show;
    emit show2Changed(show);
}

void SmartDeviceLink::disconnected() {
    qDebug() << "Websocket disconnected";
}
