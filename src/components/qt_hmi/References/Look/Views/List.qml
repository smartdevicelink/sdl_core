// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 2



    view: FView {
        id: v

        layout: ListLayout {
            id:list


            listWrap: true
            list_data: menuItemData
            list_itemWidget: MenuListItemWidget { }

        }


    }

    FMenuModel {
        id: menuItemData
        FMenuElement { id: menuAudio; menuText: "Audio Settings"; menuType: FMenuElement.Submenu
            FMenuElement { id: menuAudio1; menuText: "Audio 1"; menuType: FMenuElement.Submenu
                FMenuElement { id: menuAudio11; menuText: "Audio 11"; menuType: FMenuElement.Popup  }
                FMenuElement { id: menuAudio12; menuText: "Audio 12"; menuType: FMenuElement.Popup  }
            }
            FMenuElement { id: menuAudio2; menuText: "Audio 2"; menuType: FMenuElement.Popup  }
            FMenuElement { id: menuAudio3; menuText: "Audio 3"; menuType: FMenuElement.Popup  }
        }
        FMenuElement { id: menuVehicle; menuText: "Vehicle Settings"; menuType: FMenuElement.Submenu; isVisible: false }
        FMenuElement { id: menuClock; menuText: "Clock Settings"; menuType: FMenuElement.Submenu }
        FMenuElement { id: menuDisplay; menuText: "Display Settings"; menuType: FMenuElement.Submenu }
        FMenuElement { id: menuMC; menuText: "Message Centre"; menuType: FMenuElement.Popup  }
    }

    triggers: [
        FTrgEntry {
            FActScriptCall {onScript: condPool.settingsOpen=true}

        },
        FTrgExit {
            FActScriptCall {onScript: condPool.settingsOpen=false}
        },

        /*FTrgHardwareKey { hardkeys: hk_OnOff; keyAction: HK.In
            FActEventFire { event: closeNormalScreens }
            FActViewShow { view: "Off" }
        },*/
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        },


        // Example for a menu element trigger. This will only work for FMenuModels.
        FTrgMenuElement { menuElement: menuAudio
            FActScriptCall { onScript: menuAudio.isOpened = !menuAudio.isOpened }
        },
        // Example to use a list model trigger. This will work for all classes derived from FAbstractListModel (E.g. TreeModel, MenuModel)
        FTrgListModel { id: trgList; model: menuItemData
            FActScriptCall { onScript: { FLogger.debug("Test", trgList.element.menuText + ", index: " + trgList.index) } }
        }

    ]
}
