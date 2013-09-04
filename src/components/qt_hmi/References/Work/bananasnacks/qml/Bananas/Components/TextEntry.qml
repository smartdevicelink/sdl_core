import QtQuick 2.0

Item {
    id: textentry
    property alias input_border: search_text_box.border
    property alias autocompleteModel: autocomplete.model
    property alias text: search_text_box.text
    signal selected
    anchors.fill: parent

    TextBox {
        id: search_text_box
        icon: "../Assets/SearchIcon.png"
        anchors.top: parent.top
        width: parent.width - 160
        anchors.horizontalCenter: parent.horizontalCenter

        onTextChanged: if (autocomplete.model) autocomplete.model.setSearchString(text)
    }

    BackButton {
        anchors.top: search_text_box.top
        anchors.topMargin: 1
        anchors.right: search_text_box.left
        anchors.rightMargin: 10
        onPress: section.state = 'menu'
    }

    ImageButton {
        anchors.top: search_text_box.top
        anchors.topMargin: 1
        icon: "../Assets/DeleteIcon.png"
        anchors.left: search_text_box.right
        anchors.leftMargin: 10
        height: 42

        onPress: search_text_box.text = search_text_box.text.replace(/.$/, '')
    }

    Keyboard {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 40
        anchors.top: search_text_box.bottom

        onLetterClicked: search_text_box.text += letter
        onGo: textentry.selected()
    }

    ListView {
        id: autocomplete
        anchors.top: search_text_box.bottom
        anchors.bottom: parent.bottom
        anchors.right: search_text_box.right
        anchors.left: search_text_box.left
        clip: true
        visible: (count > 0 && count <= 5)

        delegate: Button {
            text: model.text
            width: parent.width
            textAnchors.horizontalCenter: undefined
            textAnchors.left: clickable.left
            textAnchors.leftMargin: 70

            onPress: {
                autocomplete.model.selected = model.text;
                textentry.selected()
            }
        }
    }
}
