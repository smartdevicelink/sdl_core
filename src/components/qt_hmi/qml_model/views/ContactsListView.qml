import QtQuick 2.0
import "../controls"
import "../models"

Item{
    anchors.fill: parent
    property alias contactsListView: contactsListView

    Item {
        // 3/4 top screen
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        ListView {
            anchors.fill: parent
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
    }

    Item{
        // 1/4 bottom screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        BackBtn{y: parent.height + contactsListView.spacing}
    }
}
