import QtQuick 2.0

Item
{
    default property alias content: containerRow.children
    property alias spacing: containerRow.spacing
    property int snapTo: 200

    height: containerRow.height

    Flickable {
        id: container
        maximumFlickVelocity: 1500
        contentWidth: containerRow.width
        height: containerRow.height
        width: parent.width

        onMovementEnded: {
            var rest = contentX % snapTo
            var t = 0.25
            if (rest > parent.snapTo / 2) {
                rest = rest - parent.snapTo
            }
            var vel = 2 * rest / t
            flickDeceleration = Math.abs(vel) / t
            flick(vel, 0)
            flickDeceleration = 1500
        }
        Row {
            id: containerRow
            height: childrenRect.height
        }
    }

    Pager {
        id: pager
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        pages: Math.ceil(container.contentWidth / container.width)
        activePage: Math.round(pages * container.contentX / container.contentWidth)
    }
}
