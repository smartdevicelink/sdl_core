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
            clip: true
            model: ContactsListModel {}
            spacing: 25
            section.property: "name"
            section.criteria: ViewSection.FirstCharacter
            section.delegate: Text {
                color: "blue"
                font.pixelSize: 20
                text: section
            }

            function setCurrent(firstLetter){
                for(var i = 0; i < count; i ++){
                    if(model.get(i).name[0] === firstLetter){
                        positionViewAtIndex(i, ListView.Beginning)
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

    Item {
        // 1/4 bottom screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        BackButton{ anchors.horizontalCenter: parent.horizontalCenter }
    }
}
