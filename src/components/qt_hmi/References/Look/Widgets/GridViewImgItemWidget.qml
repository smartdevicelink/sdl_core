// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"

FWidget {
    id: w
    height: 178
    width: 200



    layout: GridViewImgItemLayout {
        id: l
        bitmap: menuIcon
    }

    FViewUseCaseSimple {
        condition: isEnabled == false
        PropertyChanges { target: l
            opacity: 0.5
        }
    }

    triggers: [
        FTrgTouchArea { touchArea: l.touch; touchAction: TA.Out
            FActScriptCall { onScript: w.GridView.view.model.selectElement(index) }
        }
    ]
}
