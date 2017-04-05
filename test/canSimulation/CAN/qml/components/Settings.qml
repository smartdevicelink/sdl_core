import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.0
import "."

Rectangle {
    id: root
    width: parent.width
    height: Style.componentHeight
    anchors.margins: 10

    property int port: 8092 //Default port for TCP connection
    property string ip: "127.0.0.1" //Default IP adress for TCP connection
    property int wsPort: 2468 //Default port for WS connection
    property string wsIP: "127.0.0.1" //Default IP adress for WS connection
    property string logLocation: "."

    signal connectTCP()
    signal connectWS()

    FileDialog {
        id: fileDialog
        title: "Please choose a log file..."
        selectFolder: true
        folder: logLocation
        onAccepted: {
            var str = Qt.resolvedUrl( fileDialog.fileUrl ).toString();

            //remove default QT appender before current location path
            // and added log file name "log.txt"
            logLocation = str.slice(7, str.length) + "/log.txt";
        }
    }

    gradient: Gradient {
        GradientStop { position: 0 ; color: Style.colorMainDarkGradient}
        GradientStop { position: 0.2 ; color: Style.colorMainLightGradient}
        GradientStop { position: 0.3 ; color: Style.colorMainLightGradient}
        GradientStop { position: 1 ; color: Style.colorMainDarkGradient}
    }

    ScrollView {

        width: parent.width
        height: parent.height

        Column {

            spacing: Style.columnSpacing
            anchors.margins: 10
            anchors.topMargin: 10
            anchors.top: parent.top

            LabelText {
                id: ipInput

                dataText: root.ip
                textLabel: "TCP IP:"
                onDataTextChanged: {
                    root.ip = dataText;
                }
            }

            LabelText {
                id: portInput

                dataText: root.port
                textLabel: "TCP PORT:"
                onDataTextChanged: {
                    root.port = parseInt(dataText, 10);
                }
            }

            LabelText {
                id: ipWSInput

                dataText: root.wsIP
                textLabel: "WS IP:"
                onDataTextChanged: {
                    root.wsIP = dataText;
                }
            }

            LabelText {
                id: portWSInput

                dataText: root.wsPort
                textLabel: "WS PORT:"
                onDataTextChanged: {
                    root.wsPort = parseInt(dataText, 10);
                }
            }

            CastomButton {
                id: okTCPButton
                width: Style.buttonWidth
                text: "Create Connection TCP"
                onClicked: connectTCP()
            }

            CastomButton {
                id: okWSButton
                width: Style.buttonWidth
                text: "Create Connection WS"
                onClicked: connectWS()
            }

            CastomButton {
                id: openDialog
                width: Style.buttonWidth
                text: "Browse log location"
                onClicked: fileDialog.open()
            }

            Text {
                id: locationText
                text: "Current log loction is: " + logLocation
            }
        }
    }
}
