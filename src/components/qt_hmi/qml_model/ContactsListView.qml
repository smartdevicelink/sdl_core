import QtQuick 2.0

Item {
    anchors.fill: parent
    property alias contactsListView: contactsListView

    ListView {
        anchors.centerIn: parent
        width: parent.width - 100
        height: parent.height
        id: contactsListView
        model: ContactsListModel {}
        spacing: 25

        function setCurrent(btnText){
            var i
            for(i = 0; i <contactsListView.count; i ++){
                if(contactsListView.model.get(i).name.substring(0, 1) === btnText){
                    contactsListView.currentIndex = i
                    if(contactsListView.currentIndex < contactsListView.count- 6) contactsListView.currentIndex += 5
                    break;
                }
            }
        }


        delegate: Item {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: typeText.height

            Text  {
                text: name
                color: "lightblue"
                font.pixelSize: 25
                anchors.left: parent.left
            }
            Text  {
                id: phoneText
                text: phone
                color: "lightblue"
                font.pixelSize: 25
                anchors.left: parent.horizontalCenter
            }
            Text  {
                id: typeText
                text: type
                color: "lightblue"
                font.pixelSize: 25
                anchors.right: parent.right
            }
        }

    }

    /*Text{
        id: backtext
        anchors.horizontalCenter: parent.horizontalCenter
        y: contanctsListView.height
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
    }*/
}
