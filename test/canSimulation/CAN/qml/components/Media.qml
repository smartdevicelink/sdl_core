import QtQuick 2.0
import QtQuick.Controls 1.1

Rectangle {
    id: root
    color: "red"

    anchors.margins: 10

    Button {
        id: tuneButton
        width: 120
        text: "Tune"
        anchors.topMargin: 10
    }

    TextInput {
        id: tuneInput
        width: 120
        text: "105.3"
        anchors.left: tuneButton.right
        anchors.margins: 10
    }

    Button {
        id: startScan
        width: 120
        text: "Start Scan"
        anchors.top: tuneButton.bottom
    }

    Button {
        id: stopScan
        width: 120
        text: "Stop Scan"
        anchors.top: startScan.bottom
    }

    Button {
        id: presetButton
        width: 120
        text: "Preset"
        anchors.top: stopScan.bottom
    }
}
