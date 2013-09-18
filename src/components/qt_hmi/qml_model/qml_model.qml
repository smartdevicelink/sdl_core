import QtQuick 2.0
import com.ford.hmi_framework 1.0
import sdl.core.api 1.0
import "./controls"
import "./views"

Rectangle{
    width: 1600
    height: 768
    property string startQml: "./views/AMFMPlayerView.qml"
    property int margin: 20
    property int minWidth: 600
    property int minHieght: 400
    color: "black"

    Item {
        id: mainScreen
        width: parent.width * 0.62 < minWidth ? minWidth : parent.width * 0.62
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        height: parent.height < minHieght ? minHieght : parent.height
        visible: false

        Item{
            height: parent.height * 0.25
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            HeaderMenu{}
        }

        Item{
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.bottomMargin: 30
            anchors.fill: parent
            Loader {
                id: contentLoader
                height: parent.height * 0.75
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                source:startQml

                ListModel {
                    id: urlStack
                }

                function go(path) {
                    urlStack.append({ url: source.toString(), index: 10 })
                    source = path
                }

                function back() {
                    var item = urlStack.get(urlStack.count - 1)
                    source = item.url
                    urlStack.remove(item)
                }
            }
        }
    }

    Item {
        id: hwBtnScreen
        width: parent.width * 0.38
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: mainScreen.right
        height: parent.height < minHieght ? minHieght : parent.height
        HardwareButtonsView {}
    }

    Api {
    }
}

