import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"

FWidget {

    id:pcaButton

    property alias bitmap:button.bitmap
    property alias active: button.selected
    property alias text: button.text
    property alias touchBtn: button.touchBtn


    clip: false



    layout: TouchButtonBackGroundPCALayout{
        id:button
    }



}

