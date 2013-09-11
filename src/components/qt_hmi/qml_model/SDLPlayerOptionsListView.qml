import QtQuick 2.0

Item {
    anchors.fill: parent

    ListView {
        id: sdlPlayerOptionsListView
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width
        height:parent.height

        model: ListModel{
            ListElement{
                name: "Policies Test"
            }
            ListElement{
                name: "XML Test"
            }
        }


        delegate: Text  {
            text: name
            color: "#1d81d5"
            font.pixelSize: 40
            MouseArea {
              anchors.fill: parent
              onClicked: {
                }
            }
        }
    }

    Text{
        id: backtext
        anchors.horizontalCenter: parent.horizontalCenter
        y: sdlPlayerOptionsListView.height
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
