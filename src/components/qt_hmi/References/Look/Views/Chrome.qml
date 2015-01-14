// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 8



    view: FView {
        id: v

        layout: ChromeLayout {
            id:chrome
            button2.indicatorState: condPool.settingsOpen

        }


    }

    /*FViewUseCaseSimple{condition:condPool.settingsOpen
        PropertyChanges {
            target: chrome.button2; indicatorState: true
        }
    }*/

    triggers: [
        FTrgEntry {

        },

        FTrgHardwareKey { hardkeys: hk_OnOff; keyAction: HK.In
            FActEventFire { event: closeNormalScreens }
            FActViewShow { view: "Off" }
        },
        FTrgTouchArea { touchArea:chrome.button1; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:chrome.button2; touchAction: TA.Out
            FActViewShow { view: "List" }
        },
        FTrgTouchArea { touchArea:chrome.button3; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
