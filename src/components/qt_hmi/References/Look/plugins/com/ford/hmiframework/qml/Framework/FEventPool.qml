// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

QtObject {
    id: theEventPool
    default property alias events: theEventPool._events
    property list<FEvent> _events
}
