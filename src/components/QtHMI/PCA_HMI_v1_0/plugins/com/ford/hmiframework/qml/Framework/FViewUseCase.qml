// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

State {
    default property alias propertyChange: myState.changes
    property alias condition: myState.when
    id: myState
    when: true === true
}
