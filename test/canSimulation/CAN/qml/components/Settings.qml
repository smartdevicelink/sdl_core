import QtQuick 2.0
import QtQuick.Controls 1.1
import "."

Rectangle {
    id: root
    color: "black"
    width: parent.width
    height: Style.componentHeight
    anchors.margins: 10

    property int port: 1234
    property string ip: "127.0.0.1"

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

            Button {
                id: okButton
                width: Style.buttonWidth
                text: "Create Connection"
                onClicked: connect()
            }
        }
    }

    signal connect()
}
