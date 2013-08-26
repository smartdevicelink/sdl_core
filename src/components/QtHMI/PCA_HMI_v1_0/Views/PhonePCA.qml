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
        layout: TileBackImgLayoutPCA {
            id:l

            //useClose: true
            listWrap: false
            list_data: FProxyListModel {
                id: menuProxy
                sourceModel: menuItemData
            }
            list_itemWidget: GridViewImgItemWidget {

            }
        }

        FMenuModel {
            id: menuItemData
            FMenuElement { id: phone; menuIcon: "menu/phone.png"}
            FMenuElement { id: dnd; menuIcon: "phone/DNDoff.png" }
            FMenuElement { id: phoneBook; menuIcon: "phone/contacts.png"  }
            FMenuElement { id: recent; menuIcon: "phone/recent.png"  }
            FMenuElement { id: messages; menuIcon: "phone/messages.png"   }
            FMenuElement { id: keyPad; menuIcon: "phone/keypad.png"  }
            FMenuElement { id: textMsg; menuIcon: "phone/texts.png"  }
            FMenuElement { id: more; menuIcon: "phone/more.png"}
        }

        FViewUseCaseSimple { condition: sv.isInEntry
            PropertyChanges { target: l; list_data: null }
        }
    }

    baseBehavior: GenListHandling {}

    triggers: [
        // Example for a menu element trigger. This will only work for FMenuModels.
        FTrgMenuElement { menuElement: phone
        },
        FTrgMenuElement { menuElement: dnd
        },
        FTrgMenuElement { menuElement: phoneBook
            FActViewShow { view: "AlphaSortPCA" }
        },
        FTrgMenuElement { menuElement: recent
        },
        FTrgMenuElement { menuElement: more
        },
        FTrgMenuElement { menuElement: messages
        },
        FTrgMenuElement { menuElement: keyPad
        },
        FTrgMenuElement { menuElement: textMsg
        },
        FTrgTouchArea { touchArea:l.close; touchAction: TA.Out
            FActViewShow { view: "GridMenuPCA" }
        },
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
        }
    ]
}
