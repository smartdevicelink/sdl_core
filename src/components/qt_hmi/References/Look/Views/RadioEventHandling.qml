// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 0

    view: FView {
        layout: BlankLayout {
            id:l
        }
    }

    triggers: [
        FTrgEntry {
            FGrdCondition{ condition: dataPool.currSource==="FM"
                FActViewShow{ view: "FMPCA"}
                FActViewHide{}
            }
            FGrdCondition{ condition: dataPool.currSource==="USB"
                FActViewShow{ view: "USB"}
                FActViewHide{}
            }
            FGrdCondition{ condition: dataPool.currSource==="AM"
                FActViewShow{ view: "AM"}
                FActViewHide{}
            }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }
    ]
}
