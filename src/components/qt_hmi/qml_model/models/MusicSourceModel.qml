import QtQuick 2.0

ListModel
{
    ListElement {
        title: "AM"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/AMPlayerView.qml"
    }
    ListElement {
        title: "Sirius"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/SiriusPlayerView.qml"
    }
    ListElement {
        title: "BT Audio"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/BTPlayerView.qml"
    }
    ListElement {
        title: "FM"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/FMPlayerView.qml"
    }
    ListElement {
        title: "CD"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/CDPlayerView.qml"
    }
    ListElement {
        title: "iPod"
        icon: "../res/buttons/long_oval_btn.png"
        qml: "./views/IPodPlayerView.qml"
    }
}
