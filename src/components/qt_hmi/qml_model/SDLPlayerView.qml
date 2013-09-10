import QtQuick 2.0

Column {
    anchors.fill: parent
    Row{
        width: parent.width
        height: parent.height / 4
        GridView{
            width: parent.width
            model: ListModel{
                ListElement{
                    name: "SDL Music"
                }
                ListElement{
                    name: "Device 1"
                }
                ListElement{
                    name: "Options"
                }
                ListElement{
                    name: "Browse"
                }
            }
            delegate:  Text{
                color: "#1d81d5"
                text: name
                width: parent.width / 4
            }
        }
    }
    Row{
        width: parent.width
        height: parent.height / 4
        Image {
            source: "res/SPT.png"
        }
        Text {
            color: "#1d81d5"
            text: "Song name<br>Album name"
        }
    }
    Row{
        width: parent.width
        height: parent.height / 4
        Text {
            color: "#1d81d5"
            text: "02:36"
        }
    }
    Row{
        width: parent.width
        height: parent.height / 4
        //play pause control
    }

}
