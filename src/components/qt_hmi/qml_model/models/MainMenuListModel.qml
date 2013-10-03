import QtQuick 2.0

ListModel
{
    ListElement {
        name: "Climate"
        icon: "../res/snow.png"
        qml: "./views/ClimateControlView.qml"
    }

    ListElement {
        name: "Navigation"
        icon: "../res/arrow.png"
        qml: "./views/NavigationNoRouteGridView.qml"
    }

    ListElement {
        name: "Media"
        icon: "../res/notes.png"
        qml: "./views/AMPlayerView.qml"
    }

    ListElement {
        name: "Apps"
        icon: "../res/apps.png"
        qml: "./views/ApplicationListView.qml"
    }

    ListElement {
        name: "Phone"
        icon: "../res/phone/phone.png"
        qml: "./views/PhoneMenuGridView.qml"
    }

    ListElement {
        name: "Car"
        icon: "../res/car.png"
        qml: "./views/CarMenuGridView.qml"
    }

    ListElement {
        name: "Preferences"
        icon: "../res/gear.png"
        qml: ""
    }

}
