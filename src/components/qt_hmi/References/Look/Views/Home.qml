// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 2
    //attachedViews: ["Chrome"]



    view: FView {
        id: v

        layout: HomeLayout {
            id:home

        }


    }

    triggers: [


        /*FTrgHardwareKey { hardkeys: hk_OnOff; keyAction: HK.In
            FActEventFire { event: closeNormalScreens }
            FActViewShow { view: "Off" }
        },*/
        FTrgTouchArea { touchArea:home.button1; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.button2; touchAction: TA.Out
            FActViewShow { view: "RadioEventHandling" }
        },
        FTrgTouchArea { touchArea:home.button3; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.button4; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
