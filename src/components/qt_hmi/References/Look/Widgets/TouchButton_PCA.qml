import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"

FWidget {

    id:pcaButton


    property alias text: button.text
    property alias textWidth: button.centerWidth
    property alias touchBtn: button.touchBtn
    property alias pressed: button.pressed
    width: textWidth+44
    clip: false



    layout: TouchButtonPCALayout{
        id:button
    }



}

