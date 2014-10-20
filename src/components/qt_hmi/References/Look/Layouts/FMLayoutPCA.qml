import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: fmLayout
    width: 800
    height: 480

    property alias sourceBtn: sourceBtn
    property alias tuneBtn: tuneBtn
    property alias hdBtn: hdBtn
    property alias stationTxt: stationTxt.text
    property alias freqId: freqId.text
    property alias listModel: thePresets.listModel
    property alias artist: artistTxt.text
    property alias curHD: hdBtn.curHD
    property alias totHD: hdBtn.totHD
    property alias curPreset: thePresets.currPreset
    property alias moving: thePresets.moving
    property alias direction: thePresets.direction
    property bool page1: true





    FWdgtImage{
        bitmap: "Radio/Ford-HMI-PCA-audio_fm_sirius.png"
        visible: false
    }

    TouchButton_PCA{
        id: sourceBtn
        x:25
        y:80
        textWidth: 100
        text: "FM Radio"

    }

    TouchButton_PCA{
        id: tuneBtn
        anchors.right: fmLayout.right
        anchors.rightMargin: 25
        y:80
        textWidth: 55
        text: "Tune"

    }

    BaseText{
        id: stationTxt
        x:25
        y:155
        pxSize: 45
        text: "96.3"
        color: "#1d81d5"
        theFont: "../Fonts/HelveticaNeueLTCom-Lt.ttf"

    }

    BaseText{
        id: freqId
        anchors.left: stationTxt.right
        anchors.leftMargin: 5
        anchors.bottom: stationTxt.bottom
        anchors.bottomMargin: 4
        text: "FM"
        color: "#1d81d5"
        pxSize: 22
        theFont: "../Fonts/HelveticaNeueLTCom-Lt.ttf"

    }

    BaseText{
        id: trackTxt
        x:25
        y:212
        text: "So Far Around The Bend"
        color: "#1d81d5"
        theFont: "../Fonts/HelveticaNeueLTStd-Md.ttf"
        pxSize: 22


    }

    BaseText{
        id: artistTxt
        x:25
        y:240
        text: "The National"
        color: "#1d81d5"
        theFont: "../Fonts/HelveticaNeueLTStd-Md.ttf"
        pxSize: 22
    }

    TouchButtonSpecial_PCA{
        id: hdBtn
        x:25
        y:288
        textWidth: 120
        bitmap: "Radio/HD Logo.png"
        bitmap2: "Radio/HD Logo1.png"
        imageShift: -9
        spacing: 0
        text: "  1 "
        text1: "2"
        text2: " 3 4"

    }

    Rectangle{
        x:25
        y: 365
        height:1
        width: 750
        color: "#1d81d5"
    }


    PresetsGridPCA{
        id: thePresets
        x:0
        y:365

    }

    FWdgtImage{
        id: swipeIcon1
        bitmap: "Radio/whiteBall.png"
        x: 389
        y: 381
    }

    FWdgtImage{
        id: swipeIcon2
        bitmap: "Radio/blueBall.png"
        x: 404
        y: 381
    }

    FViewUseCaseSimple{condition: fmLayout.page1===false
        PropertyChanges{
            target: swipeIcon2; bitmap: "Radio/whiteBall.png"
        }
        PropertyChanges{
            target: swipeIcon1; bitmap: "Radio/blueBall.png"
        }
    }

}
