import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: alphaLayout
    width: 800
    height: 480


    property alias a: aBtn.touchBtn
    property alias b: bBtn.touchBtn
    property alias c: cBtn.touchBtn
    property alias d: dBtn.touchBtn
    property alias e: eBtn.touchBtn
    property alias f: fBtn.touchBtn
    property alias g: gBtn.touchBtn
    property alias h: hBtn.touchBtn
    property alias i: iBtn.touchBtn
    property alias j: jBtn.touchBtn
    property alias k: kBtn.touchBtn
    property alias l: lBtn.touchBtn
    property alias m: mBtn.touchBtn
    property alias n: nBtn.touchBtn
    property alias o: oBtn.touchBtn
    property alias p: pBtn.touchBtn
    property alias q: qBtn.touchBtn
    property alias r: rBtn.touchBtn
    property alias s: sBtn.touchBtn
    property alias t: tBtn.touchBtn
    property alias u: uBtn.touchBtn
    property alias v: vBtn.touchBtn
    property alias w: wBtn.touchBtn
    property alias xB: xBtn.touchBtn
    property alias yB: yBtn.touchBtn
    property alias zB: zBtn.touchBtn
    property alias num: numBtn.touchBtn
    property alias close: closeTouch

    property string phonEntry: ""

//    BaseText{
//        id: phoneEntry
//        x:25
//        y:115
//        text: phonEntry
//        theFont:  "../Fonts/HelveticaNeueLTStd-Roman.ttf"
//        pxSize: 45
//        horAlignment: Text.AlignLeft
//        vertAlignment: Text.AlignTop
//    }

    TouchButtonClimatePCA{
        id: aBtn
        x:48
        y:131
        text: "A"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: jBtn
        x:48
        y:212
        text: "J"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: sBtn
        x:48
        y:293
        text: "S"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: bBtn
        x:129
        y:131
        text: "B"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: kBtn
        x:129
        y:212
        text: "K"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: tBtn
        x:129
        y:293
        text: "T"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: cBtn
        x:210
        y:131
        text: "C"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: lBtn
        x:210
        y:212
        text: "L"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: uBtn
        x:210
        y:293
        text: "U"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: dBtn
        x:291
        y:131
        text: "D"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: mBtn
        x:291
        y:212
        text: "M"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: vBtn
        x:291
        y:293
        text: "V"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: eBtn
        x:372
        y:131
        text: "E"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: nBtn
        x:372
        y:212
        text: "N"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: wBtn
        x:372
        y:293
        text: "W"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: fBtn
        x:453
        y:131
        text: "F"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: oBtn
        x:453
        y:212
        text: "O"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: xBtn
        x:453
        y:293
        text: "X"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: gBtn
        x:534
        y:131
        text: "G"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: pBtn
        x:534
        y:212
        text: "P"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: yBtn
        x:534
        y:293
        text: "Y"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: hBtn
        x:615
        y:131
        text: "H"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: qBtn
        x:615
        y:212
        text: "Q"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: zBtn
        x:615
        y:293
        text: "Z"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: iBtn
        x:696
        y:131
        text: "I"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: rBtn
        x:696
        y:212
        text: "R"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: numBtn
        x:696
        y:293
        text: "123"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    BaseText{
        id: closeTxt
        anchors.horizontalCenter: alphaLayout.horizontalCenter
        y:430
        horAlignment: Text.AlignHCenter
        vertAlignment: Text.AlignTop
        text: "CLOSE"
        color: "#1d81d5"
        pxSize: 22
        theFont: "../Fonts/HelveticaNeueLTStd-Md.ttf"

    }

    FWdgtTouchArea{
        id: closeTouch
        height: 75
        width: 750
        y:430
        anchors.horizontalCenter: alphaLayout.horizontalCenter
    }

    FViewUseCase{condition: closeTouch.pressed === true
        //FActScriptCall { onScript: { FLogger.debug("Test color should change") } }
        PropertyChanges {
            target: closeTxt; color: "white"
        }
    }

}
