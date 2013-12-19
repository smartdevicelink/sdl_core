import QtQuick 2.0

ListView {
    id: listView
    anchors.fill: parent
    anchors.topMargin: 120
    spacing: 10
    clip: true
    model: MediaModel

    delegate: Button {
        width: 500
        anchors.horizontalCenter: parent.horizontalCenter
        OverflowableText {
            text: name
            fontColor: "#fff"
            font.pixelSize: 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
            width: parent.width - 40
        }
        Text {
            text: (model.count === 0) ? "" : model.count
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            color: "#fff"
        }

        onPress: {
            if (model.is_file) {
                MediaPlayer.setPlaylist(model.pid, model.index);
                usb.state = '';
            } else {
                MediaModel.browseToChild(model.id)
            }
        }
    }
}
