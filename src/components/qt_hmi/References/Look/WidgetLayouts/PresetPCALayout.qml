// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {
    id: button
    property alias active: presetBtn.selected
    property alias text: presetBtn.text
    property alias touchBtn: presetBtn
    property alias presetText: presetTxt.text


    TouchButtonClimatePCA{
        id: presetBtn
        text: "X"
        image: "buttons/preset_9_9.png"
        imageActive: "buttons/presetSelected.png"
        imagePressed: "buttons/presetPressed_9_9.png"
    }

    BaseText{
        id: presetTxt
        anchors.horizontalCenter: presetBtn.horizontalCenter
        anchors.horizontalCenterOffset: -16
        y:59
        horAlignment: Text.AlignHCenter
        text: "104.3"
        color: "#1d81d5"
    }

    FViewUseCaseSimple{
        condition: presetBtn.active === true
        PropertyChanges {
            target: presetTxt; color: "white"
        }
    }

}
