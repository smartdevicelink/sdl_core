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
    property alias destSearch: destSearch.touchBtn
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
    TouchButtonSpecial_PCA{
        id: destSearch
        x:601
        y:419
        textWidth: 130
        bitmap: "Nav/destIcon.png"
        bitmap2: "Nav/destIconBlack.png"
        imageShift: -9
        spacing: 0
        text: " Destination"
        text1: ""
        text2: ""
        totHD: -1

    }
}
