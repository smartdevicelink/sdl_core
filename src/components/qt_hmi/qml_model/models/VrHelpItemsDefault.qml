///!!!!!!!!!
import QtQuick 2.0

ListModel {
    ListElement {
        text: "SyncProxyTester"
        moreParameters:[ ListElement {
                value: "/home/aleshin/work/applink/src/appMain/SyncProxyTester/action.png"
                imageType: 1
            } ]
        position: 1
    }
    ListElement {
        text: "XML Test "
        image: ListElement {
            value: "/home/aleshin/work/applink/src/appMain/SyncProxyTester/action.png"
            imageType: 1
        }
        position: 2
    }
    ListElement {
        text: "Policies Test"
        image: ListElement{
            value: "/home/aleshin/work/applink/src/appMain/SyncProxyTester/action.png"
            imageType: 1
        }
        position: 3
    }
}

