import QtQuick 2.0
import com.ford.sdl.hmi.hw_buttons 1.0

MaskedContainer {
    property string name

    Image {
        id: icon
        source: "../res/controlButtons/" + name + "Button.png"
    }

    onPressed: {
        state = "pressed";
    }

    onReleased: {
        state = "";
    }

    states: [
        State {
            name: "pressed"
            PropertyChanges  {
                target: icon
                source: "../res/controlButtons/" + name + "Button_pressed.png"
            }
        }

    ]
}
