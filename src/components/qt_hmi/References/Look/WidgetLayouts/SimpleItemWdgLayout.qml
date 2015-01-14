// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {

    id: theItem
    height: 50; width: 50
    property bool vis: true

    property alias cx_text : cx.text

    Item{

       FWdgtText{
           color:"White"
           id: cx
           width: 302; height: 45
           visible: vis
       }
    }
}
