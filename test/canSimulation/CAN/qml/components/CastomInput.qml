import QtQuick 2.0
import "."

Rectangle {

    id: root
    property string textInput: "105.3"
    property int widthInput: 50

    border.color: Style.inputBorderColor
    width: root.widthInput
    height: parent.height
    radius: 3
    clip: true
    color: Style.colorCastomInputBackground

    onTextInputChanged: {
        tuneInput.text = textInput
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
            root.textInput = text;
        }
        onFocusChanged: {
            if (focus) {
                selectAll();
            }
        }

        Connections {
            target: root
            onTextInputChanged: {
                tuneInput.text = root.textInput
            }
        }
    }
}
