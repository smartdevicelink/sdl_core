import QtQuick 2.0
import "../controls"

GeneralView{
    id: menuView
    property ListModel listModel
    property int countOfUpperRowItems: Math.ceil(listModel.count / 2)

    Flickable {
        id: flicker
        anchors.fill: parent
        contentWidth: upperRow.width

        flickableDirection: Flickable.HorizontalFlick

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

                        Image {
                            source: menuView.listModel.get(index).icon
                            anchors.centerIn: parent
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(menuView.listModel.get(index).qml !== "") {
                                    contentLoader.go(menuView.listModel.get(index).qml)
                                }
                            }
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
                    model: menuView.listModel.count - menuView.countOfUpperRowItems
                    delegate: Item {
                        id: item2
                        width: flicker.width / 3
                        height: flicker.height / 2
                        opacity: 0

                        Image {
                            source: menuView.listModel.get(index + menuView.countOfUpperRowItems).icon
                            anchors.centerIn: parent
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(menuView.listModel.get(index).qml !== "") {
                                    contentLoader.go(menuView.listModel.get(index+ menuView.countOfUpperRowItems).qml)
                                }
                            }
                        }
                        SequentialAnimation
                        {
                            id: lowRowAnimation
                            PauseAnimation {duration: 300 + index * 100 }
                            NumberAnimation {
                                target: item2
                                duration:  500
                                property: "opacity"
                                from: 0; to: 1;
                            }
                        }
                        Component.onCompleted: { //opacityAnimation.running = true
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
}
