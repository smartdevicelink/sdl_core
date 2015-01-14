import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: climateLayout
    width: 800
    height: 480

    property alias dualBtn: dualBtn.touchBtn
    property alias drvHeatSeat: drvHeatSeat.touchBtn
    property alias drvCoolSeat: drvCoolSeat.touchBtn
    property alias passCoolSeat: passCoolSeat.touchBtn
    property alias passHeatSeat: passHeatSeat.touchBtn
    property alias heatWheel: heatWheel.touchBtn
    property alias heatWinSheild: heatWinSheild.touchBtn
    property alias defrost: defrost.touchBtn
    property alias pannel: pannel.touchBtn
    property alias floor: floor.touchBtn

    property alias dualBtnSt: dualBtn.selected
    property alias drvHeatSeatSt: drvHeatSeat.selected
    property alias drvCoolSeatSt: drvCoolSeat.selected
    property alias passCoolSeatSt: passCoolSeat.selected
    property alias passHeatSeatSt: passHeatSeat.selected
    property alias heatWheelSt: heatWheel.selected
    property alias heatWinSheildSt: heatWinSheild.selected
    property alias defrostSt: defrost.selected
    property alias pannelSt: pannel.selected
    property alias floorSt: floor.selected



    TouchButtonClimatePCA{
        id: drvHeatSeat
        x:105
        y:178
        bitmap: "climate/heated seat icon.png"
        bitmap2: "climate/heated seat iconBlack.png"
    }
    TouchButtonClimatePCA{
        id: drvCoolSeat
        x:105
        y:273
        bitmap: "climate/A_C icon.png"
        bitmap2: "climate/A_C iconBlack.png"
    }
    TouchButtonClimatePCA{
        id: heatWheel
        x:200
        y:178
        bitmap: "climate/wheel icon.png"
        bitmap2: "climate/wheel iconBlack.png"
    }
    TouchButtonClimatePCA{
        id: heatWinSheild
        x:200
        y:273
        bitmap: "climate/headed dashBlue.png"
        bitmap2: "climate/headed dash.png"
    }

    TouchButtonClimatePCA{
        id: defrost
        x:360
        y:130
        bitmap: "climate/defrost iconBlue.png"
        bitmap2: "climate/defrost icon.png"
    }

    TouchButtonClimatePCA{
        id: pannel
        x:360
        y:226
        bitmap: "climate/face icon.png"
        bitmap2: "climate/face iconBlack.png"
    }

    TouchButtonClimatePCA{
        id: floor
        x:360
        y:322
        bitmap: "climate/feet icon.png"
        bitmap2: "climate/feet iconBlack.png"
    }


    TouchButtonClimatePCA{
        id: dualBtn
        y:225
        x:528
        text: "Dual"
    }

    TouchButtonClimatePCA{
        id: passHeatSeat
        x:615
        y:178
        bitmap: "climate/heated seat icon.png"
        bitmap2: "climate/heated seat iconBlack.png"
    }
    TouchButtonClimatePCA{
        id: passCoolSeat
        x:615
        y:273
        bitmap: "climate/A_C icon.png"
        bitmap2: "climate/A_C iconBlack.png"
    }


}
