import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.0
import "."

Rectangle {
    id: root
    color: "black"
    width: parent.width
    height: Style.componentHeight
    anchors.margins: 10

    property int port: 1234
    property string ip: "127.0.0.1"
    property string logLocation: "."

    signal connect()

    FileDialog {
        id: fileDialog
        title: "Please choose a log file..."
        selectFolder: true
        folder: logLocation
        onAccepted: {
            var str = Qt.resolvedUrl( fileDialog.fileUrl ).toString();
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

            spacing: Style.spacing
            anchors.margins: 10
            anchors.topMargin: 10
            anchors.top: parent.top

            LabelText {
                id: ipInput

                dataText: root.ip
                textLabel: "IP:"
                onDataTextChanged: {
                    root.ip = dataText;
                }
            }

            LabelText {
                id: portInput

                dataText: root.port
                textLabel: "PORT:"
                onDataTextChanged: {
                    root.port = parseInt(dataText, 10);
                }
            }

            CastomButton {
                id: okButton
                width: Style.buttonWidth
                text: "Create Connection"
                onClicked: connect()
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
