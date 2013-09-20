import QtQuick 2.0
import com.ford.hmi_framework 1.0
import "../ford/enums.js" as Enums

MaskedContainer {

    property string name
    property int    buttonId: Enums.ButtonName.CUSTOM_BUTTON
    property bool   upDownAvailable: true
    property bool   shortPressAvailable: true
    property bool   longPressAvailable: true

    signal hold

    Image {
        source: "../res/controlButtons/" + name + "Button.png"
    }
    Image {
        id: pressedImg
        source: "../res/controlButtons/" + name + "Button_pressed.png"
        opacity: 0
        Behavior on opacity {
            NumberAnimation { duration: 80 }
        }
    }
    Timer {
        id: timer
        interval: 400
        repeat: false
        triggeredOnStart: false
    }
    onPressed: {
        pressedImg.opacity = 1
        timer.start()
    }

    onReleased: {
        timer.stop()
        pressedImg.opacity = 0
    }

    Connections {
        target: timer
        onTriggered: {
            hold()
        }
    }

    Component.onCompleted: {
        if (buttonId !== Enums.ButtonName.CUSTOM_BUTTON) {
            sdlButtons.capabilities.push(
                        {
                            name: buttonId,
                            upDownAvailable: upDownAvailable,
                            shortPressAvailable: shortPressAvailable,
                            longPressAvailable: longPressAvailable
                        });
        }
    }
}
