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

        layout: ClimatePCALayout {
            id:climate
            dualBtnSt: condPool.dualBtnSt
            drvHeatSeatSt: condPool.drvHeatSeatSt
            drvCoolSeatSt: condPool.drvCoolSeatSt
            passCoolSeatSt: condPool.passCoolSeatSt
            passHeatSeatSt: condPool.passHeatSeatSt
            heatWheelSt: condPool.heatWheelSt
            heatWinSheildSt: condPool.heatWinSheildSt
            defrostSt: condPool.defrostSt
            pannelSt: condPool.pannelSt
            floorSt: condPool.floorSt

        }

    }



    triggers: [



        FTrgTouchArea { touchArea:climate.dualBtn; touchAction: TA.Out
            FActScriptCall { onScript: condPool.dualBtnSt = !condPool.dualBtnSt }
        },
        FTrgTouchArea { touchArea:climate.drvHeatSeat; touchAction: TA.Out
             FActScriptCall { onScript: condPool.drvHeatSeatSt = !condPool.drvHeatSeatSt }
        },
        FTrgTouchArea { touchArea:climate.drvCoolSeat; touchAction: TA.Out
             FActScriptCall { onScript: condPool.drvCoolSeatSt = !condPool.drvCoolSeatSt }
        },
        FTrgTouchArea { touchArea:climate.passCoolSeat; touchAction: TA.Out
             FActScriptCall { onScript: condPool.passCoolSeatSt = !condPool.passCoolSeatSt }
        },
        FTrgTouchArea { touchArea:climate.passHeatSeat; touchAction: TA.Out
             FActScriptCall { onScript: condPool.passHeatSeatSt = !condPool.passHeatSeatSt }
        },
        FTrgTouchArea { touchArea:climate.heatWheel; touchAction: TA.Out
             FActScriptCall { onScript: condPool.heatWheelSt = !condPool.heatWheelSt }
        },
        FTrgTouchArea { touchArea:climate.heatWinSheild; touchAction: TA.Out
             FActScriptCall { onScript: condPool.heatWinSheildSt = !condPool.heatWinSheildSt }
        },
        FTrgTouchArea { touchArea:climate.defrost; touchAction: TA.Out
             FActScriptCall { onScript: condPool.defrostSt = !condPool.defrostSt }
        },
        FTrgTouchArea { touchArea:climate.pannel; touchAction: TA.Out
             FActScriptCall { onScript: condPool.pannelSt = !condPool.pannelSt }
        },
        FTrgTouchArea { touchArea:climate.floor; touchAction: TA.Out
             FActScriptCall { onScript: condPool.floorSt = !condPool.floorSt }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
