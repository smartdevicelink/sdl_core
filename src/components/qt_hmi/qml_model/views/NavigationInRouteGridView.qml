import QtQuick 2.0

Item {
    id: navInRoute
    anchors.fill: parent

    Image {
        // 3/4 top screen
        id: map
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        source: "../res/nav/map.png"

        Image{
            id: compas
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 1/4 * compas.width
            anchors.left: parent.left
            anchors.leftMargin: 1/4 * compas.width
            source: "../res/nav/compass.png"
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: compas.horizontalCenter
            spacing: 1/8 * parent.height

            // Zoom "+" button
            Image{
                id: zoomIn
                source: "../res/buttons/zoom.png"

                Text{
                    id: zoomInBtn
                    anchors.centerIn: parent
                    text: "+"
                    color: "white"
                    font.pixelSize: 30
                }

                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        parent.source = "../res/buttons/zoom_pressed.png"
                        zoomInBtn.color = "black"
                    }
                    onReleased:  {
                        parent.source = "../res/buttons/zoom.png"
                        zoomInBtn.color = "white"
                    }

                    onClicked: {
                        //Some behavior
                    }
                }
            }

            // Zoom "-" button
            Image{
                id: zoomOut
                source: "../res/buttons/zoom.png"

                Text{
                    id: zoomOutBtn
                    anchors.centerIn: parent
                    text: "-"
                    color: "white"
                    font.pixelSize: 30
                }

                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        parent.source = "../res/buttons/zoom_pressed.png"
                        zoomOutBtn.color = "black"
                    }
                    onReleased:  {
                        parent.source = "../res/buttons/zoom.png"
                        zoomOutBtn.color = "white"
                    }

                    onClicked: {
                        //Some behavior
                    }
                }
            }
        } // column

        // Options button
        Image{
            id: navOptions
            anchors.top: parent.top
            anchors.right: parent.right
            source: "../res/nav/options.png"

            MouseArea{
                anchors.fill: parent
                onPressed: {
                    parent.source = "../res/nav/options_pressed.png"
                }
                onReleased:  {
                    parent.source = "../res/nav/options.png"
                }
                onClicked: {
                //Options screen
                }
            }
        }
    }   //map

    Item{
        // 1/4 bottom screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        Image{
           id: muteBtnImg
           anchors.verticalCenter: parent.verticalCenter
           anchors.left: parent.left
           anchors.leftMargin: 1/4 * muteBtnImg.width
           source: "../res/nav/mute_off.png"
                MouseArea{
                  anchors.fill: parent
                  onClicked: if (muteBtnImg.state === "off") {
                                   muteBtnImg.state = "on"
                                   muteBtnImg.source = "../res/nav/mute_off.png"
                             }
                             else {
                                 muteBtnImg.source = "../res/nav/mute_on.png"
                                 muteBtnImg.state = "off"
                             }
                }
         }

        Image{
            id: turnArrow
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: muteBtnImg.right
            anchors.leftMargin: 1/20 * contentLoader.width
            source: "../res/nav/turnArrow.png"
        }

        Text{
            id: street
            anchors.verticalCenter: cancel.verticalCenter
            anchors.left: turnArrow.right
            anchors.leftMargin: 1/20 * contentLoader.width
            text: "0.2 mi on Cherry Hill Rd."
            color: "White"
            font.pixelSize: 20
        }

        Text{
            id: timeToDest
            anchors.verticalCenter: cancel.verticalCenter
            anchors.right: cancel.left
            anchors.rightMargin: 1/20 * contentLoader.width
            text: "2 hrs 27 min"
            color: "White"
            font.pixelSize: 20
        }

        Image{
            id: cancel
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right

            source: "../res/buttons/long_oval_btn.png"

            Image{
                id: destIcon
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
                source: "../res/nav/dest_icon.png"
            }

            Text{
                id: cancelText
                anchors.centerIn: parent
                text: "Cancel"
                color: "#1d81d5"
                font.pixelSize: 20
            }

            MouseArea {
                anchors.fill: parent

                onPressed: {
                    cancel.source = "../res/buttons/long_oval_pressed_btn.png"
                    destIcon.source = "../res/nav/dest_icon_black.png"
                    cancelText.color = "black"
                }

                onClicked: {
                    contentLoader.go("./views/NavigationNoRouteGridView.qml")
                }
            }
        }
    }
}



    /*


    Image {
        anchors.verticalCenter: compas.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        source: "../res/nav/current_street.png"

        Text{
            anchors.centerIn: parent
            text: "East Capital St. NE"
            color: "White"
            font.pixelSize: 18
        }
    }

    Image{
       id: muteBtnImg
       anchors.verticalCenter: cancel.verticalCenter
       anchors.horizontalCenter: zoomIn.horizontalCenter
       source: "../res/nav/mute_off.png"
            MouseArea{
              anchors.fill: parent
              onClicked: if (muteBtnImg.state === "off") {
                               muteBtnImg.state = "on"
                               muteBtnImg.source = "../res/nav/mute_off.png"
                         }
                         else {
                             muteBtnImg.source = "../res/nav/mute_on.png"
                             muteBtnImg.state = "off"
                         }
            }
     }

    Image{
        id: turnArrow
        anchors.verticalCenter: cancel.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 1/5 * contentLoader.width
        source: "../res/nav/turnArrow.png"
    }

    Text{
        id: street
        anchors.verticalCenter: cancel.verticalCenter
        anchors.left: turnArrow.right
        anchors.leftMargin: 1/20 * contentLoader.width
        text: "0.2 mi on Cherry Hill Rd."
        color: "White"
        font.pixelSize: 20
    }

    Text{
        id: timeToDest
        anchors.verticalCenter: cancel.verticalCenter
        anchors.right: cancel.left
        anchors.rightMargin: 1/20 * contentLoader.width
        text: "2 hrs 27 min"
        color: "White"
        font.pixelSize: 20
    }

    Image{
        id: cancel
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        source: "../res/buttons/long_oval_btn.png"

        Image{
            id: destIcon
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
            source: "../res/nav/dest_icon.png"
        }

        Text{
            id: cancelText
            anchors.centerIn: parent
            text: "Cancel"
            color: "#1d81d5"
            font.pixelSize: 20
        }

        MouseArea {
            anchors.fill: parent

            onPressed: {
                cancel.source = "../res/buttons/long_oval_pressed_btn.png"
                destIcon.source = "../res/nav/dest_icon_black.png"
                cancelText.color = "black"
            }

            onClicked: {
                contentLoader.go("./views/NavigationMenuGridView.qml")
            }
        }
    }
}

*/
