import QtQuick 2.0

GridView
{
    id: musicSourceGridView
    anchors.fill: parent
    cellWidth: width / 3
    cellHeight: height / 3

    model: MusicSourceModel {}

    delegate: Item {
        width: musicSourceGridView.cellWidth
        height: musicSourceGridView.cellHeight

        MouseArea {
            cursorShape: Qt.PointingHandCursor
            anchors.fill: parent
            onClicked: {
                menuContainer.go(qml)
                if(menuContainer.item.radioType !== undefined) menuContainer.item.radioType = name;
                if(menuContainer.item.playerType !== undefined) menuContainer.item.playerType = name;
            }
        }

        Image { source: icon; anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter;}
        Text {text: name; font.pixelSize: 25; color: "#1d81d5"; anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter;}
    }

    Text{
        id: backtext
        anchors.horizontalCenter: parent.horizontalCenter
        y: musicSourceGridView.height
        text: "Back"
        color: "#1d81d5"
        font.pixelSize: 30
        Image{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: backtext.verticalCenter
            source: "res/buttons/long_oval_btn.png"
        }
        MouseArea {
          anchors.fill: parent
          onClicked: {
              menuContainer.back()
            }
        }
    }

}
