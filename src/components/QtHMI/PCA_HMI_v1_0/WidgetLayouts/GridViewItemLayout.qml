// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: theItem

    property alias cx_text : _btn.text
    property int indentLevel: 0
    property alias touch: _btn.touchBtn
    property alias pressed: _btn.pressed


    TouchButton_PCA{
        id: _btn
        anchors.left: theItem.left
        anchors.leftMargin: 10
        anchors.top: theItem.top
        anchors.topMargin: 10
        textWidth: 100
    }

}
