// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 1

    view: FView {
        layout: OffLayout {
           id:l
        }
    }

    triggers: [
        FTrgHardwareKey { hardkeys: hk_OnOff; keyAction: HK.In
            FActViewShow { view: "On" }
        },
        FTrgTouchArea { touchArea:l.offTouch; touchAction: TA.Out
            FActViewShow { view: "On" }
        },
        /*FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        },*/
        FTrgEntry {
            //FActScriptCall { onScript: dataPool.phoneCrnIcons = false}
            //FActScriptCall { onScript: dataPool.phonePaired = false}
        }

    ]
}
