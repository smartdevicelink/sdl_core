import QtQuick 2.0


Item
{
    id: climateControlBtn
    width: climateBtnImg.width
    height: climateBtnImg.height
    property string name: ""
    property string txt: ""
    state: "off"
    Image {
        id: climateBtnImg
        source: climateControlBtn.state === "off" ? "../res/climate/climate_round_off_btn.png" : "../res/climate/climate_round_on_btn.png"

        Image {
            id: hoverImg
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            property string st: climateControlBtn.state === "off" ? "_off" : "_on"
            source: climateControlBtn.name === "" ? "": "../res/climate/" + climateControlBtn.name + st + ".png"
        }
        Text {
            id: hoverText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: climateControlBtn.txt
            font.pixelSize: 25
            color: climateControlBtn.state === "off" ? "#1d81d5" : "black"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(climateControlBtn.state === "on") {
                    climateControlBtn.state = "off"
                    climateBtnImg.source = "../res/climate/climate_round_off_btn.png"
                    if(climateControlBtn.txt != "") {
                        hoverText.color = "#1d81d5"
                    }
                    if(climateControlBtn.name != "") {
                        hoverImg.source = "../res/climate/" + climateControlBtn.name + "_off.png"
                    }
                } else {
                    climateControlBtn.state = "on"
                    climateBtnImg.source = "../res/climate/climate_round_on_btn.png"
                    if(climateControlBtn.txt != "") {
                        hoverText.color = "black"
                    }
                    if(climateControlBtn.name != "") {
                        hoverImg.source = "../res/climate/" + climateControlBtn.name + "_on.png"
                    }
                }
            }
        }
    }


}
