import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"

FWidget {

    id:pcaButton

    property alias bitmap: button.bitmap
    property alias bitmap2: button.bitmap2
    property alias selected: button.selected
    property alias touchBtn: button.touchBtn
    property alias text: button.text
    property alias image: button.image
    property alias imageActive: button.imageActive
    property alias imagePressed: button.imagePressed
    property alias imageInActive: button.imageInActive
    property alias active: button.active
    property alias newFont: button.newFont
    property alias txtSize: button.txtSize
    property alias txtColor: button.txtColor


    height: 80
    width: 80
    clip: false



    layout: TouchButtonClimatePCALayout{
        id:button
    }



}

