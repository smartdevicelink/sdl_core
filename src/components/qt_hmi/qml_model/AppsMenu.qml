import QtQuick 2.0

ListModel
{
    ListElement {
        text: "Install"
        icon: "res/install_apps.png"
    }
    ListElement {
        text: "Find"
        icon: "res/find_apps.png"
    }
    ListElement {
        text: "Change Devices"
        icon: "res/change_devices.png"
        qml: "ChangeDeviceView.qml"
    }
}
