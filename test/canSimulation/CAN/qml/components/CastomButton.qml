import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.2
import "."

Button {
    id: root
    text: "A button"
    style: ButtonStyle {
        background: Rectangle {
            border.color: Style.colorButtonBorder
            radius: Style.radius
            gradient: Gradient {
                GradientStop { position: 0 ; color: control.pressed ? Style.colorButtonGradTop : Style.colorButtonGradBot }
                GradientStop { position: 0.4 ; color: control.pressed ? Style.colorButtonGradTop : Style.colorButtonGradTop }
                GradientStop { position: 0.5 ; color: control.pressed ? Style.colorButtonGradTop : Style.colorButtonGradTop }
                GradientStop { position: 1 ; color: control.pressed ? Style.colorButtonGradBot : Style.colorButtonGradTop }
            }
        }
        label: Text {
            color: Style.colorButtonText
            text: root.text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
