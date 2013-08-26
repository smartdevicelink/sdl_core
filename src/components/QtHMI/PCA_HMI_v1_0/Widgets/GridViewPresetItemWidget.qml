// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"

FWidget {
    id: w
    height: 115
    width: 169

    property int currPreset: 0
    property bool pressed: false



    layout: GridViewPresetItemLayout {
        id: l
        presetName: menuText
        pressed: w.pressed
        presetNumber: index + 1
    }

    FViewUseCaseSimple {
        condition: isEnabled == false
        PropertyChanges { target: l
            opacity: 0.5
        }
    }

    FViewUseCaseSimple {
        condition: w.currPreset===index+1
        PropertyChanges { target: w; pressed: true}
    }

    triggers: [
        FTrgTouchArea { touchArea: l.touchBtn.touchBtn; touchAction: TA.Out
            FActScriptCall { onScript: w.GridView.view.model.selectElement(index) }
        }
    ]
}
