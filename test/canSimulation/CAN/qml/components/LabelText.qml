import QtQuick 2.0
import "."

Rectangle {

    id: root

    property string dataText: "Default"
    property string textLabel: "Not initialised"

    width: Style.textBoxWidth
    height: Style.textBoxHeight
    radius: Style.radius
    anchors.left: parent.left
    anchors.leftMargin: Style.labelTextLeftMargin

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
    }
}
