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

        layout: NavKeyboardLayout {
            id:nav
            entry:dataPool.navEnrty
            list_data:sItemData
            list_itemWidget: VerySimpleListItemWidget {
                vis: false

            }
        }

    }
    ListModel
    {

    id:sItemData
    ListElement { staticItem: "Five Guys" }
    ListElement { staticItem: "Archers Of Loaf" }
    ListElement { staticItem: "Bob Mould" }
    ListElement { staticItem: "Cat Power" }
    ListElement { staticItem: "Cloud Nothings"}
    ListElement { staticItem: "Dentist"}
    ListElement { staticItem: "Dirty Frank's" }
    ListElement { staticItem: "Grouplove" }
    ListElement { staticItem: "Hudson Bell"}
    ListElement { staticItem: "Japandroids" }
    ListElement { staticItem: "Macklemore & Ryan Lewis"}
    ListElement { staticItem: "The National"}
    ListElement { staticItem: "Titus Andronicus" }
    ListElement { staticItem: "Tito" }
    ListElement { staticItem: "Tamal" }
    ListElement { staticItem: "Taco" }

    }

    triggers: [

        FTrgTouchArea { touchArea:nav.listOpen; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= ""}
            FActViewShow { view: "NavInRoute" }
        },
        FTrgTouchArea { touchArea:nav.a; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"a"}
        },
        FTrgTouchArea { touchArea:nav.b; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"b"}
        },
        FTrgTouchArea { touchArea:nav.c; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"c"}
        },
        FTrgTouchArea { touchArea:nav.d; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"d"}
        },
        FTrgTouchArea { touchArea:nav.e; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"e"}
        },
        FTrgTouchArea { touchArea:nav.f; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"f"}
        },
        FTrgTouchArea { touchArea:nav.g; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"g"}
        },
        FTrgTouchArea { touchArea:nav.h; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"h"}
        },
        FTrgTouchArea { touchArea:nav.i; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"i"}
        },
        FTrgTouchArea { touchArea:nav.j; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"j"}
        },
        FTrgTouchArea { touchArea:nav.k; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"k"}
        },
        FTrgTouchArea { touchArea:nav.l; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"l"}
        },
        FTrgTouchArea { touchArea:nav.m; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"m"}
        },
        FTrgTouchArea { touchArea:nav.n; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"n"}
        },
        FTrgTouchArea { touchArea:nav.o; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"o"}
        },
        FTrgTouchArea { touchArea:nav.p; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"p"}
        },
        FTrgTouchArea { touchArea:nav.q; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"q"}
        },
        FTrgTouchArea { touchArea:nav.r; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"r"}
        },
        FTrgTouchArea { touchArea:nav.s; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"s"}
        },
        FTrgTouchArea { touchArea:nav.t; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"t"}
        },
        FTrgTouchArea { touchArea:nav.u; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"u"}
        },
        FTrgTouchArea { touchArea:nav.v; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"v"}
        },
        FTrgTouchArea { touchArea:nav.w; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"w"}
        },
        FTrgTouchArea { touchArea:nav.xB; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"x"}
        },
        FTrgTouchArea { touchArea:nav.yB; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"y"}
        },
        FTrgTouchArea { touchArea:nav.zB; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+"z"}
        },
        FTrgTouchArea { touchArea:nav.num; touchAction: TA.Out
        },
        FTrgTouchArea { touchArea:nav.clr; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= ""}
        },
        FTrgTouchArea { touchArea:nav.space; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty+" "}
        },
        FTrgTouchArea { touchArea:nav.del; touchAction: TA.Out
            FActScriptCall { onScript: dataPool.navEnrty= dataPool.navEnrty.substring(0,dataPool.navEnrty.length-1)}
        },
        FTrgTouchArea { touchArea:nav.sym; touchAction: TA.Out
        },
        FTrgTouchArea { touchArea:nav.go; touchAction: TA.Out
            FActViewShow { view: "NavInRoute" }
        },
        FTrgTouchArea { touchArea:nav.close; touchAction: TA.Out
            FActViewShow { view: "NavNoRoute" }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }

    ]
}
