// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 8


    view: FView {
        id: v

        layout: PCAstatusBarLayout {
            id:statusBar

        }

    }



    triggers: [



        FTrgTouchArea { touchArea:statusBar.menuTouch; touchAction: TA.Out
            FActViewShow { view: "GridMenuPCA" }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
