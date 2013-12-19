// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {
    id: button
    property string bitmap: ""
    property string bitmap2: ""
    property bool selected: false
    property alias touchBtn: myButton
    property string text: ""
    property string image: "climate/Button_9_9.png"
    property string imageActive: "climate/ButtonActive_0_0.png"
    property string imagePressed: "climate/ButtonPressed_9_9.png"
    property string imageInActive: "climate/ButtonInActive_0_0.png"
    property bool active: true
    property string newFont: "../Fonts/HelveticaNeueLTStd-Roman.ttf"
    property int txtSize: 24
    property string txtColor: "#1d81d5"


    FWdgtImage{
        id: img
        x:-9
        y:-9
        bitmap: image
    }

    FWdgtImage{
        id: imgSelected
        x:0
        y:0
        bitmap: imageActive
        visible: false
    }

    FWdgtImage{
        id: imgInBtn
        anchors.centerIn: imgSelected
        bitmap: button.bitmap
        visible: false
        //height: 10
        //width: 10
    }

    BaseText {
        id: btnTxt
        anchors.horizontalCenter: imgSelected.horizontalCenter
        anchors.verticalCenter: imgSelected.verticalCenter
        //anchors.verticalCenterOffset: -1
        vertAlignment: Text.AlignVCenter
        horAlignment: Text.AlignHCenter
        text: button.text
        color: button.txtColor
        visible: false
        theFont: button.newFont
        pxSize: button.txtSize

    }

    FWdgtTouchArea {
        id: myButton
        anchors.fill: imgSelected
    }



    FViewUseCaseSimple{
        condition: button.text !== ""
        PropertyChanges {
            target: btnTxt; visible: true
        }
    }

    FViewUseCaseSimple{
        condition: button.bitmap !== ""
        PropertyChanges {
            target: imgInBtn; visible: true
        }
    }


    FViewUseCaseGroup{
        FViewUseCase{condition: myButton.pressed === true
            PropertyChanges {
                target: img; bitmap: imagePressed
            }
            PropertyChanges {
                target: btnTxt; color: "black"
            }
            PropertyChanges {
                target: imgInBtn; bitmap: button.bitmap2
            }
        }
        FViewUseCase{
            condition: button.active === false
            PropertyChanges {
                target: img; visible: false
            }
            PropertyChanges {
                target: imgSelected; visible: true; bitmap: imageInActive
            }
            PropertyChanges {
                target: btnTxt; color: "black"
            }
            PropertyChanges {
                target: myButton; enabled: false
            }
            PropertyChanges {
                target: imgInBtn; bitmap: button.bitmap2
            }
        }
        FViewUseCase{
            condition: button.selected === true
            PropertyChanges {
                target: img; visible: false
            }
            PropertyChanges {
                target: imgSelected; visible: true
            }
            PropertyChanges {
                target: btnTxt; color: "black"
            }
            PropertyChanges {
                target: imgInBtn; bitmap: button.bitmap2
            }
        }
        FViewUseCase{condition: myButton.pressed === false
            PropertyChanges {
               target: img; bitmap: image
            }
            PropertyChanges {
                target: btnTxt; color: button.txtColor
            }
            PropertyChanges {
                target: imgInBtn; bitmap: button.bitmap
            }
        }
    }

}
