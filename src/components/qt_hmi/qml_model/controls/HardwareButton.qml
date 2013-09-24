import QtQuick 2.0
import com.ford.hmi_framework 1.0
import "../hmi_api/enums.js" as Enums

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
    property bool clickProcessed
    onPressed: {
        clickProcessed = false
        pressedImg.opacity = 1
        timer.start()
        if (upDownAvailable) {
            sdlButtons.onButtonEvent(buttonId, Enums.ButtonEventMode.BUTTONDOWN, undefined)
        }
    }

    onReleased: {
        if (upDownAvailable) {
            sdlButtons.onButtonEvent(buttonId, Enums.ButtonEventMode.BUTTONUP, undefined)
        }
        timer.stop()
        if (!clickProcessed && shortPressAvailable) {
            sdlButtons.onButtonPress(buttonId, Enums.ButtonPressMode.SHORT, undefined)
        }
        pressedImg.opacity = 0
    }

    Connections {
        target: timer
        onTriggered: {
            if(!clickProcessed && longPressAvailable) {
                sdlButtons.onButtonPress(buttonId, Enums.ButtonPressMode.LONG, undefined)
                clickProcessed = true
                hold()
            }
        }
    }

    Component.onCompleted: {
        if (buttonId !== Enums.ButtonName.CUSTOM_BUTTON) {
            settings.capabilities.push(
            {
                name: buttonId,
                upDownAvailable: upDownAvailable,
                shortPressAvailable: shortPressAvailable,
                longPressAvailable: longPressAvailable
            });
        }
    }
}
