// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"
import ".."

FStateView {
    priority: 2
    attachedViews: ["PCAstatusBar"]




    view: FView {
        id: v

        layout: AlphaSortLayout {
            id:home
            phonEntry: dataPool.filterPhone

        }


    }

    triggers: [

        FTrgEntry{
            FActScriptCall { onScript: dataPool.filterPhone= ""}
        },
        FTrgTouchArea { touchArea:home.a; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "a"}
             FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.b; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "b"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.c; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "c"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.d; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "d"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.e; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "e"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.f; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "f"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.g; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "g"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.h; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "h"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.i; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "i"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.j; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "j"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.k; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "k"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.l; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "l"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.m; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "m"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.n; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "n"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.o; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "o"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.p; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "p"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.q; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "q"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.r; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "r"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.s; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "s"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.t; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "t"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.u; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "u"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.v; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "v"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.w; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "w"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.xB; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "x"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.yB; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "y"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.zB; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "z"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.num; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.filterPhone= "a"}
            FActViewShow { view: "PhoneContactsListView" }
        },
        FTrgTouchArea { touchArea:home.close; touchAction: TA.Out

            FActViewShow { view: "PhonePCA" }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]

}
