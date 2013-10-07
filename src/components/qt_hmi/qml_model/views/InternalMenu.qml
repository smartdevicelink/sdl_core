import QtQuick 2.0
import "../controls"

GeneralView {
    id: menuView
    property ListModel listModel
    property int countOfUpperRowItems: Math.ceil(listModel.count / 2)

    // top 3/4 of screen with flick menu
    Flickable {
        id: flicker
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        contentWidth: flickItem.width

        Item {
            id: flickItem
            width: upperRow.width
            height: parent.height
            anchors.centerIn: parent

            Row {
                id: upperRow
                anchors.top: parent.top

                Repeater {
                    model: menuView.countOfUpperRowItems
                    delegate: Item {
                        id: item
                        width: flicker.width / 3
                        height: flicker.height / 2
                        opacity: 0

                        LongOvalButton {
                            id: button
                            text: menuView.listModel.get(index).title
                            pixelSize: 20
                            dest: menuView.listModel.get(index).qml
                            anchors.centerIn: parent
                        }

                        SequentialAnimation {
                            id: upRowAnimation
                            PauseAnimation {duration: index * 100 }
                            NumberAnimation {
                                target: item
                                duration:  500
                                property: "opacity"
                                from: 0; to: 1;
                            }
                        }
                        Component.onCompleted: {
                            upRowAnimation.start()
                        }
                    }
                }
            }

            Row {
                id: lowerRow
                anchors.bottom: parent.bottom

                Repeater {
                    model: menuView.countOfUpperRowItems
                    delegate: Item {
                        id: item2
                        width: flicker.width / 3
                        height: flicker.height / 2
                        opacity: 0

                        LongOvalButton {
                            id: buttonLowerRow
                            text: menuView.listModel.get(index + menuView.countOfUpperRowItems).title
                            pixelSize: 20
                            dest: menuView.listModel.get(index + menuView.countOfUpperRowItems).qml
                            anchors.centerIn: parent
                        }
                        SequentialAnimation {
                            id: lowRowAnimation
                            PauseAnimation {duration: 300 + index * 100 }
                            NumberAnimation {
                                target: item2
                                duration:  500
                                property: "opacity"
                                from: 0; to: 1;
                            }
                        }
                        Component.onCompleted: {
                            lowRowAnimation.start()
                        }
                    }
                }
            }
        }

        property int snapTo: width / 3
        onMovementEnded: {
            var rest = flicker.contentX % snapTo
            var time = 0.25
            if (rest > flicker.snapTo / 2) { rest = rest - flicker.snapTo }
            var vel = 2 * rest / time
            flickDeceleration = Math.abs(vel) / time
            flick(vel, 0)
            flickDeceleration = 1500
        }
}

    Pager {
        id: pager
        space: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top

        pages: Math.ceil(menuView.countOfUpperRowItems / 3) // 3 items in a row on 1 screen
        activePage: Math.round(pages * flicker.contentX / flicker.contentWidth)
    }

    Item {
        // 1/4 bottom screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        BackButton {
            id: backButton
            anchors.centerIn: parent
            opacity: 0

            SequentialAnimation {
                id: backButtonAnimation
                PauseAnimation {duration: 700}
                NumberAnimation {
                    target: backButton
                    duration:  500
                    property: "opacity"
                    from: 0; to: 1;
                }
            }

            Component.onCompleted: {
                backButtonAnimation.start()
            }
        }
    }


}
