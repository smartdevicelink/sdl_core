import QtQuick 2.0

Item {
    id: navigationKeyboard

    width: 800
    height: 480

    anchors.centerIn: parent

        Row {
            x:37.5
            y:122.5
            Image {
                //x: parent.x + 69
                Text{
                    anchors.centerIn: parent
                    text: "Q"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: qBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
               // x: parent.x + 69
                Text{
                    anchors.centerIn: parent
                    text: "W"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: wBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
              //  x: parent.x + 69
                Text{
                    anchors.centerIn: parent
                    text: "E"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: eBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "R"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: rBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "T"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: tBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "Y"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: yBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "U"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: uBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "I"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: iBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "O"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: oBtn
                source: "res/buttons/key_9_9.png"
             }

            Image {
                Text{
                    anchors.centerIn: parent
                    text: "P"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: pBtn
                source: "res/buttons/key_9_9.png"
             }
        }

        Row {
            x:0
            y:183.5

            Image {
                Text{
                    anchors.centerIn: parent
                    text: "123"
                    color: "#1d81d5"
                    font.pixelSize: 20
                }

                id: numbersBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "A"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: aBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "S"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: sBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "D"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: dBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "F"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: fBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "G"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: gBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "H"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: hBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "J"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: jBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "K"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: kBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "L"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: lBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                id: bspaceBtn
                source: "res/buttons/key_9_9.png"

                Image{
                    anchors.centerIn: parent
                    source: "res/buttons/deleteIcon.png"
                }

             }
        }

        Row {
            x: 37.5
            y: 243.5

            Image {
                Text{
                    anchors.centerIn: parent
                    text: "!@#"
                    color: "#1d81d5"
                    font.pixelSize: 20
                }

                id: symbolBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "Z"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: zBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "X"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: xBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "C"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: cBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "V"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: vBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "B"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: bBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "N"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: nBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "M"
                    color: "#1d81d5"
                    font.pixelSize: 30
                }

                id: mBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "Clear"
                    color: "#1d81d5"
                    font.pixelSize: 15
                }

                id: clearBtn
                source: "res/buttons/key_9_9.png"
             }
            Image {
                Text{
                    anchors.centerIn: parent
                    text: "Space"
                    color: "#1d81d5"
                    font.pixelSize: 15
                }

                id: spacelearBtn
                source: "res/buttons/key_9_9.png"
             }



        }


}




