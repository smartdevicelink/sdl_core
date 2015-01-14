// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {
    id: button
    property string text: ""
    property int centerWidth: 2
    property alias touchBtn: myButton
    property alias pressed: myButton.pressed

    FWdgtImage{
        id: img
        x:-8
        y:-8
        bitmap: "Radio/ButtonEnd_8_8.png"
    }

    FWdgtImage{
        id: img1
        anchors.left: img.right
        anchors.top: img.top
        width: centerWidth
        fillMode: Image.Stretch
        bitmap: "Radio/ButtonCenter_0_8.png"
    }

    FWdgtImage{
        id: img2
        anchors.left: img1.right
        anchors.top: img1.top
        bitmap: "Radio/ButtonOtherEnd_8_8.png"
    }

    FWdgtTouchArea {
        id: myButton
        width:img1.width+44
        height: 46
        anchors.left: img1.left
        anchors.leftMargin: -22
        anchors.top: img1.top
        anchors.topMargin: 8
    }

    Rectangle {
        id: btnFill
        color: "#1d81d5"
        anchors.fill: myButton
        radius: 23
        border.width: 1
        border.color: "#1d81d5"
        visible: false
    }

    BaseText {
        id: btnTxt
        anchors.centerIn: btnFill
        text: button.text
        color: "#1d81d5"
        visible: false
    }



    FViewUseCaseSimple{
        condition: button.text !== ""
        PropertyChanges {
            target: btnTxt; visible: true
        }
    }

    FViewUseCaseGroup{
        FViewUseCase{condition: myButton.pressed === true
            PropertyChanges {
                target: btnFill; visible: true
            }
            PropertyChanges {
                target: btnTxt; color: "black"
            }
        }
        FViewUseCase{condition: indicatorState === true
          /*  PropertyChanges {
                target: buttonArea; color: pressedColor
            }*/
        }
        FViewUseCase{condition: myButton.pressed === false
            PropertyChanges {
                target: btnFill; visible: false
            }
        }
    }

}
