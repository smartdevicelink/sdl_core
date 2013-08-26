import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: alphaLayout
    width: 800
    height: 480
    property alias list : theListView
    property alias list_itemWidget: theListView.itemWidget
    property alias list_data: theListView.data

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
    property alias sym: symBtn.touchBtn
    property alias del: delBtn.touchBtn
    property alias clr: clrBtn.touchBtn
    property alias space: spaceBtn.touchBtn
    property alias close: closeTouch
    property string entry: ""
    property alias go: goBtn.touchBtn
    property alias listOpen: listOpen

    FWdgtListView {
        id: theListView
        anchors.fill: parent
        clip: true
        visible:false
    }

    FWdgtListView {
        x:400
        id: filteredListView
        clip: true
        data:filtermodel
        itemWidget:list_itemWidget
        width:100
        height:400
        visible: true
    }
    TextInput{ //Little Trick for Update on List
        text:dataPool.navEnrty
        x:200
        y:20
        onTextChanged: {

            if(text.length > 0 ) {
                            dataPool.filter = text
                            console.log(dataPool.filter)
                            sortList.fillFilterModel(theListView.data)//Just backup original
                            sortList.updateFilterModel(text)

                        } else {
                            sortList.fillFilterModel(theListView.data)
                            dataPool.navList1stItem=""

                        }


        }

    }
    BaseText{
        id: navEntry
        x:25
        y:65
        text: entry
        theFont:  "../Fonts/HelveticaNeueLTStd-Roman.ttf"
        pxSize: 45
        color: "#1d81d5"
        horAlignment: Text.AlignLeft
        vertAlignment: Text.AlignTop
    }
    BaseText{
        id: listFiltEntry
        x:25
        y:130
        color:"#1d81d5"
        text:dataPool.navList1stItem
        theFont:  "../Fonts/HelveticaNeueLTStd-Roman.ttf"
        pxSize: 45
        horAlignment: Text.AlignLeft
        vertAlignment: Text.AlignTop
    }
    Rectangle{
        id:rect
        x:20
        y:120
        height: 60
        radius:10
        width:650
        color:"transparent"
        border.color: "#1d81d5"
        border.width: 1
        visible: false
    }

    FWdgtTouchArea{
        id: listOpen
        anchors.fill: rect
        enabled: rect.visible
    }

    TouchButton_PCA{
        id: goBtn
        x:690
        y:115
        textWidth: 40
        text: "Go"

    }


    TouchButtonClimatePCA{
        id: qBtn
        x:61
        y:211
        text: "Q"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: wBtn
        x:130
        y:211
        text: "W"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: eBtn
        x:199
        y:211
        text: "E"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: rBtn
        x:268
        y:211
        text: "R"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: tBtn
        x:337
        y:211
        text: "T"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: yBtn
        x:406
        y:211
        text: "Y"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: uBtn
        x:475
        y:211
        text: "U"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: iBtn
        x:544
        y:211
        text: "I"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: oBtn
        x:613
        y:211
        text: "O"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: pBtn
        x:682
        y:211
        text: "P"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: numBtn
        x:27
        y:272
        text: "123"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 22
    }

    TouchButtonClimatePCA{
        id: aBtn
        x:96
        y:272
        text: "A"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: sBtn
        x:165
        y:272
        text: "S"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: dBtn
        x:234
        y:272
        text: "D"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: fBtn
        x:303
        y:272
        text: "F"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: gBtn
        x:372
        y:272
        text: "G"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: hBtn
        x:441
        y:272
        text: "H"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: jBtn
        x:510
        y:272
        text: "J"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: kBtn
        x:579
        y:272
        text: "K"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: lBtn
        x:648
        y:272
        text: "L"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: delBtn
        x:717
        y:272
        bitmap: "Nav/deleteIcon.png"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
    }

    TouchButtonClimatePCA{
        id: symBtn
        x:61
        y:333
        text: "!@#"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 22
    }

    TouchButtonClimatePCA{
        id: zBtn
        x:130
        y:333
        text: "Z"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: xBtn
        x:199
        y:333
        text: "X"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: cBtn
        x:268
        y:333
        text: "C"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: vBtn
        x:337
        y:333
        text: "V"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: bBtn
        x:406
        y:333
        text: "B"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: nBtn
        x:475
        y:333
        text: "N"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: mBtn
        x:544
        y:333
        text: "M"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        txtSize: 30
    }

    TouchButtonClimatePCA{
        id: clrBtn
        x:613
        y:333
        text: "Clear"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        newFont: "../Fonts/HelveticaNeueLTStd-Md.ttf"
        txtSize: 16
    }

    TouchButtonClimatePCA{
        id: spaceBtn
        x:682
        y:333
        text: "Space"
        image: "buttons/key_9_9.png"
        imagePressed: "buttons/keyPressed_9_9.png"
        imageActive: "buttons/keySelected.png"
        imageInActive: "buttons/keySelected.png"
        newFont: "../Fonts/HelveticaNeueLTStd-Md.ttf"
        txtSize: 16
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

    FViewUseCaseSimple{condition: listFiltEntry.text !== ""
        PropertyChanges {
            target: rect; visible: true
        }
    }


}
