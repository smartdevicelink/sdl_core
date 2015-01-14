// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {
    id: button
    property string bitmap: ""
    property string bitmap2: ""
    property bool vis: true
    property string text: ""
    property string text1: ""
    property string text2: ""
    property int centerWidth: 2
    property alias touchBtn: myButton
    property int imageShift: 0
    property alias spacing: btnArea.spacing
    property alias pressed: myButton.pressed
    property int curHD: 4
    property int totHD: 4

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


    Row {
        id: btnArea
        anchors.fill: img1
        anchors.left: img1.left
        anchors.leftMargin: imageShift
        spacing: 2


        FWdgtImage{
            id: btnImg
            anchors.verticalCenter: btnArea.verticalCenter
            bitmap: button.bitmap
            visible: false
        }

        BaseText {
            id: btnTxt
            anchors.verticalCenter: btnArea.verticalCenter
            text: button.text
            color: "#1d81d5"
            visible: false
        }

        BaseText {
            id: btnTxt1
            anchors.verticalCenter: btnArea.verticalCenter
            text: button.text1
            color: "white"
            visible: false
            theFont: "../Fonts/HelveticaNeueLTStd-Bold.ttf"
            //boldTx: true
        }

        BaseText {
            id: btnTxt2
            anchors.verticalCenter: btnArea.verticalCenter
            text: button.text2
            color: "#1d81d5"
            visible: false
        }


    }

    FViewUseCaseSimple{
        condition: button.bitmap !== ""
        PropertyChanges {
            target: btnImg; visible: true
        }
    }

    FViewUseCaseSimple{
        condition: button.text !== ""
        PropertyChanges {
            target: btnTxt; visible: true
        }
    }

    FViewUseCaseSimple{
        condition: button.text1 !== ""
        PropertyChanges {
            target: btnTxt1; visible: true
        }
    }

    FViewUseCaseSimple{
        condition: button.text2 !== ""
        PropertyChanges {
            target: btnTxt2; visible: true
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
            PropertyChanges {
                target: btnTxt2; color: "black"
            }
            PropertyChanges {
                target: btnImg; bitmap: button.bitmap2
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


    FViewUseCaseGroup{
        FViewUseCase{condition: totHD === -1
            PropertyChanges {
                target: button; visible: true
            }
        }
        FViewUseCase{condition: totHD === 0
            PropertyChanges {
                target: button; visible: false
            }
        }
        FViewUseCase{condition: totHD === 1
            PropertyChanges {
                target: myButton; enabled: false
            }
            PropertyChanges {
                target: btnTxt; text: ""
            }
            PropertyChanges {
                target: btnTxt1; text: ""
            }
            PropertyChanges {
                target: btnTxt2; text: ""
            }
            PropertyChanges {
                target: img1; width: 4
            }
        }
        FViewUseCase{condition:  totHD === 2 && curHD === 1
            PropertyChanges {
                target: img1; width: 50
            }
            PropertyChanges {
                target: btnTxt; text: ""
            }
            PropertyChanges {
                target: btnTxt1; text: " 1"
            }
            PropertyChanges {
                target: btnTxt2; text: " 2"
            }
        }
        FViewUseCase{condition:  totHD === 2 && curHD === 2
            PropertyChanges {
                target: img1; width: 50
            }
            PropertyChanges {
                target: btnTxt; text: " 1 "
            }
            PropertyChanges {
                target: btnTxt1; text: "2"
            }
            PropertyChanges {
                target: btnTxt2; text: ""
            }
        }
        FViewUseCase{condition:  totHD === 3 &&  curHD === 1
            PropertyChanges {
                target: img1; width: 65
            }
            PropertyChanges {
                target: btnTxt; text: ""
            }
            PropertyChanges {
                target: btnTxt1; text: " 1"
            }
            PropertyChanges {
                target: btnTxt2; text: " 2 3"
            }
        }
        FViewUseCase{condition:  totHD === 3 && curHD === 2
            PropertyChanges {
                target: img1; width: 65
            }
            PropertyChanges {
                target: btnTxt; text: " 1 "
            }
            PropertyChanges {
                target: btnTxt1; text: "2"
            }
            PropertyChanges {
                target: btnTxt2; text: " 3"
            }
        }
        FViewUseCase{condition:  totHD === 3 && curHD === 3
            PropertyChanges {
                target: img1; width: 65
            }
            PropertyChanges {
                target: btnTxt; text: " 1 2 "
            }
            PropertyChanges {
                target: btnTxt1; text: "3"
            }
            PropertyChanges {
                target: btnTxt2; text: ""
            }
        }
        FViewUseCase{condition:  totHD === 4 && curHD === 1
            PropertyChanges {
                target: img1; width: 85
            }
            PropertyChanges {
                target: btnTxt; text: ""
            }
            PropertyChanges {
                target: btnTxt1; text: " 1"
            }
            PropertyChanges {
                target: btnTxt2; text: " 2 3 4"
            }
        }
        FViewUseCase{condition:  totHD === 4 && curHD === 2
            PropertyChanges {
                target: img1; width: 85
            }
            PropertyChanges {
                target: btnTxt; text: " 1 "
            }
            PropertyChanges {
                target: btnTxt1; text: "2"
            }
            PropertyChanges {
                target: btnTxt2; text: " 3 4"
            }
        }
        FViewUseCase{condition:  totHD === 4 && curHD === 3
            PropertyChanges {
                target: img1; width: 85
            }
            PropertyChanges {
                target: btnTxt; text: " 1 2 "
            }
            PropertyChanges {
                target: btnTxt1; text: "3"
            }
            PropertyChanges {
                target: btnTxt2; text: " 4"
            }
        }
        FViewUseCase{condition:  totHD === 4 && curHD === 4
            PropertyChanges {
                target: img1; width: 85
            }
            PropertyChanges {
                target: btnTxt; text: " 1 2 3 "
            }
            PropertyChanges {
                target: btnTxt1; text: "4"
            }
            PropertyChanges {
                target: btnTxt2; text: ""
            }
        }

    }

}
