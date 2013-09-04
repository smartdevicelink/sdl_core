// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 2
    //attachedViews: ["Chrome"]



    view: FView {
        id: v

        layout: FMLayoutPCA {
            id:home
            sourceBtn.text: "Line In"
            sourceBtn.textWidth: 90
        }


    }



    triggers: [



        FTrgTouchArea { touchArea:home.sourceBtn.touchBtn; touchAction: TA.Out
            FActViewShow { view: "SourceChangePCA" }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
