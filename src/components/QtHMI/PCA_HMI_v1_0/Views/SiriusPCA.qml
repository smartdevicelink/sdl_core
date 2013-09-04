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

        layout: FMLayoutPCA {
            id:home
            sourceBtn.text: "Sirius"
            sourceBtn.textWidth: 70
            freqId: ""
            curPreset: dataPool.currSATpreset
            hdBtn.visible: false


            listModel: FProxyListModel {
                id: menuProxy
                sourceModel: menuItemData
            }
        }


        FMenuModel {
            id: menuItemData
            FMenuElement { id: preset1; menuText: "Lithium"; menuData: true; }
            FMenuElement { id: preset2; menuText: "Spectrum"; menuData: false }
            FMenuElement { id: preset3; menuText: "ESPN"; menuData: false }
            FMenuElement { id: preset4; menuText: "Alt Nation"; menuData: false }
            FMenuElement { id: preset5; menuText: "NHL";  menuData: false }
            FMenuElement { id: preset6; menuText: "the Highway"; menuData: false  }
            FMenuElement { id: preset7; menuText: "SiriusXMU"; menuData: false  }
            FMenuElement { id: preset8; menuText: "First Wave"; menuData: false}
        }
    }

    triggers: [

        FTrgEntry{
            FGrdCondition{condition: dataPool.currSATpreset === 1
                FActScriptCall { onScript: home.stationTxt = "Lithium" }
            }
            FGrdCondition{condition: dataPool.currSATpreset === 2
                FActScriptCall { onScript: home.stationTxt = "Spectrum" }
            }
            FGrdCondition{condition: dataPool.currSATpreset === 3
                FActScriptCall { onScript: home.stationTxt = "ESPN" }
            }
            FGrdCondition{condition: dataPool.currSATpreset === 4
                FActScriptCall { onScript: home.stationTxt = "Alt Nation" }
            }
            FGrdCondition{condition: dataPool.currSATpreset === 5
                FActScriptCall { onScript: home.stationTxt = "NHL" }
            }
            FGrdCondition{condition: dataPool.currSATpreset === 6
                FActScriptCall { onScript: home.stationTxt = "the Highway" }
            }
            FGrdCondition{condition: dataPool.currSATpreset === 7
                FActScriptCall { onScript: home.stationTxt = "SiriusXMU" }
            }
            FGrdCondition{condition: dataPool.currSATpreset === 8
                FActScriptCall { onScript: home.stationTxt = "First Wave" }
            }
        },
        FTrgTouchArea { touchArea:home.sourceBtn.touchBtn; touchAction: TA.Out
            FActViewShow { view: "SourceChangePCA" }
        },
        FTrgMenuElement { menuElement: preset1
            FActScriptCall { onScript: home.stationTxt = "Lithium" }
            FActScriptCall { onScript: dataPool.currSATpreset = 1 }
        },
        FTrgMenuElement { menuElement: preset2
            FActScriptCall { onScript: home.stationTxt = "Spectrum" }
            FActScriptCall { onScript: dataPool.currSATpreset = 2 }
        },
        FTrgMenuElement { menuElement: preset3
            FActScriptCall { onScript: home.stationTxt = "ESPN" }
            FActScriptCall { onScript: dataPool.currSATpreset = 3 }
        },
        FTrgMenuElement { menuElement: preset4
            FActScriptCall { onScript: home.stationTxt = "Alt Nation" }
            FActScriptCall { onScript: dataPool.currSATpreset = 4 }
        },
        FTrgMenuElement { menuElement: preset5
            FActScriptCall { onScript: home.stationTxt = "NHL" }
            FActScriptCall { onScript: dataPool.currSATpreset = 5 }
        },
        FTrgMenuElement { menuElement: preset6
            FActScriptCall { onScript: home.stationTxt = "the Highway" }
            FActScriptCall { onScript: dataPool.currSATpreset = 6 }
        },
        FTrgMenuElement { menuElement: preset7
            FActScriptCall { onScript: home.stationTxt = "SiriusXMU" }
            FActScriptCall { onScript: dataPool.currSATpreset = 7 }
        },
        FTrgMenuElement { menuElement: preset8
            FActScriptCall { onScript: home.stationTxt = "First Wave" }
            FActScriptCall { onScript: dataPool.currSATpreset = 8 }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
