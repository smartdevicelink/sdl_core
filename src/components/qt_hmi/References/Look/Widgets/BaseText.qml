// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

FWdgtText {
    id: myText

    property bool enabled: true
    property bool secondary: false
    property double secondaryOpacity: 0.7
    property alias horAlignment: myText.horizontalAlignment
    property alias vertAlignment: myText.verticalAlignment
    property int pxSize: 24
    property bool boldTx: false
    property alias theFont: testFont.source

    color: "white"
    smooth: true

    property alias style: _styler.style

    FontLoader {
        id: testFont; source: "../Fonts/HelveticaNeueLTStd-Roman.ttf"
    }

    font.family: testFont.name
    font.pixelSize: pxSize
    font.bold: boldTx
    //horizontalAlignment: Text.AlignLeft
    //verticalAlignment: Text.AlignTop

    states: [
        State {
            name: "disabled"
            when: enabled === false
            PropertyChanges { target:myText; opacity: secondaryOpacity }
        },

        State {
            name: "secondary"
            when: secondary === true
            PropertyChanges { target:myText; opacity: secondaryOpacity }
        }
    ]

    FStyler {
        id: _styler
        styleTarget: myText
    }
}
