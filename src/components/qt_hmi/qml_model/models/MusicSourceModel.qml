import QtQuick 2.0

ListModel
{
    ListElement {
        name: "AM"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/AMPlayerView.qml"
    }
    ListElement {
        name: "Sirius"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/SiriusPlayerView.qml"
    }
    ListElement {
        name: "BT Audio"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/BTPlayerView.qml"
    }
    ListElement {
        name: "FM"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/FMPlayerView.qml"
    }
    ListElement {
        name: "CD"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/CDPlayerView.qml"
    }
    ListElement {
        name: "iPod"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/IPodPlayerView.qml"
    }
}
