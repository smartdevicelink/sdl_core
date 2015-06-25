import QtQuick 2.2
import QtQuick.Controls 1.1
import "components" 1.0
import "."

ApplicationWindow {

    id: root
    visible: true
    width: 800
    height: Style.windowHeight
    title: qsTr("Can Simulation      by Luxoft")

    property ListModel logModel: ListModel { }
    property string logURL: "log.txt"

    //signal viewClicked(string name)
    signal createConnectionTCP(string ip, int port)
    signal createConnectionWS(string ip, int port)
    signal sendMessageTCP(string message)
    signal sendMessageWS(string message)
    signal saveLogToFile(string source, string data)

    Connections {
        target: RequestHandler
        onSendMessageHMI: {

            var messageJson = JSON.stringify(message);

            sendMessageWS(messageJson);
        }
    }

    Connections {
        target: RequestHandler
        onSendMessageSDL: {

            var messageJson = JSON.stringify(message);

            sendMessageTCP(messageJson);
        }
    }

    /**
     * Method to show current location of CAN on SettingView
     * @param path - Location path
     */
    function setCurrentPath(path) {
        root.logURL = path + "/log.txt";
        logModel.append({
                            "color": Style.colorLogError,
                            "text": "Carrent log location: " + root.logURL
                        });
    }

    /**
     * Method to redirect incoming fom WS messages to RequestHandler
     * @param message - Text message came from WS
     */
    function incomingWS(message) {

        var result = JSON.parse(message);

        RequestHandler.receivedMessageWS(result);
    }

    /**
     * Method to redirect incoming fom TCP messages to RequestHandler
     * and send response back to TCP client
     * @param message - Text message came from TCP
     */
    function incomingTCP(message) {

        var result = JSON.parse(message),
            resp,
            respJson;

        resp = RequestHandler.receivedMessageTCP(result);

        if (resp !== false) {

            respJson = JSON.stringify(resp);

            sendMessageTCP(respJson);
        }

    }

    /**
     * Method to add new messages to LogView
     * @param message - Text message to show in Logger
     * @param color - Number value of color Enum
     */
    function logger(message, color) {

        tcpLogsView.append(message, color);
        return true;
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    TabView {

        id: viewsTab
        width: parent.width
        height: root.height - 47 - tcpLogsView.height

        Tab {
            title: "Media"
            Media {
                id: mediaComponent

                onRequestButtonClick: {
                    switch (item.objectName) {
                        case "OnRadioDetails": {

                            RequestHandler.onRadioDetails();

                            break;
                        }
                        default: {

                            //Default behavior for other cases
                            break;
                        }
                    }
                }

                onResponseButtonClick: {

                    var notify;

                    switch (item.objectName) {
                        case "TuneUp": {

                            RequestHandler.tuneUp();

                            break;
                        }
                        case "TuneDown": {

                            RequestHandler.tuneDown();

                            break;
                        }
                        case "StartScan": {

                            notify = {
                                "jsonrpc": "2.0",
                                "method": "CAN.StartScan"
                            }

                            RequestHandler.sendMessageHMI(notify);

                            RequestHandler.startScan();

                            break;
                        }
                        case "StopScan": {

                            notify = {
                                "jsonrpc": "2.0",
                                "method": "CAN.StopScan"
                            }

                            RequestHandler.sendMessageHMI(notify);

                            RequestHandler.stopScan();

                            break;
                        }
                        default: {

                            //RequestHandler.func();
                            break;
                        }
                    }
                }
            }
        }
        Tab {
            title: "Climate"
            Rectangle {
                gradient: Gradient {
                    GradientStop { position: 0 ; color: Style.colorMainDarkGradient}
                    GradientStop { position: 0.1 ; color: Style.colorMainLightGradient}
                    GradientStop { position: 0.2 ; color: Style.colorMainLightGradient}
                    GradientStop { position: 1 ; color: Style.colorMainDarkGradient}
                }
            }
        }
        Tab {
            title: "Navigation"
            Rectangle {
                gradient: Gradient {
                    GradientStop { position: 0 ; color: Style.colorMainDarkGradient}
                    GradientStop { position: 0.6 ; color: Style.colorMainLightGradient}
                    GradientStop { position: 0.7 ; color: Style.colorMainLightGradient}
                    GradientStop { position: 1 ; color: Style.colorMainDarkGradient}
                }
            }
        }
        Tab {
            title: "Phone"
            Rectangle {
                gradient: Gradient {
                    GradientStop { position: 0 ; color: Style.colorMainLightGradient}
                    GradientStop { position: 0.2 ; color: Style.colorMainLightGradient}
                    GradientStop { position: 1 ; color: Style.colorMainDarkGradient}
                }
            }
        }
        Tab {
            title: "MCS"
            Rectangle {
                gradient: Gradient {
                    GradientStop { position: 0 ; color: Style.colorMainDarkGradient}
                    GradientStop { position: 0.7 ; color: Style.colorMainLightGradient}
                    GradientStop { position: 0.8 ; color: Style.colorMainLightGradient}
                    GradientStop { position: 1 ; color: Style.colorMainDarkGradient}
                }
            }
        }
        Tab {
            title: "Settings"
            Settings {
                id: settingsComponent

                Component.onCompleted: {

                    logLocation = root.logURL
                }
                onConnectTCP: {
                    createConnectionTCP(settingsComponent.ip, settingsComponent.port);
                }
                onConnectWS: {
                    createConnectionWS(settingsComponent.wsIP, settingsComponent.wsPort);
                }
                onLogLocationChanged: {
                    root.logURL = logLocation
                    logModel.append({"color":Style.colorLogError, "text": "You chose: " + root.logURL});
                }
            }
        }
    }

    Rectangle {
        id: appearRect

        width: root.width
        anchors.left: root.left
        anchors.right: root.right
        height: 20
        anchors.top: viewsTab.bottom
        color: Style.colorMainDarkGradient
        border.color: Style.colorButtonGradBot

        Text {
            text: "Logger"
            color: Style.colorButtonText
        }

        CastomButton {
            height: parent.height
            text: "Save Log"
            id: saveLog
            anchors.right: parent.right
            anchors.top: parent.top
            z: parent.z + 1
            onClicked: {

                var data;

                for (var i=0; i < logModel.count; i++) {
                    data += "\n";
                    data += logModel.get(i).text;
                }

                saveLogToFile(root.logURL, data);
                logModel.clear();
            }
        }

        CastomButton {
            height: parent.height
            text: "Clear Log"
            anchors.right: saveLog.left
            anchors.top: parent.top
            z: parent.z + 1
            onClicked: {
                logModel.clear();
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (tcpLogsView.height === 0) {
                    tcpLogsView.height = 150
                } else {

                    tcpLogsView.height = 0
                }

            }
        }
    }

    Rectangle {

        id: tcpLogsView
        width: parent.width
        anchors.top: appearRect.bottom
        height: 00
        visible: true

        gradient: Gradient {
            GradientStop { position: 0 ; color: Style.colorMainDarkGradient}
            GradientStop { position: 0.1 ; color: Style.colorMainLightGradient}
            GradientStop { position: 0.8 ; color: Style.colorMainLightGradient}
            GradientStop { position: 1 ; color: Style.colorMainDarkGradient}
        }

        Component {
            id: logDelegate
            Rectangle {
                color: "transparent"
                width: tcpLogsView.width
                height: logItemText.height
                Text {
                    id: logItemText
                    width: tcpLogsView.width;
                    color: model.color
                    text: model.text
                    wrapMode: Text.WordWrap
                    font.family: "Helvetica"
                }
            }
        }

        ListView {
            id: logView
            delegate: logDelegate
            model: logModel
            anchors.fill: parent
            clip: true

            highlight: highlight
            highlightFollowsCurrentItem: true
            focus: true
            onCountChanged: {
                //Change index for auto scrolling messages in Logger
                currentIndex = count > 0 ? count - 1 : 0;
            }
        }

        function append(message, colorEnum) {
            var colorString;
            switch (colorEnum) {
            case 0:
                colorString =  Style.colorLogError
                break;

            case 1:
                colorString =  Style.colorLogReceived
                break;

            case 2:
                colorString = Style.colorLogSend
                break;
            }

            logModel.append({"color":colorString, "text": message});
        }
    }
}
