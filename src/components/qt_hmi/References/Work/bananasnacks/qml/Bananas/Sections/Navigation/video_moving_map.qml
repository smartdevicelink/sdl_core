import QtQuick 2.0
import "../../Components"
import QtMultimedia 5.0

Item {
    Video {
        anchors.fill: parent
        source: "rtsp://172.20.10.7:8086"
        autoPlay: true
    }

    Button {
        text: "Go"
        anchors.top: parent.top
        anchors.topMargin: 3
        anchors.right: parent.right
        anchors.rightMargin: 20
        onPress: section.state = 'menu'
    }
}
