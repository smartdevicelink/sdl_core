import QtQuick 2.0

ListModel
{
    ListElement {
        name: "AM"
        icon: "res/buttons/long_oval_btn.png"
        qml: "AMFMPlayerView.qml"
    }
    ListElement {
        name: "Sirius"
        icon: "res/buttons/long_oval_btn.png"
        qml: "SiriusPlayerView.qml"
    }
    ListElement {
        name: "BT Audio"
        icon: "res/buttons/long_oval_btn.png"
        qml: "BTCDIPODPlayerView.qml"
    }
    ListElement {
        name: "FM"
        icon: "res/buttons/long_oval_btn.png"
        qml: "AMFMPlayerView.qml"
    }
    ListElement {
        name: "CD"
        icon: "res/buttons/long_oval_btn.png"
        qml: "BTCDIPODPlayerView.qml"
    }
    ListElement {
        name: "iPod"
        icon: "res/buttons/long_oval_btn.png"
        qml: "BTCDIPODPlayerView.qml"
    }
}
