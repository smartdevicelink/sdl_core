// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

QQuickItem {
    property int xPixel
    property int yPixel
    property string initialView
    property string initalAnim
    property QUrl modelPath
    property string activeVariant
    property int priorityLayerCount
    property int minPriority
    readonly property QStringList priorityLayerDebug
    property int viewCacheSize

    signal preStartInit

    function animationCompleted (FAnimation ani) {
    }

    function updateDebugInformation {
    }
}
