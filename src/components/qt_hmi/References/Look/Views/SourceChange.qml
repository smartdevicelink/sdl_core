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

        layout: TileBackLayout {
            id:source
            button1.text: "AM"
            button2.text: "FM"
            button3.text: "Sirius"
            button4.text: "USB"
            button5.text: "BT Audio"
            button6.text: "Line In"
            button7.vis: false
            button8.vis: false
        }


    }

    triggers: [


        /*FTrgHardwareKey { hardkeys: hk_OnOff; keyAction: HK.In
            FActEventFire { event: closeNormalScreens }
            FActViewShow { view: "Off" }
        },*/
        FTrgTouchArea { touchArea:source.button1; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.currSource = "AM" }
            FActViewShow { view: "RadioEventHandling" }
        },
        FTrgTouchArea { touchArea:source.button2; touchAction: TA.Out
            FActViewShow { view: "FM" }
        },
        FTrgTouchArea { touchArea:source.button3; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:source.button4; touchAction: TA.Out
            FActViewShow { view: "USB" }
        },
        FTrgTouchArea { touchArea:source.button5; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:source.button6; touchAction: TA.Out
            FActViewShow { view: "RadioEventHandling" }
        },
        FTrgTouchArea { touchArea:source.button7; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:source.button8; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:source.backBtn; touchAction: TA.Out
            FActViewShow { view: "RadioEventHandling" }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
