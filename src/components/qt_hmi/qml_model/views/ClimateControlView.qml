import QtQuick 2.0
import "../controls"

Item{
    id: climatMenu
    anchors.fill: parent

    Item {
        // 3/4 top screen
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left

        Column {
            id: leftItem
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 1/4*parent.width - useThisToGetButtonSize.width

            Row {
                ClimateControlBtn {name: "heated_seat"}
                ClimateControlBtn {name: "heated_wheel"}
            }
            Row {
                ClimateControlBtn {name: "AC"}
                ClimateControlBtn {name: "headed_dash"}
            }
        }

        Column{
            id: centralItem
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            ClimateControlBtn {id: useThisToGetButtonSize; name: "defrost"; state: "on"}
            ClimateControlBtn {name: "face"; state: "on"}
            ClimateControlBtn {name: "feet"}
        }

        Row {
            id: rightItem
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 1/4*parent.width - useThisToGetButtonSize.width

            ClimateControlBtn {
                anchors.verticalCenter: parent.verticalCenter
                txt: "Dual"
                state: "on"
            }

            Column{
                ClimateControlBtn {name: "heated_seat"}
                ClimateControlBtn {name: "AC"}
            }
        }
    }
    // bottom 1\4 of screen doesnt use now
}
