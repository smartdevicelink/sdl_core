import QtQuick 2.0

ListModel
{
    ListElement {
        title: "Home"
        icon: "res/buttons/long_oval_btn.png"
    }
    ListElement {
        title: "Keyboard"
        icon: "res/buttons/long_oval_btn.png"
        qml: "NavigationKeyboard.qml"
    }
    ListElement {
        title: "Favorites"
        icon: "res/buttons/long_oval_btn.png"
        qml: "ChangeDeviceView.qml"
    }
    ListElement {
        title: "Work"
        icon: "res/buttons/long_oval_btn.png"
    }
    ListElement {
        title: "Recent"
        icon: "res/buttons/long_oval_btn.png"
    }
    ListElement {
        title: "POI"
        icon: "res/buttons/long_oval_btn.png"
    }
}
