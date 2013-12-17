// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"

FStateView {
    id:sv
    priority: 2

    view: FView {
        //viewClass: baseScreen
        layout: TC1_B1_Grid {
            id:l


            listWrap: false
            list_data: FProxyListModel {
                id: menuProxy
                sourceModel: menuItemData
            }
            list_itemWidget: GridViewItemWidget {

            }
        }

        FMenuModel {
            id: menuItemData
            FMenuElement { id: menuAudio; menuText: "AM"; menuType: FMenuElement.Submenu}
            FMenuElement { id: menuVehicle; menuText: "FM"; menuType: FMenuElement.Submenu; isVisible: false}
            FMenuElement { id: menuClock; menuText: "Sirius"; menuType: FMenuElement.Submenu }
            FMenuElement { id: menuDisplay; menuText: "CD"; menuType: FMenuElement.Submenu }
            FMenuElement { id: menuMC; menuText: "BT Audio"; menuType: FMenuElement.Popup  }
            FMenuElement { menuText: "USB"; menuType: FMenuElement.Popup  }
            FMenuElement { menuText: "Dummy 2"; menuType: FMenuElement.Popup  }
            FMenuElement { menuText: "Dummy 3"; menuType: FMenuElement.Popup  }
            FMenuElement { menuText: "Dummy 4"; menuType: FMenuElement.Popup  }
            FMenuElement { menuText: "Dummy 5"; menuType: FMenuElement.Popup  }
            FMenuElement { menuText: "Dummy 6"; menuType: FMenuElement.Popup  }
            FMenuElement { menuText: "Dummy 7"; menuType: FMenuElement.Popup  }
            FMenuElement { menuText: "Dummy 8"; menuType: FMenuElement.Popup  }
        }

        FViewUseCaseSimple { condition: sv.isInEntry
            PropertyChanges { target: l; list_data: null }
        }
    }

    baseBehavior: GenListHandling {}

    triggers: [
        // Example for a menu element trigger. This will only work for FMenuModels.
        FTrgMenuElement { menuElement: menuAudio
            FGrdCondition { condition: menuAudio.isEnabled
                FActViewShow { view: "MenuAudioSettings"; animation: "AnimWiperRight" }
            }
        },
        FTrgMenuElement { menuElement: menuClock
            FActViewShow { view: "MenuTopLevelMenu" }
        },
        FTrgMenuElement { menuElement: menuVehicle
            FActViewShow { view: "MenuTopLevelMenu"; animation: "AnimWiperRight" }
        },
        // Example to use a list model trigger. This will work for all classes derived from FAbstractListModel (E.g. TreeModel, MenuModel)
        FTrgListModel { id: trgList; model: menuItemData
            FActScriptCall { onScript: { FLogger.debug("Test", trgList.element.menuText + ", index: " + trgList.index) } }
        },
        FTrgEntry {
            FActScriptCall { onScript: { FLogger.debug("Test", "Entry") } }
        },
        FTrgEntryComplete {
            FActScriptCall { onScript: { FLogger.debug("Test", "EntryComplete") } }
        },
        FTrgExit {
            FActScriptCall { onScript: { FLogger.debug("Test", "Exit") } }
        },
        FTrgExitComplete {
            FActScriptCall { onScript: { FLogger.debug("Test", "ExitComplete") } }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }
    ]
}
