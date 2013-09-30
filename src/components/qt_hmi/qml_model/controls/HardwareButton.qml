import QtQuick 2.0
import com.ford.sdl.hmi.hw_buttons 1.0
import "../hmi_api/Common.js" as Common

MaskedContainer {

    property string name
    property int    buttonId: Common.ButtonName.CUSTOM_BUTTON
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
            sdlButtons.onButtonEvent(buttonId, Common.ButtonEventMode.BUTTONDOWN, undefined)
        }
    }

    onReleased: {
        if (upDownAvailable) {
            sdlButtons.onButtonEvent(buttonId, Common.ButtonEventMode.BUTTONUP, undefined)
        }
        timer.stop()
        if (!clickProcessed && shortPressAvailable) {
            sdlButtons.onButtonPress(buttonId, Common.ButtonPressMode.SHORT, undefined)
        }
        pressedImg.opacity = 0
    }

    Connections {
        target: timer
        onTriggered: {
            if(!clickProcessed && longPressAvailable) {
                sdlButtons.onButtonPress(buttonId, Common.ButtonPressMode.LONG, undefined)
                clickProcessed = true
                hold()
            }
        }
    }

    Component.onCompleted: {
        if (buttonId !== Common.ButtonName.CUSTOM_BUTTON) {
            settings.buttonCapabilities.push(
            {
                name: buttonId,
                upDownAvailable: upDownAvailable,
                shortPressAvailable: shortPressAvailable,
                longPressAvailable: longPressAvailable
            });
        }
    }
}
