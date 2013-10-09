import QtQuick 2.0
import "../hmi_api/Common.js" as Common
Item {
    property var source: undefined

    Image {
        anchors.fill: parent

        source: url(parent.source)

        function image(turnIcon) {
            return (turnIcon === undefined ||
                    turnIcon.imageType !== Common.ImageType.STATIC) ? "" :
                        turnIcon.value;
        }

        function url(turnIcon) {
            return (turnIcon === undefined ||
                    turnIcon.imageType !== Common.ImageType.DYNAMIC) ? "" :
                        turnIcon.value;
        }
    }
}
