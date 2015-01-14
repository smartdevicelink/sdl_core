import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: mapLayout
    width: 800
    height: 480

    property alias zoomIn: zoomIn.touchBtn
    property alias zoomOut: zoomOut.touchBtn
    property alias compass: compass.touchBtn
    property alias cancel: cancel.touchBtn
    property alias mute: mute.touchBtn
    property alias muteSt: mute.selected
    property alias options: options.touchBtn



    FWdgtImage{
        id: mapImg
        bitmap: "Nav/Map.png"
        anchors.fill: mapLayout
    }

    TouchButtonClimatePCA{
        id: zoomIn
        text: "+"
        x:25
        y:103
        image: "buttons/zoom_9_9.png"
        imageActive: "buttons/zoomSelected.png"
        imagePressed: "buttons/zoomPressed_9_9.png"
        txtColor: "white"
        txtSize: 40

    }
    TouchButtonClimatePCA{
        id: zoomOut
        text: "-"
        x:25
        y:223
        image: "buttons/zoom_9_9.png"
        imageActive: "buttons/zoomSelected.png"
        imagePressed: "buttons/zoomPressed_9_9.png"
        txtColor: "white"
        txtSize: 40
    }
    TouchButtonClimatePCA{
        id: options
        x:659
        y:80
        image: "Nav/Options_9_9.png"
        imageActive: "Nav/OptionsInActive.png"
        imagePressed: "Nav/OptionsPressed_9_9.png"
    }
    TouchButtonClimatePCA{
        id: compass
        x:34
        y:352
        image: "Nav/compass.png"
        imageActive: "Nav/compass.png"
        imagePressed: "Nav/compass.png"
    }
    TouchButtonClimatePCA{
        id: mute
        x:25
        y:419
        image: "buttons/mute_9_9.png"
        imageActive: "buttons/muteActive.png"
        imagePressed: "buttons/mutePressed_9_9.png"
        imageInActive: "buttons/muteInActive.png"
    }
    FWdgtImage{
        bitmap: "Nav/turnArrow.png"
        x:97
        y:428
    }
    BaseText{
        text: "0.2 mi on Cherry Hill Rd."
        pxSize: 22
        x:144
        y:434
        color: "white"
    }

    BaseText{
        text: "2 hrs 27 min"
        pxSize: 22
        anchors.right: cancel.left
        anchors.rightMargin: 26
        horAlignment: Text.AlignRight
        y:434
        color: "white"
    }

    TouchButtonSpecial_PCA{
        id: cancel
        x:621
        y:419
        textWidth: 110
        bitmap: "Nav/destIcon.png"
        bitmap2: "Nav/destIconBlack.png"
        imageShift: -9
        spacing: 0
        text: " Cancel"
        text1: ""
        text2: ""
        totHD: -1

    }
    FWdgtImage{
        id: currStBkgrnd
        bitmap: "Nav/current street.png"
        x:265
        y:349
    }

    BaseText{
        id: currentStreet
        x:312
        y:360
        text: "East Capital St. NE"
        pxSize: 18
        theFont: "../Fonts/HelveticaNeueLTCom-Lt.ttf"
        horAlignment: Text.AlignLeft
        vertAlignment: Text.AlignTop
    }

}
