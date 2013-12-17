import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: fmLayout
    width: 800
    height: 480

    property alias sourceBtn: sourceBtn
    property alias browseBtn: browseBtn
    property alias trackNumTxt: trackNumTxt.text
    property alias artist: artistTxt.text
    property alias play: play.touchBtn
    property alias pause: pause.touchBtn
    property bool playing: true




    FWdgtImage{
        id: albumArt
        x:25
        y:176
        bitmap: "USB/albumArt.png"
    }

    TouchButton_PCA{
        id: sourceBtn
        x:25
        y:80
        textWidth: 100
        text: "FM Radio"

    }

    TouchButton_PCA{
        id: browseBtn
        anchors.right: fmLayout.right
        anchors.rightMargin: 25
        y:80
        textWidth: 75
        text: "Browse"

    }

    BaseText{
        id: trackNumTxt
        x:181
        y:188
        pxSize: 22
        text: "Track 13/ 16"
        color: "#1d81d5"
        theFont: "../Fonts/HelveticaNeueLTCom-Lt.ttf"

    }

    BaseText{
        id: trackTxt
        x:181
        y:225
        text: "The Dog Days Are Over"
        color: "#1d81d5"
        theFont: "../Fonts/HelveticaNeueLTStd-Md.ttf"
        pxSize: 45


    }

    BaseText{
        id: artistTxt
        x:181
        y:280
        text: "Florence and the Machine"
        color: "#1d81d5"
        theFont: "../Fonts/HelveticaNeueLTStd-Md.ttf"
        pxSize: 22
    }


    TouchButtonClimatePCA{
        id: play
        x:367
        y:390
        image: "USB/play_9_9.png"
        imagePressed: "USB/playPressed_9_9.png"
        visible: playing
        enabled: playing
    }

    TouchButtonClimatePCA{
        id: pause
        x:367
        y:390
        image: "USB/pause_9_9.png"
        imagePressed: "USB/pausePressed_9_9.png"
        visible: !playing
        enabled: !playing
    }

    TouchButtonClimatePCA{
        id: seekBack
        x:287
        y:399
        image: "USB/Prev_9_9.png"
        imagePressed: "USB/PrevPressed_9_9.png"
    }

    TouchButtonClimatePCA{
        id: seekNext
        x:463
        y:399
        image: "USB/Next_9_9.png"
        imagePressed: "USB/NextPressed_9_9.png"
    }

    Rectangle{
        x:92
        y: 365
        height:1
        width: 616
        color: "#1d81d5"
    }

    Rectangle{
        x:92
        y: 365
        height:1
        width: 138
        color: "white"
    }

    BaseText{
        id: elapsedTime
        x:25
        y:358
        text: "02:36"
        color: "white"
        theFont: "../Fonts/HelveticaNeueLTStd-Md.ttf"
        pxSize: 18
    }

    BaseText{
        id: totTime
        y:358
        anchors.right: fmLayout.right
        anchors.rightMargin: 25
        horAlignment: Text.AlignRight
        text: "04:23"
        color: "#1d81d5"
        theFont: "../Fonts/HelveticaNeueLTStd-Md.ttf"
        pxSize: 18
    }

}
