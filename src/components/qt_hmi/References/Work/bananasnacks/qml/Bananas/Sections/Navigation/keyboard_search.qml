import QtQuick 2.0
import "../../Components"
import "../../Layouts"

Item {
    anchors.fill: parent
    id: keyboard_search

    states: [
        State {
            name: 'city'
            PropertyChanges {target: text_entry; autocompleteModel: citySuggestions}
        },
        State {
            name: 'street'
            PropertyChanges {target: text_entry; autocompleteModel: streetSuggestions}
        },
        State {
            name: 'door'
            PropertyChanges {target: text_entry; autocompleteModel: undefined}
        },
        State {
            name: 'review'
            PropertyChanges {target: text_entry; visible: false}
            PropertyChanges {target: review; visible: true}
        }
    ]

    Column {
        id: review
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: 500

        visible: false
        Button {
            text: stateSuggestions.selected
            width: parent.width
            onPress: keyboard_search.state = ""
        }
        Button {
            text: citySuggestions.selected
            width: parent.width
             onPress: keyboard_search.state = "city"
        }
        Button {
            text: streetSuggestions.selected
            width: parent.width
            onPress: keyboard_search.state = "city"
        }
        Button {
            id: doornumber
            width: parent.width
            onPress: keyboard_search.state = "door"
        }
        Button {
            text: "Begin navigation"
            anchors.horizontalCenter: parent.horizontalCenter
            onPress: {
                section.state = 'map';
            }
        }
    }
}
