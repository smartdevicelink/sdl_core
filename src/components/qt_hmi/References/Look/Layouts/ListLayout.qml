import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"


FLayout {

    id: theLayout
    width: 800
    height: 480
    x:10
    y:10
    property alias list : theListView
    property alias list_itemWidget: theListView.itemWidget
    property alias list_data: theListView.data
    Rectangle{
        anchors.fill:parent
        color:"white"

    }


    FWdgtListView {

        id: theListView
        anchors.fill: parent
        clip: true

    }
    FWdgtListView {
        x:400
        id: filteredListView
        clip: true
        data:filtermodel
        itemWidget:list_itemWidget
        width:100
        height:400
    }
    TextInput{
        text:"HelloWorld"
        x:200
        y:20
        onTextChanged: {

            if(text.length > 0 ) {
                            dataPool.filter = text
                console.log(dataPool.filter)

                            sortList.fillFilterModel(theListView.data)//Just backup original
                            sortList.updateFilterModel(text)

                        } else {
                            sortList.fillFilterModel(theListView.data)
                        }
        }

    }

}
