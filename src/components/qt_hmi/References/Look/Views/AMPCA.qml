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




    view: FView {
        id: v

        layout: FMLayoutPCA {
            id:home
            sourceBtn.text: "AM Radio"
            freqId: "AM"
            sourceBtn.textWidth: 100
            curPreset: dataPool.currAMpreset
            hdBtn.visible: false

            listModel: FProxyListModel {
                id: menuProxy
                sourceModel: menuItemData
            }
        }


        FMenuModel {
            id: menuItemData
            FMenuElement { id: preset1; menuText: "1130"; menuData: false; }
            FMenuElement { id: preset2; menuText: "950"; menuData: false }
            FMenuElement { id: preset3; menuText: "760"; menuData: true }
            FMenuElement { id: preset4; menuText: "1270"; menuData: false }
            FMenuElement { id: preset5; menuText: "1310";  menuData: false }
            FMenuElement { id: preset6; menuText: "800"; menuData: false  }
            FMenuElement { id: preset7; menuText: "610"; menuData: false  }
            FMenuElement { id: preset8; menuText: "820"; menuData: false}
        }
    }



    triggers: [



        FTrgEntry{
            FGrdCondition{condition: dataPool.currAMpreset === 1
                FActScriptCall { onScript: home.stationTxt = "1130" }
            }
            FGrdCondition{condition: dataPool.currAMpreset === 2
                FActScriptCall { onScript: home.stationTxt = "950" }
            }
            FGrdCondition{condition: dataPool.currAMpreset === 3
                FActScriptCall { onScript: home.stationTxt = "760" }
            }
            FGrdCondition{condition: dataPool.currAMpreset === 4
                FActScriptCall { onScript: home.stationTxt = "1270" }
            }
            FGrdCondition{condition: dataPool.currAMpreset === 5
                FActScriptCall { onScript: home.stationTxt = "1310" }
            }
            FGrdCondition{condition: dataPool.currAMpreset === 6
                FActScriptCall { onScript: home.stationTxt = "800" }
            }
            FGrdCondition{condition: dataPool.currAMpreset === 7
                FActScriptCall { onScript: home.stationTxt = "610" }
            }
            FGrdCondition{condition: dataPool.currAMpreset === 8
                FActScriptCall { onScript: home.stationTxt = "820" }
            }
        },
        FTrgTouchArea { touchArea:home.sourceBtn.touchBtn; touchAction: TA.Out
            FActViewShow { view: "SourceChangePCA" }
        },
        FTrgMenuElement { menuElement: preset1
            FActScriptCall { onScript: home.stationTxt = "1130" }
            FActScriptCall { onScript: dataPool.currAMpreset = 1 }
        },
        FTrgMenuElement { menuElement: preset2
            FActScriptCall { onScript: home.stationTxt = "950" }
            FActScriptCall { onScript: dataPool.currAMpreset = 2 }
        },
        FTrgMenuElement { menuElement: preset3
            FActScriptCall { onScript: home.stationTxt = "760" }
            FActScriptCall { onScript: dataPool.currAMpreset = 3 }
        },
        FTrgMenuElement { menuElement: preset4
            FActScriptCall { onScript: home.stationTxt = "1270" }
            FActScriptCall { onScript: dataPool.currAMpreset = 4 }
        },
        FTrgMenuElement { menuElement: preset5
            FActScriptCall { onScript: home.stationTxt = "1310" }
            FActScriptCall { onScript: dataPool.currAMpreset = 5 }
        },
        FTrgMenuElement { menuElement: preset6
            FActScriptCall { onScript: home.stationTxt = "800" }
            FActScriptCall { onScript: dataPool.currAMpreset = 6 }
        },
        FTrgMenuElement { menuElement: preset7
            FActScriptCall { onScript: home.stationTxt = "610" }
            FActScriptCall { onScript: dataPool.currAMpreset = 7 }
        },
        FTrgMenuElement { menuElement: preset8
            FActScriptCall { onScript: home.stationTxt = "820" }
            FActScriptCall { onScript: dataPool.currAMpreset = 8 }
        },
        FTrgListModel { id: trgList; model: menuItemData
            FActScriptCall { onScript: { FLogger.debug("Test", trgList.element.menuText + ", index: " + trgList.index) } }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
