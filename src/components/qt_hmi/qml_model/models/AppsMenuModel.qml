import QtQuick 2.0

ListModel
{
    ListElement {
        text: "Install"
        icon: "../res/install_apps.png"
    }
    ListElement {
        text: "Find"
        icon: "../res/find_apps.png"
    }
    ListElement {
        text: "Change Devices"
        icon: "../res/change_devices.png"
        qml: "./views/ChangeDeviceView.qml"
    }
    ListElement {
        text: "Vehicle Info"
        icon: "../res/app_vehicle_info.png"
    }
    ListElement {
        text: "911 Assist"
        icon: "../res/app_911_assist.png"
    }
}
