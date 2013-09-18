import QtQuick 2.0

Item{
    id: navNoRoute
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
        id: bottomPart
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        // Destination search button
        Image{
            id: destSearch
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            source: "../res/buttons/longest_oval_btn.png"

            Image{
                id: destIcon
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 15
                source: "../res/nav/dest_icon.png"
            }

            Text{
                id: destText
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: destIcon.right
                anchors.leftMargin: 5
                text: "Destination"
                color: "#1d81d5"
                font.pixelSize: 20
            }

            MouseArea {
                anchors.fill: parent

                onPressed: {
                    destSearch.source = "../res/buttons/longest_oval_btn_pressed.png"
                    destIcon.source = "../res/nav/dest_icon_black.png"
                    destText.color = "black"
                }

                onReleased: {
                    destSearch.source = "../res/buttons/longest_oval_btn.png"
                    destIcon.source = "../res/nav/dest_icon.png"
                    destText.color = "#1d81d5"
                }

                onClicked: {
                    contentLoader.go("./views/NavigationMenuGridView.qml")
                }
            }
        }
    }
}
