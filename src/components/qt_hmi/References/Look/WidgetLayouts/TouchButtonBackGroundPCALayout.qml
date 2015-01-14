// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {
    id: button
    property string bitmap: ""
    property bool selected: false
    property string text: ""
    property alias touchBtn: myButton
    property string image: "climate/Button_9_9.png"
    property string imageActive: "climate/ButtonActive_0_0.png"
    property string imagePressed: "climate/ButtonPressed_9_9.png"
    property string imageInActive: "climate/ButtonInActive_0_0.png"
    property bool active: true


    FWdgtImage{
        id: img
        x:-9
        y:-9
        bitmap: "Radio/PresetBtnNotSelected_9_9.png"
    }

    FWdgtImage{
        id: imgSelected
        x:-9
        y:-9
        bitmap: "Radio/PresetBtn_9_9.png"
        visible: false
    }

    FWdgtTouchArea {
        id: myButton
        anchors.fill: img
    }

    Rectangle {
        id: btnFill
        color: "#1d81d5"
        anchors.centerIn: myButton
        height: 46
        width: 46
        radius: 23
        visible: false
    }

    BaseText {
        id: btnTxt
        anchors.centerIn: myButton
        text: button.text
        color: "#1d81d5"
    }


    FViewUseCaseSimple{
        condition: button.selected === true
        PropertyChanges {
            target: img; visible: false
        }
        PropertyChanges {
            target: imgSelected; visible: true
        }
        PropertyChanges {
            target: btnTxt; color: "white"
        }
        PropertyChanges {
            target: btnFill; color: "white"
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
        FViewUseCase{condition: myButton.indicatorState === true
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
