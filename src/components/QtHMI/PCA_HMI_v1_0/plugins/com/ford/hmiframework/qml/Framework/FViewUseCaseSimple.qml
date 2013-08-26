// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

StateGroup {
    default property alias propertyChange: theState.changes
    property alias condition: theState.when

    states: [
        State {
            id: theState
            name: "*"
            when: true == true
        }
    ]
}
