import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"


FLayout {

    id: theLayout
    width: 800
    height: 480

    property alias list : theListPhoneView
    property alias list_itemWidget: theListPhoneView.itemWidget
    property alias list_data: theListPhoneView.data
    property alias back: back.touchBtn
    TouchButtonClimatePCA{
        id: back
        text: "+"
        x:25
        y:80
        image: "buttons/zoom_9_9.png"
        imageActive: "buttons/zoomSelected.png"
        imagePressed: "buttons/zoomPressed_9_9.png"
        txtColor: "white"
        txtSize: 40
        visible: false

    }
    FWdgtListView {

        width:600
        height:400
        id: theListPhoneView
        x:150
        y:80
        clip: true
        visible:true
    }

}
