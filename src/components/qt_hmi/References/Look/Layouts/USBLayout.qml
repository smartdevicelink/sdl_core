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
    property alias stationTxt: stationTxt
    property alias trackTxt: trackTxt
    property alias artistTxt: artistTxt
    property alias seekBkBtn: seekBkBtn
    property alias playBtn: playBtn
    property alias seekFdBtn: seekFdBtn
    property alias repeatBtn: repeatBtn
    property alias shuffleBtn: shuffleBtn


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
        anchors.topMargin: 290
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
        id: seekBkBtn
        anchors.verticalCenter: browseBtn.verticalCenter
        anchors.horizontalCenter: playBtn.horizontalCenter
        anchors.horizontalCenterOffset: -150
        height:100
        width: 100
        text: "<<"
    }

    TouchButton{
        id: playBtn
        anchors.verticalCenter: browseBtn.verticalCenter
        anchors.horizontalCenter: fmLayout.horizontalCenter
        height:120
        width: 120
        text: "||"
    }

    TouchButton{
        id: seekFdBtn
        anchors.verticalCenter: browseBtn.verticalCenter
        anchors.horizontalCenter: playBtn.horizontalCenter
        anchors.horizontalCenterOffset: 150
        height:100
        width: 100
        text: ">>"
    }

    TouchButton{
        id: repeatBtn
        anchors.verticalCenter: browseBtn.verticalCenter
        anchors.verticalCenterOffset: -35
        anchors.left: fmLayout.left
        anchors.leftMargin: 670
        height:40
        width: 55
        text: "rpt"
    }

    TouchButton{
        id: shuffleBtn
        anchors.verticalCenter: browseBtn.verticalCenter
        anchors.verticalCenterOffset: 35
        anchors.left: repeatBtn.left
        height:40
        width: 55
        text: "sfl"
    }

}
