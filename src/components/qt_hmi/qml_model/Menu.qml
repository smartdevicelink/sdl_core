import QtQuick 2.0

ListModel
{
    ListElement {
        name: "Climate"
        icon: "res/snow.png"
        qml: "ClimateControlView.qml"
    }

    ListElement {
        name: "Navigation"
        icon: "res/arrow.png"
        qml: "NavigationMenuGridView.qml"
    }

    ListElement {
        name: "Media"
        icon: "res/notes.png"
        qml: "AMFMPlayerView.qml"
    }

    ListElement {
        name: "Apps"
        icon: "res/apps.png"
        qml: "AppsMenuGridView.qml"
    }

    ListElement {
        name: "Phone"
        icon: "res/phone/phone.png"
        qml: "PhoneMenuGridView.qml"
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
