import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: alphaLayout
    width: 800
    height: 480


    Item {
        id: _fg

        width: 800; height: 480
        x:0
        y:0

        FWdgtTouchArea{
            id: blockTouch
            x:0
            y:0
            height: 480
            width: 800
        }

        Rectangle{
            anchors.fill: blockTouch
            color: "black"
            opacity: 0.8
        }

        FWdgtImage{
            id: popupBg
            anchors.centerIn: blockTouch
            bitmap: "climate/ring.png"
        }

        BaseText{
            id:curTemp
            anchors.centerIn: popupBg
            text: dataPool.drvTemp + "\xBA"
            pxSize: 96
        }

        BaseText{
            id:drvPass
            anchors.horizontalCenter: popupBg.horizontalCenter
            anchors.bottom: curTemp.top
            horAlignment: Text.AlignHCenter
            text: "Driver"
            pxSize: 28
        }

    }


}
