import QtQuick 2.0

ListModel
{
    ListElement {
        name: "Car"
        icon: "res/car.png"
        qml: "CarMenuGridView.qml"
    }

    ListElement {
        name: "Navigation"
        icon: "res/arrow.png"
    }

    ListElement {
        name: "Preferences"
        icon: "res/gear.png"
    }

    ListElement {
        name: "Media"
        icon: "res/notes.png"
    }

    ListElement {
        name: "Phone"
        icon: "res/phone.png"
    }

    ListElement {
        name: "Climate"
        icon: "res/snow.png"
    }
}
