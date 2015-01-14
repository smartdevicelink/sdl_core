// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: theItem
    width: 200
    height: 178

    property alias bitmap : _img.bitmap
    property alias touch: _touch


    FWdgtImage{
        id: _img
        anchors.centerIn: theItem
    }

    FWdgtTouchArea{
        id: _touch
        anchors.fill: theItem
    }

}
