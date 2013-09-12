import QtQuick 2.0
Item{
    anchors.fill: parent
    Item{
        x: 100
        anchors.verticalCenter: centralItem.verticalCenter
        width: 200
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
        width: 100
        height: 100
        anchors.horizontalCenter: parent.horizontalCenter
        Column{
            ClimateControlBtn {name: "defrost"; state: "on"}
            ClimateControlBtn {name: "face"; state: "on"}
            ClimateControlBtn {name: "feet"}
        }
    }

    Item {
        x: 500
        anchors.verticalCenter: centralItem.verticalCenter + 100
        width: 100
        height: 200
        Item {
            anchors.verticalCenter: parent.verticalCenter
            ClimateControlBtn {txt: "Dual"; state: "on"}
        }
        Item {
            x: 100
            y: 50
            width: 100
            height: 100
            Column {
                    ClimateControlBtn {name: "heated_seat"}
                    ClimateControlBtn {name: "AC"}
            }
        }
    }
}
