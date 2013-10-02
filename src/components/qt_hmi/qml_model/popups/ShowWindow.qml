import QtQuick 2.0
import QtQuick.Window 2.0

Window {
    width: 400
    height: 256

    title: "Show"
    flags: Qt.Dialog

    Rectangle {
        id: rectangle
        anchors.fill: parent

        property alias mainField1Text: mainField1.text
        property alias mainField2Text: mainField2.text
        property alias mainField3Text: mainField3.text
        property alias mainField4Text: mainField4.text

        Column {
            Text {
                id: mainField1
            }
            Text {
                id: mainField2
            }
            Text {
                id: mainField3
            }
            Text {
                id: mainField4
            }
        }
    }

    function showMessage (fieldSubstringsStruct) {
        rectangle.mainField1Text = "<h1>" + fieldSubstringsStruct.mainField1 + "</h1>"
        rectangle.mainField2Text = "<h2>" + fieldSubstringsStruct.mainField2 + "</h2>"
        rectangle.mainField3Text = "<h3>" + fieldSubstringsStruct.mainField3 + "</h3>"
        rectangle.mainField4Text = fieldSubstringsStruct.mainField4
    }
}
