import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"

FWidget {

    id:pcaButton

    property alias active: button.active
    property alias presetNumber: button.text
    property alias presetName: button.presetText
    property alias touchBtn: button.touchBtn


    clip: false



    layout: PresetPCALayout{
        id:button
    }



}

