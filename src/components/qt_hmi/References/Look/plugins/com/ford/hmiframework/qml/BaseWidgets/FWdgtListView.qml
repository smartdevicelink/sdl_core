// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

ListView {
    id: theList

    property alias data : theList.model
    property alias itemWidget : theList.delegate
}
