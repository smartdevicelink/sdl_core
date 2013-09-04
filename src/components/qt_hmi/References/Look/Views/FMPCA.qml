// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 2
    attachedViews: ["PCAstatusBar"]
    property int currHD: 1
    property int totalHD: 4



    view: FView {
        id: v

        layout: FMLayoutPCA {
            id:home
            sourceBtn.text: "FM Radio"
            sourceBtn.textWidth: 100
            freqId: "FM"
            curHD: currHD
            totHD: totalHD
            curPreset: dataPool.currFMpreset

            listModel: FProxyListModel {
                id: menuProxy
                sourceModel: menuItemData
            }
        }


        FMenuModel {
            id: menuItemData
            FMenuElement { id: preset1; menuText: "96.3"; menuData: false; }
            FMenuElement { id: preset2; menuText: "107.9"; menuData: false }
            FMenuElement { id: preset3; menuText: "104.3"; menuData: false }
            FMenuElement { id: preset4; menuText: "101.9"; menuData: false }
            FMenuElement { id: preset5; menuText: "97.1";  menuData: false }
            FMenuElement { id: preset6; menuText: "91.7"; menuData: false  }
            FMenuElement { id: preset7; menuText: "101.1"; menuData: false  }
            FMenuElement { id: preset8; menuText: "91.9"; menuData: false}
        }
    }


    triggers: [

        FTrgCondition{ condition: home.moving === true
            FActScriptCall { onScript: home.page1=!home.page1 }
        },
        FTrgEntry{
            FGrdCondition{condition: dataPool.currFMpreset === 1
                FActScriptCall { onScript: home.stationTxt = "96.3" }
            }
            FGrdCondition{condition: dataPool.currFMpreset === 2
                FActScriptCall { onScript: home.stationTxt = "107.9" }
            }
            FGrdCondition{condition: dataPool.currFMpreset === 3
                FActScriptCall { onScript: home.stationTxt = "104.3" }
            }
            FGrdCondition{condition: dataPool.currFMpreset === 4
                FActScriptCall { onScript: home.stationTxt = "101.9" }
            }
            FGrdCondition{condition: dataPool.currFMpreset === 5
                FActScriptCall { onScript: home.stationTxt = "97.1" }
            }
            FGrdCondition{condition: dataPool.currFMpreset === 6
                FActScriptCall { onScript: home.stationTxt = "91.7" }
            }
            FGrdCondition{condition: dataPool.currFMpreset === 7
                FActScriptCall { onScript: home.stationTxt = "101.1" }
            }
            FGrdCondition{condition: dataPool.currFMpreset === 8
                FActScriptCall { onScript: home.stationTxt = "91.9" }
            }
        },
        FTrgTouchArea { touchArea:home.sourceBtn.touchBtn; touchAction: TA.Out
            FActViewShow { view: "SourceChangePCA" }
        },
        FTrgTouchArea { touchArea:home.hdBtn.touchBtn; touchAction: TA.Out
            FGrdCondition { condition: currHD === totalHD
                FActScriptCall { onScript: currHD = 1 }
            }
            FGrdCondition { condition: currHD !== totalHD
                FActScriptCall { onScript: currHD ++}
            }
        },
        FTrgMenuElement { menuElement: preset1
            FActScriptCall { onScript: home.stationTxt = "96.3" }
            FActScriptCall { onScript: dataPool.currFMpreset = 1 }
        },
        FTrgMenuElement { menuElement: preset2
            FActScriptCall { onScript: home.stationTxt = "107.9" }
            FActScriptCall { onScript: dataPool.currFMpreset = 2 }
        },
        FTrgMenuElement { menuElement: preset3
            FActScriptCall { onScript: home.stationTxt = "104.3" }
            FActScriptCall { onScript: dataPool.currFMpreset = 3 }
        },
        FTrgMenuElement { menuElement: preset4
            FActScriptCall { onScript: home.stationTxt = "101.9" }
            FActScriptCall { onScript: dataPool.currFMpreset = 4 }
        },
        FTrgMenuElement { menuElement: preset5
            FActScriptCall { onScript: home.stationTxt = "97.1" }
            FActScriptCall { onScript: dataPool.currFMpreset = 5 }
        },
        FTrgMenuElement { menuElement: preset6
            FActScriptCall { onScript: home.stationTxt = "91.7" }
            FActScriptCall { onScript: dataPool.currFMpreset = 6 }
        },
        FTrgMenuElement { menuElement: preset7
            FActScriptCall { onScript: home.stationTxt = "101.1" }
            FActScriptCall { onScript: dataPool.currFMpreset = 7 }
        },
        FTrgMenuElement { menuElement: preset8
            FActScriptCall { onScript: home.stationTxt = "91.9" }
            FActScriptCall { onScript: dataPool.currFMpreset = 8 }
        },
        FTrgListModel { id: trgList; model: menuItemData
            FActScriptCall { onScript: { FLogger.debug("Test", trgList.element.menuText + ", index: " + trgList.index) } }
        },
        /*FTrgTouchArea { touchArea:home.browseBtn; touchAction: TA.Out
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

        FTrgTouchArea { touchArea:home.presetsBtn; touchAction: TA.Out
            FActViewShow { view: "Start" }
        },*/
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
