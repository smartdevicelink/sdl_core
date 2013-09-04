import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: fmLayout
    width: 800
    height: 480

    property alias sourceBtn: sourceBtn
    property alias browseBtn: browseBtn
    property alias albumInfoBtn: albumInfoBtn
    property alias albumInfoImg: stationLogoImg
    property alias stationTxt: stationTxt
    property alias trackTxt: trackTxt
    property alias artistTxt: artistTxt
    property alias alertBtn: alertBtn
    property alias preset1Btn: preset1Btn
    property alias preset2Btn: preset2Btn
    property alias preset3Btn: preset3Btn
    property alias preset4Btn: preset4Btn
    property alias preset5Btn: preset5Btn
    property alias preset6Btn: preset6Btn
    property alias replayBtn: replayBtn
    property alias tuneBtn: tuneBtn
    property alias presetsBtn: presetsBtn
    property bool artShown: true


    TouchButton{
        id: sourceBtn
        anchors.top: fmLayout.top
        anchors.topMargin: 140
        anchors.left: fmLayout.left
        anchors.leftMargin: 10
        height:25
        width: 130
        text: "Source"
    }

    TouchButton{
        id: browseBtn
        anchors.top: fmLayout.top
        anchors.topMargin: 180
        anchors.left: fmLayout.left
        anchors.leftMargin: 10
        height:25
        width: 130
        text: "Browse"
    }

    TouchButton{
        id: albumInfoBtn
        anchors.top: fmLayout.top
        anchors.topMargin: 95
        anchors.left: fmLayout.left
        anchors.leftMargin: 190
        height:130
        width: 130
        text: "Album"
        vis: false

    }

    FWdgtImage{
        id: stationLogoImg
        anchors.top: fmLayout.top
        anchors.topMargin: 95
        anchors.left: fmLayout.left
        anchors.leftMargin: 190
        height:70
        width: 130
        bitmap: ""
    }

    TouchButton{
        id: alertBtn
        anchors.top: albumInfoImg.bottom
        anchors.topMargin: 20
        anchors.left: albumInfoImg.left
        height:25
        width: 90
        text: "Alert"
    }

    BaseText{
        id: stationTxt
        anchors.verticalCenter: trackTxt.verticalCenter
        anchors.verticalCenterOffset: -45
        anchors.left: trackTxt.left
        text: "Ch 22 / Pearl Jam Radio"
    }

    BaseText{
        id: trackTxt
        anchors.verticalCenter: albumInfoBtn.verticalCenter
        anchors.left: albumInfoBtn.right
        anchors.leftMargin: 15
        pxSize: 34
        text: "State of Love ..."

    }

    BaseText{
        id: artistTxt
        anchors.verticalCenter: trackTxt.verticalCenter
        anchors.verticalCenterOffset: 45
        anchors.left: trackTxt.left
        text: "Pearl Jam"
    }

    TouchButton{
        id: preset1Btn
        anchors.top: fmLayout.top
        anchors.topMargin: 280
        anchors.left: browseBtn.left
        height:60
        width: 208
        text: "WWWWWWWW"
    }

    TouchButton{
        id: preset2Btn
        anchors.top: preset1Btn.bottom
        anchors.left: preset1Btn.left
        height:60
        width: 208
        text: "WWWWWWWW"
    }

    TouchButton{
        id: preset3Btn
        anchors.top: preset1Btn.top
        anchors.left: preset1Btn.right
        height:60
        width: 208
        text: "WWWWWWWW"
    }

    TouchButton{
        id: preset4Btn
        anchors.top: preset2Btn.top
        anchors.left: preset2Btn.right
        height:60
        width: 208
        text: "WWWWWWWW"
    }

    TouchButton{
        id: preset5Btn
        anchors.top: preset3Btn.top
        anchors.left: preset3Btn.right
        height:60
        width: 208
        text: "WWWWWWWW"
    }

    TouchButton{
        id: preset6Btn
        anchors.top: preset4Btn.top
        anchors.left: preset4Btn.right
        height:60
        width: 208
        text: "WWWWWWWW"
    }

    TouchButton{
        id: replayBtn
        anchors.verticalCenter: browseBtn.verticalCenter
        anchors.verticalCenterOffset: 35
        anchors.left: fmLayout.left
        anchors.leftMargin: 725
        height:40
        width: 55
        text: "rply"
    }

    TouchButton{
        id: tuneBtn
        anchors.top: preset1Btn.top
        anchors.right: replayBtn.right
        height:40
        width: 120
        text: "tune"
    }

    TouchButton{
        id: presetsBtn
        anchors.bottom: preset6Btn.bottom
        anchors.right: replayBtn.right
        height:60
        width: 120
        text: "presets"
    }

    FViewUseCaseGroup{
        FViewUseCase{
            condition: dataPool.hasArt === 0
            PropertyChanges { target: albumInfoBtn; vis: false;}
            PropertyChanges { target: albumInfoImg; visible: false;}
            PropertyChanges { target: trackTxt; anchors.left: albumInfoBtn.left;}
            PropertyChanges { target: alertBtn; vis: false;}
        }
        FViewUseCase{
            condition: dataPool.hasArt === 1
            PropertyChanges { target: albumInfoBtn; vis: false;}
            PropertyChanges { target: albumInfoImg; visible: false;}
            PropertyChanges { target: trackTxt; anchors.left: albumInfoBtn.left;}
            PropertyChanges { target: alertBtn; vis: false;}
        }
        FViewUseCase{
            condition: dataPool.hasArt === 2
            PropertyChanges { target: albumInfoImg; visible: true;}
            PropertyChanges { target: albumInfoBtn; vis: true;}
        }
    }
}
