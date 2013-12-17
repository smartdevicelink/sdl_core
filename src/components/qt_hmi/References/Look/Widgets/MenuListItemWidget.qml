// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"

FWidget {
    id: w
    height: 50

    layout: TGListItemLayout {
        id: l
        anchors.fill: parent
        ax_bitmap: ""
        cx_text: menuText
        activeMarker_visible: false
        indentLevel: level
    }

    FViewUseCaseSimple {
        condition: isEnabled == false
        PropertyChanges { target: l
            opacity: 0.5
        }
    }

    FViewUseCaseGroup {
        FViewUseCase {
            condition: menuType == FMenuElement.Popup
            PropertyChanges { target: l
                ax_bitmap: "../Resources/g137p.png"
            }
        }

        FViewUseCase {
            condition: menuType == FMenuElement.Submenu
            PropertyChanges { target: l
                ax_bitmap: "../Resources/g136p.png"
            }
        }
    }

    triggers: [
        FTrgTouchArea { touchArea: l.touch; touchAction: TA.In
            FActScriptCall { onScript: w.ListView.view.model.selectElement(index) }
        }
    ]
}
