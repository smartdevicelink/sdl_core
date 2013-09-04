import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FWdgtTouchArea {
    id: button

    property string text: "Text"
    property string bitmap: ""
    property int wrapText: Text.NoWrap
    property bool indicatorState: false
    property string unpressedColor: "gray"
    property string pressEventColor: "lightGray"
    property string pressedColor: "darkGray"
    property bool vis: true


    width: 120
    height: 40
    enabled: button.vis



    Rectangle {
        id: buttonArea
        radius: 10
        anchors.centerIn: parent
        width: button.width
        height: button.height
        color: "gray"
        visible: button.vis

        BaseText {
            id: st
            anchors.centerIn: parent
            text: button.text
            wrapMode: wrapText
            visible: button.vis
        }

        FWdgtImage{
            id: img
            anchors.fill: buttonArea
            bitmap: button.bitmap
            visible: false
        }


    }

    FViewUseCaseSimple{
        condition: button.bitmap !== ""
        PropertyChanges {
            target: img; visible: true
        }
    }

    FViewUseCaseGroup{
        FViewUseCase{condition: pressed === true
            PropertyChanges {
                target: buttonArea; color: pressEventColor
            }
        }
        FViewUseCase{condition: indicatorState === true
            PropertyChanges {
                target: buttonArea; color: pressedColor
            }
        }
        FViewUseCase{condition: pressed === false
            PropertyChanges {
                target: buttonArea; color: unpressedColor
            }
        }
    }

}

