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

        layout: FMLayout {
            id:home
            albumInfoBtn.vis: false
            albumInfoImg.visible: false
            alertBtn.vis:  false
            browseBtn.vis: false
            replayBtn.vis: false
            preset1Btn.text: "WJR"
            preset2Btn.text: "1130"
            preset3Btn.text: "950"
            preset4Btn.text: "WWJ"
            preset5Btn.text: "WXYZ"
            preset6Btn.text: "1330"
            artistTxt.text: ""
            stationTxt.text: ""
            trackTxt.text: "950"


        }


    }

    triggers: [



        FTrgTouchArea { touchArea:home.sourceBtn; touchAction: TA.Out
            FActViewShow { view: "SourceChange" }
        },
        FTrgTouchArea { touchArea:home.browseBtn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.albumInfoBtn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.preset1Btn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.preset2Btn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.preset3Btn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.preset4Btn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.preset5Btn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.preset6Btn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.replayBtn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.tuneBtn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.presetsBtn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
