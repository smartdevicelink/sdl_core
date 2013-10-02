import QtQuick 2.0
import "../controls"

GeneralView {
    RadioPlayerView {
        id: siriusRadioPlayer
        anchors.fill: parent
        radioType: ""
        radioName: "Sirius"
        songName: "So Far Around The Bend"
        albumName: "The National"
        presets: [ "Lithium", "Spectrum", "ESPN", "Alt Nation", "Lithium", "Spectrum", "ESPN", "Alt Nation" ]
    }
}
