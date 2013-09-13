import QtQuick 2.0
Item{
    anchors.fill: parent

    Item{
        x: centralItem.x - childrenRect.width - 40
        anchors.verticalCenter: centralItem.verticalCenter
        width: childrenRect.width
        height: childrenRect.height

        Column {
            Row {
                ClimateControlBtn {name: "heated_seat"}
                ClimateControlBtn {name: "heated_wheel"}
            }
            Row {
                ClimateControlBtn {name: "AC"}
                ClimateControlBtn {name: "headed_dash"}
            }
        }
    }

    Item{
        id: centralItem
        width: childrenRect.width
        height: childrenRect.height
        anchors.horizontalCenter: parent.horizontalCenter

        Column{
            ClimateControlBtn {name: "defrost"; state: "on"}
            ClimateControlBtn {name: "face"; state: "on"}
            ClimateControlBtn {name: "feet"}
        }

    }

    Item {
        x: centralItem.x + centralItem.width + 40
        anchors.verticalCenter: centralItem.verticalCenter
        width: childrenRect.width
        height: childrenRect.height
        Item {
            id: dualBtn
            width: childrenRect.width
            height: childrenRect.height
            anchors.verticalCenter: parent.verticalCenter
            ClimateControlBtn {txt: "Dual"; state: "on"}
        }
        Item {
            x: dualBtn.width
            anchors.verticalCenter: parent.verticalCenter
            width: childrenRect.width
            height: childrenRect.height
            Column {
                    ClimateControlBtn {name: "heated_seat"}
                    ClimateControlBtn {name: "AC"}
            }
        }
    }
}
