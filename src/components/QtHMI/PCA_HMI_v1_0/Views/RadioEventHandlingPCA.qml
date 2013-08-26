// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 0

    view: FView {
        layout: BlankLayout {
            id:l
        }
    }

    triggers: [
        FTrgEntry {
            FGrdCondition{ condition: dataPool.currSource==="FM"
                FActViewShow{ view: "FMPCA"}
                FActViewHide{}
            }
            FGrdCondition{ condition: dataPool.currSource==="USB"
                FActViewShow{ view: "USBPCA"}
                FActViewHide{}
            }
            FGrdCondition{ condition: dataPool.currSource==="AM"
                FActViewShow{ view: "AMPCA"}
                FActViewHide{}
            }
            FGrdCondition{ condition: dataPool.currSource==="Sirius"
                FActViewShow{ view: "SiriusPCA"}
                FActViewHide{}
            }
            FGrdCondition{ condition: dataPool.currSource==="CD"
                FActViewShow{ view: "CDPCA"}
                FActViewHide{}
            }
            FGrdCondition{ condition: dataPool.currSource==="BT_Audio"
                FActViewShow{ view: "BT_AudioPCA"}
                FActViewHide{}
            }
            FGrdCondition{ condition: dataPool.currSource==="iPod"
                FActViewShow{ view: "iPodPCA"}
                FActViewHide{}
            }
            FGrdCondition{ condition: dataPool.currSource==="LineIn"
                FActViewShow{ view: "AuxPCA"}
                FActViewHide{}
            }
            FGrdCondition{ condition: dataPool.currSource==="SDL_Music"
                FActViewShow{ view: "SDL_MusicPCA"}
                FActViewHide{}
            }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }
    ]
}
