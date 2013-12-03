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

        layout: USBLayoutPCA {
            id:home
            sourceBtn.text: "SDL Music"
            sourceBtn.textWidth: 100
            playing: true
        }


    }



    triggers: [



        FTrgTouchArea { touchArea:home.sourceBtn.touchBtn; touchAction: TA.Out
            FActViewShow { view: "SourceChangePCA" }
        },
        FTrgTouchArea { touchArea:home.play; touchAction: TA.Out
            FActScriptCall { onScript: home.playing = false }
        },
        FTrgTouchArea { touchArea:home.pause; touchAction: TA.Out
            FActScriptCall { onScript: home.playing = true }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
