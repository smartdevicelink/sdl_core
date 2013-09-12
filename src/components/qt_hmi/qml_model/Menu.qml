import QtQuick 2.0

ListModel
{
    ListElement {
        name: "Climate"
        icon: "res/snow.png"
    }

    ListElement {
        name: "Navigation"
        icon: "res/arrow.png"
    }

    ListElement {
        name: "Media"
        icon: "res/notes.png"
        qml: "SiriusPlayerView.qml"
    }

    ListElement {
        name: "Apps"
        icon: "res/apps.png"
        qml: "AppsMenuGridView.qml"
    }

    ListElement {
        name: "Phone"
        icon: "res/phone.png"
    }

    ListElement {
        name: "Car"
        icon: "res/car.png"
        qml: "CarMenuGridView.qml"
    }

    ListElement {
        name: "Preferences"
        icon: "res/gear.png"
    }

}
