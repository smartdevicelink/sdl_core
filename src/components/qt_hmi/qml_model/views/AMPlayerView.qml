import QtQuick 2.0

GeneralView {
    RadioPlayerView {
        anchors.fill: parent

        radioType: "AM"
        presets: [ "1130", "950", "760", "1270", "1400", "2100", "940", "780" ]
    }
}
