// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

PathView {
    id: thePathView

    property alias data : thePathView.model
    property alias itemWidget : thePathView.delegate
}
