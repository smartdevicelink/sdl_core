import QtQuick 2.0


Item {
    id: navigationKeyboard

    width: 800
    height: 480

    anchors.centerIn: parent

    Column {

        Row {
            //x:61
           // y:211



            Repeater {
                model: 10
                Image {
                    Text{
                        anchors.centerIn: parent
                        text: "W"
                        color: "steelblue"
                        font.pixelSize: 30
                    }

                    id: wBtn
                    source: "res/buttons/key_9_9.png"
                }

            }

        }

        Row {

        }

        Row {

        }

    }

}




