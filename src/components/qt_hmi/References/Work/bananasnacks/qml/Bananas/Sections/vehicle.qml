import QtQuick 2.0
import "../Components"
import "../Layouts"

Rectangle {
    anchors.fill: parent

    gradient: Gradient {
        GradientStop { position: 0.4; color: "#000000" }
        GradientStop { position: 1.0; color: "#104369" }
    }

    Section {
        id: section
        source: "Vehicle/menu.qml"
    }
}
