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

        layout: NavInRouteLayout {
            id:map


        }

    }



    triggers: [
        FTrgTouchArea { touchArea:map.cancel; touchAction: TA.Out
            FActViewShow { view: "NavNoRoute" }
        },
        FTrgTouchArea { touchArea:map.mute; touchAction: TA.Out
            FActScriptCall { onScript: { map.muteSt = !map.muteSt} }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
