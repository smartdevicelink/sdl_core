// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: theItem

    property alias presetName : _preset.presetName
    property alias presetNumber: _preset.presetNumber
    property alias pressed: _preset.active
    property alias touchBtn: _preset.touchBtn


    PresetPCA{
        id: _preset
        anchors.horizontalCenter: theItem.horizontalCenter
        anchors.horizontalCenterOffset: 0
        y:20
    }

}
