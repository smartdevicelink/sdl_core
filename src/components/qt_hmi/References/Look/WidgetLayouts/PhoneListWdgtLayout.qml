// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {

    id: theItem
    height: 50; width: 50

    property alias cx_text: cx.text
    property alias cn_text: cn.text
    property alias ct_text: ct.text
    property alias touch:_touch

    FWdgtTouchArea {
        id: touch
        anchors.fill: theItem
    }

    Item{


       BaseText{
           color:"#9deaec"
           id: cx
           width: 100; height: 50

       }
       BaseText{
           color:"#9deaec"
           id: cn
           width: 302; height: 50
           x:300
       }

       BaseText{
           color:"#9deaec"
           id: ct
           width: 302; height: 50
           x:500
       }

    }
    FWdgtTouchArea {
        id: _touch
        height: 50; width: 600
    }
}
