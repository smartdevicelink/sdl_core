import QtQuick 2.0
import "."

Rectangle {

    id: root

    property variant dataText: "Default"
    property string textLabel: "Not initialised"

    width: Style.textBoxWidth
    height: Style.textBoxHeight
    radius: Style.radius
    anchors.left: parent.left
    anchors.leftMargin: Style.labelTextLeftMargin
    color: Style.colorLabelTextBackground

    Text {
        id: label

        text: root.textLabel
        width: Style.labelWidth
    }

    CastomInput {
        id: input

        anchors.left: label.right
        textInput: root.dataText
        widthInput: Style.widthInput
        onTextInputChanged: {
            root.dataText = textInput;
        }
        Connections {
            target: root
            onDataTextChanged: {
                input.textInput = root.dataText
            }
        }
    }
}
