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
            preset1Btn.text: dataPool.fm1pre1
            preset2Btn.text: dataPool.fm1pre2
            preset3Btn.text: dataPool.fm1pre3
            preset4Btn.text: dataPool.fm1pre4
            preset5Btn.text: dataPool.fm1pre5
            preset6Btn.text: dataPool.fm1pre6
            trackTxt.text: dataPool.currStation
            stationTxt.text: dataPool.currStatText
            artistTxt.text: dataPool.currSong
            albumInfoBtn.bitmap: dataPool.currAlbumArt
        }


    }



    triggers: [



        FTrgTouchArea { touchArea:home.sourceBtn.touchBtn; touchAction: TA.Out
            FActViewShow { view: "SourceChange" }
        },
        FTrgTouchArea { touchArea:home.browseBtn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.albumInfoBtn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.preset1Btn; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.currStation = dataPool.fm1pre1 }
            FActScriptCall { onScript:dataPool.currStatText = dataPool.fm1stat1}
            FActScriptCall {  onScript:dataPool.currSong=dataPool.fm1song1 }
        },
        FTrgTouchArea { touchArea:home.preset2Btn; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.currStation = dataPool.fm1pre2 }
            FActScriptCall { onScript:dataPool.currStatText = dataPool.fm1stat2}
            FActScriptCall {  onScript:dataPool.currSong=dataPool.fm1song2 }
        },
        FTrgTouchArea { touchArea:home.preset3Btn; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.currStation = dataPool.fm1pre3 }
            FActScriptCall { onScript:dataPool.currStatText = dataPool.fm1stat3}
            FActScriptCall {  onScript:dataPool.currSong=dataPool.fm1song3 }
        },
        FTrgTouchArea { touchArea:home.preset4Btn; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.currStation = dataPool.fm1pre4 }
            FActScriptCall { onScript:dataPool.currStatText = dataPool.fm1stat4}
            FActScriptCall {  onScript:dataPool.currSong=dataPool.fm1song4 }
        },
        FTrgTouchArea { touchArea:home.preset5Btn; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.currStation = dataPool.fm1pre5 }
            FActScriptCall { onScript:dataPool.currStatText = dataPool.fm1stat5}
            FActScriptCall {  onScript:dataPool.currSong=dataPool.fm1song5 }
        },
        FTrgTouchArea { touchArea:home.preset6Btn; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.currStation = dataPool.fm1pre6 }
            FActScriptCall { onScript:dataPool.currStatText = dataPool.fm1stat6}
            FActScriptCall {  onScript:dataPool.currSong=dataPool.fm1song6 }
        },
        FTrgTouchArea { touchArea:home.replayBtn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },
        FTrgTouchArea { touchArea:home.tuneBtn.touchBtn; touchAction: TA.Out
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
