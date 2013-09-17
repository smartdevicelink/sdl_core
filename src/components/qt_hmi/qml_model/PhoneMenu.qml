import QtQuick 2.0

ListModel
{
    ListElement {
        name: "phone"
        icon: "res/phone/phone.png"
        qml: ""
    }
    ListElement {
        name: "contacts"
        icon: "res/phone/contacts.png"
        qml: "PhoneKeyboardGridView.qml"
    }
    ListElement {
        name: "messages"
        icon: "res/phone/messages.png"
        qml: ""
    }
    ListElement {
        name: "texts"
        icon: "res/phone/texts.png"
        qml: ""
    }
    ListElement {
        name: "DND"
        icon: "res/phone/dnd_off.png"
        qml: ""
    }
    ListElement {
        name: "recent"
        icon: "res/phone/recent.png"
        qml: ""
    }
    ListElement {
        name: "keyboard"
        icon: "res/phone/keypad.png"
        qml: ""
    }
    ListElement {
        name: "more"
        icon: "res/phone/more.png"
        qml: ""
    }
}
