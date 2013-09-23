import QtQuick 2.0

Item {
    property string route_text: ""

    property var playerPreferences: {
        "AM": { },
        "FM": { },
        "Sirius": { preset: 0 },
        "BT": { },
        "CD": { },
        "iPod": { }
    }

    function reset () {
        route_text = ""
    }
}
