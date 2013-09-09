import QtQuick 2.0

Image {
    signal switched(string sw)

    function toggle() {
        if (state == "off") {
            state = "on";
            switched("on")
        } else {
            state = "off";
            switched("off")
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            parent.toggle();
        }
    }

    x: (parent.width - width) / 2
    y: 10
    source: state === "off" ? "res/buttons/key_9_9.png" : "res/buttons/keyPressed_9_9.png";
    Text {
        id: onOffCaption
        color: "white"
        text: parent.state === "off" ? "Off" : "On";
        anchors.centerIn: parent
    }

    states: [
        State { name: "on" },
        State { name: "off" }
    ]
    state: "off"
}
