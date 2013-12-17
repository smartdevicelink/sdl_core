// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 5
    property string dirTuneEntry: ""


    FTimer { id: popupEnd; interval: 2500; repeat: false }

    view: FView {
        id: v

        layout: ControlMirrorLayout {
            id:home
        }


    }




    triggers: [
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        },
        FTrgEntry{
            FActTimer { timer: popupEnd; action: TM.Start }
        },
        FTrgTimer{timer: popupEnd
            FActTimer { timer: popupEnd; action: TM.Stop }
            FActViewHide {  }
        },
        FTrgHardwareKey { hardkeys: buttonUp; keyAction: HK.In
            FActTimer { timer: popupEnd; action: TM.Restart }
           // FActScriptCall { onScript: condPool.climateOnOff = true }
            FGrdCondition { condition: dataPool.drvTemp !== 80
                FActScriptCall { onScript: dataPool.drvTemp = (dataPool.drvTemp+1)  }
            }
        },
        FTrgHardwareKey { hardkeys: buttonDown; keyAction: HK.In
            FActTimer { timer: popupEnd; action: TM.Restart }
           // FActScriptCall { onScript: condPool.climateOnOff = true }
            FGrdCondition { condition: dataPool.drvTemp !== 65
                FActScriptCall { onScript: dataPool.drvTemp = (dataPool.drvTemp-1) }
            }
        }
    ]
}
