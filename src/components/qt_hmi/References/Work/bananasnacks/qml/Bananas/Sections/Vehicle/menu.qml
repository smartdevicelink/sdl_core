import QtQuick 2.0
import "../../Components"
import "../../Layouts"

LeftThirdSection {
    MultiPointTouchArea {
        anchors.fill: parent
    }

    Grid {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        GraphicalButton {
            text: "HEV"
            image: "../../Assets/EvIcon.png"
        }

        GraphicalButton {
            text: "Ambient Lighting"
            image: "../../Assets/LightingIcon.png"
        }

        GraphicalButton {
            text: "Wallpaper"
            image: "../../Assets/WallpaperIcon.png"
        }

        GraphicalButton {
            text: "Wi-Fi"
            image: "../../Assets/WifiIcon.png"
        }

        GraphicalButton {
            text: "Health Report"
            image: "../../Assets/VhrIcon.png"
        }

        GraphicalButton {
            text: "Seats"
            image: "../../Assets/SeatsIcon.png"
        }

        GraphicalButton {
            text: "Keypad Entry"
            image: "../../Assets/KeypadEntryIcon.png"
        }

        GraphicalButton {
            text: "Sync Services"
            image: "../../Assets/ServicesIcon.png"
        }

        GraphicalButton {
            text: "Add Shortcut"
            image: "../../Assets/AddIcon.png"
        }
    }
}
