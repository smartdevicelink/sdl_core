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
           offTouch.enabled: false
        }
    }

    triggers: [
        FTrgEntry {
            FActViewShow { view: "RadioEventHandlingPCA" }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewShow { view: "Off" }
        },
        FTrgHardwareKey { hardkeys: hk_OnOff; keyAction: HK.In
            //FActViewShow { view: "Off" }
            FActEventFire{ event: closeNormalScreens }
        },
        FTrgHardwareKey { hardkeys: buttonUp; keyAction: HK.In
            FActViewShow { view: "ControlMirror" }
        },
        FTrgHardwareKey { hardkeys: buttonDown; keyAction: HK.In
            FActViewShow { view: "ControlMirror" }
        }

    ]
}
