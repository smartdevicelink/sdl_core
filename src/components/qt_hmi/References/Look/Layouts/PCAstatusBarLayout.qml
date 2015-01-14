import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: statusLayout
    width: 800
    height: 480

    property alias menuTouch: menuTouch

    Rectangle{
        id: ln1
        anchors.horizontalCenter: statusLayout.horizontalCenter
        y:15
        height:1
        width: 14
        color: "#1d81d5"
    }
    Rectangle{
        id: ln2
        anchors.horizontalCenter: statusLayout.horizontalCenter
        y:19
        height:1
        width: 14
        color: "#1d81d5"
    }
    Rectangle{
        id: ln3
        anchors.horizontalCenter: statusLayout.horizontalCenter
        y:23
        height:1
        width: 14
        color: "#1d81d5"
    }



    BaseText{
        id: menuTxt
        anchors.horizontalCenter: statusLayout.horizontalCenter
        anchors.bottom: statusLayout.top
        anchors.bottomMargin: -50
        horAlignment: Text.AlignHCenter
        vertAlignment: Text.AlignBottom
        text: "MENU"
        color: "#1d81d5"
        pxSize: 22
        theFont: "../Fonts/HelveticaNeueLTStd-Md.ttf"

    }

    BaseText{
        id: tempTxt
        x:25
        anchors.bottom: statusLayout.top
        anchors.bottomMargin: -50
        horAlignment: Text.AlignLeft
        vertAlignment: Text.AlignBottom
        text: "75" + "\xBA"
        color: "#1d81d5"
        theFont: "../Fonts/HelveticaNeueLTStd-Roman.ttf"
        pxSize: 28


    }

    BaseText{
        id: clockTxt
        anchors.right: statusLayout.right
        anchors.rightMargin: 25
        anchors.bottom: statusLayout.top
        anchors.bottomMargin: -50
        horAlignment: Text.AlignRight
        vertAlignment: Text.AlignBottom
        text: "12:45"
        color: "#1d81d5"
        theFont: "../Fonts/HelveticaNeueLTStd-Roman.ttf"
        pxSize: 28
    }

    FWdgtTouchArea{
        id: menuTouch
        height: 75
        width: 80
        y:0
        anchors.horizontalCenter: statusLayout.horizontalCenter
    }

    FViewUseCase{condition: menuTouch.pressed === true
        //FActScriptCall { onScript: { FLogger.debug("Test color should change") } }
        PropertyChanges {
            target: ln1; color: "white"
        }
        PropertyChanges {
            target: ln2; color: "white"
        }
        PropertyChanges {
            target: ln3; color: "white"
        }
        PropertyChanges {
            target: menuTxt; color: "white"
        }
    }

    FViewUseCase{
        condition: dataPool.mainPCAmenuShown ===true
        PropertyChanges {
            target: ln1; visible: false
        }
        PropertyChanges {
            target: ln2; visible: false
        }
        PropertyChanges {
            target: ln3; visible: false
        }
        PropertyChanges {
            target: menuTxt; visible: false
        }
    }


}
