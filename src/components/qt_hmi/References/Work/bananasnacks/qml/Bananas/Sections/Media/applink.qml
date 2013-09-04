import QtQuick 2.0
import com.ford.sdlcore 1.0
import "../../Components"
import "../../Layouts"

MediaPlayerLayout {
    header: "AppLink"
    id: applink

    Column {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20

        Text {
            id: title
            text: AppLink.show1
            color: "#ffffff"
            font.pixelSize: 28
            anchors.leftMargin: 20
        }
        Text {
            id: artist
            text: AppLink.show2
            color: "#ffffff"
            font.pixelSize: 18
        }
    }
}
