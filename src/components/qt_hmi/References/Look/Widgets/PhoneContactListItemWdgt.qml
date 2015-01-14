import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"


FWidget {
    id: w
    height: 50

    property FEvent itemClicked

    layout: PhoneListWdgtLayout {
        anchors.fill: parent
        id: l
        cx_text: staticItem
        cn_text: staticNumber
        ct_text: type
    }
    triggers: [
        FTrgTouchArea { touchArea: l.touch; touchAction: TA.Out
            FActEventFire{ event: itemClicked; data: index }
            FActScriptCall { onScript: console.log(staticItem) }
        }
    ]


}
