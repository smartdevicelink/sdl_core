import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"

FWidget {

    id:pcaButton

    property alias bitmap:button.bitmap
    property alias bitmap2: button.bitmap2
    property alias text: button.text
    property alias text1: button.text1
    property alias text2:button.text2
    property alias textWidth: button.centerWidth
    property alias touchBtn: button.touchBtn
    property alias imageShift: button.imageShift
    property alias spacing: button.spacing
    property alias pressed: button.pressed
    property alias curHD: button.curHD
    property alias totHD: button.totHD
    width: 164
    clip: false



    layout: TouchButtonPCASpecialLayout{
        id:button
    }



}

