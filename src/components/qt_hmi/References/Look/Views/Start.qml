// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 2
    attachedViews: ["Chrome"]



    view: FView {
        id: v

        layout: StartLayout {
            id:corner

        }


    }

    triggers: [
        FTrgEntry {

        },

        /*FTrgHardwareKey { hardkeys: hk_OnOff; keyAction: HK.In
            FActEventFire { event: closeNormalScreens }
            FActViewShow { view: "Off" }
        },*/
        FTrgTouchArea { touchArea:corner.button1; touchAction: TA.Out
            FActViewShow { view: "Home" }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
