import QtQuick 2.0
import "."

Rectangle {

    id: root
    property string textInput: "105.3"

    border.color: Style.inputBorderColor
    width: Style.customInputWidthInput
    height: parent.height
    radius: Style.radius
    clip: true
    color: Style.colorCastomInputBackground

    onTextInputChanged: {
        tuneInput.text = textInput //Bidirectional binding
    }

    TextInput {
        id: tuneInput
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font.weight: Font.Bold
        text: root.textInput
        width: parent.width
        color: Style.colorCastomInputText
        onTextChanged: {
            root.textInput = text //Bidirectional binding
        }
        onFocusChanged: {
            if (focus) {
                selectAll();
            }
        }

        /**
         * Connections for bidirectional binding
         */
        Connections {
            target: root
            onTextInputChanged: {
                tuneInput.text = root.textInput
            }
        }
    }
}
