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
        layout:  PhoneContactsLayout {
           id:l
           list_data:phoneItemData
           list_itemWidget: PhoneContactListItemWdgt {
           itemClicked: childClicked

           }
        }
    }

    FEvent {id: browseSelected}

    ListModel
    {

    id:phoneItemData
    ListElement { staticItem: "Aaron" ; staticNumber:"(614)555-2444";type:"Mobile"}
    ListElement { staticItem: "Adrian" ; staticNumber:"(740)245-3434";type:"Mobile" }
    ListElement { staticItem: "Albert" ; staticNumber:"(614)245-5584";type:"Mobile" }
    ListElement { staticItem: "Alice" ; staticNumber:"(313)878-3566";type:"Office"}
    ListElement { staticItem: "Alec" ; staticNumber:"(917)608-2418";type:"Mobile"}
    ListElement { staticItem: "Alexander" ; staticNumber:"(614)608-2400";type:"Home"}
    ListElement { staticItem: "Alexander" ; staticNumber:"(614)608-2400";type:"Mobile"}
    ListElement { staticItem: "Amber" ; staticNumber:"(614)608-2111";type:"Mobile"}
    ListElement { staticItem: "Amy" ; staticNumber:"(440)245-3434";type:"Mobile" }
    ListElement { staticItem: "Andy" ; staticNumber:"(313)245-3434";type:"Mobile" }
    ListElement { staticItem: "Antje" ; staticNumber:"(567)245-3434";type:"Home" }
    ListElement { staticItem: "Barry" ; staticNumber:"(313)555-2211";type:"Mobile" }
    ListElement { staticItem: "Bea" ; staticNumber:"(614)455-3434";type:"Mobile" }
    ListElement { staticItem: "Ben" ; staticNumber:"(614)882-1843";type:"Office" }
    ListElement { staticItem: "Blake" ; staticNumber:"(330)149-7778";type:"Mobile" }
    ListElement { staticItem: "Bob Mould" ; staticNumber:"(313)245-3434";type:"Mobile" }
    ListElement { staticItem: "Brian" ; staticNumber:"(614)332-7499";type:"Mobile" }
    ListElement { staticItem: "Calvin" ; staticNumber:"(614)442-9974";type:"Mobile" }
    ListElement { staticItem: "Carl" ; staticNumber:"(567)773-3434";type:"Home" }
    ListElement { staticItem: "Cat Power" ; staticNumber:"(313)608-3434";type:"Home"}
    ListElement { staticItem: "Cathy" ; staticNumber:"(313)444-456";type:"Home"}
    ListElement { staticItem: "Chris" ; staticNumber:"(734)997-6514";type:"Home" }
    ListElement { staticItem: "Cloud Nothings"; staticNumber:"(444)608-543";type:"Mobile"}
    ListElement { staticItem: "Cody" ; staticNumber:"(567)365-8413";type:"Home" }
    ListElement { staticItem: "Curtis" ; staticNumber:"(614)245-1547";type:"Office" }
    ListElement { staticItem: "Dakota" ; staticNumber:"(567)555-4444";type:"Home" }
    ListElement { staticItem: "Dale" ; staticNumber:"(567)723-1974";type:"Mobile" }
    ListElement { staticItem: "Dallas"; staticNumber:"(614)556-879";type:"Home"}
    ListElement { staticItem: "Damian" ; staticNumber:"(614)245-3434";type:"Office" }
    ListElement { staticItem: "Daniel"; staticNumber:"(614)556-879";type:"Home"}
    ListElement { staticItem: "Darina"; staticNumber:"(313)895-888";type:"Mobile"}
    ListElement { staticItem: "Dee "; staticNumber:"(313)432-3434";type:"Home"}
    ListElement { staticItem: "Edward" ; staticNumber:"(917)608-975";type:"Office"}
    ListElement { staticItem: "Ferdinand" ; staticNumber:"(917)879-795";type:"Mobile"}
    ListElement { staticItem: "Gomez" ; staticNumber:"(313)455-543";type:"Mobile"}
    ListElement { staticItem: "Harry Potter"; staticNumber:"(313)888-432";type:"Home"}
    ListElement { staticItem: "Ivan"; staticNumber:"(313)996-225";type:"Home"}
    ListElement { staticItem: "James" ; staticNumber:"(917)608-3434";type:"Mobile"}
    ListElement { staticItem: "Karen"; staticNumber:"(313)998-332";type:"Home"}
    ListElement { staticItem: "Luis Torres"; staticNumber:"(313)779-111";type:"Home"}
    ListElement { staticItem: "Mary"; staticNumber:"(313)234-098";type:"Mobile"}
    ListElement { staticItem: "Neil"; staticNumber:"(313)444-112";type:"Mobile"}
    ListElement { staticItem: "Oswald"; staticNumber:"(313)456-921";type:"Mobile"}
    ListElement { staticItem: "Oscar"; staticNumber:"(313)332-778";type:"Home"}
    ListElement { staticItem: "Patrice"; staticNumber:"(313)789-111";type:"Mobile"}
    ListElement { staticItem: "Queen Elizabeth"; staticNumber:"(313)897-666";type:"Mobile"}
    ListElement { staticItem: "Rose"; staticNumber:"(313)111-211";type:"Mobile"}
    ListElement { staticItem: "Susell"; staticNumber:"(313)345-122";type:"Home"}
    ListElement { staticItem: "Susell"; staticNumber:"(313)345-122";type:"Home"}
    ListElement { staticItem: "Thomas"; staticNumber:"(917)608-342";type:"Mobile"}
    ListElement { staticItem: "Titus Andronicus" ; staticNumber:"(917)608-321";type:"Office"}
    ListElement { staticItem: "Toretto" ; staticNumber:"(917)608-3434";type:"Home"}
    ListElement { staticItem: "Uhl" ; staticNumber:"(313)444-654";type:"Office"}
    ListElement { staticItem: "Zara" ; staticNumber:"(313)231-009";type:"Mobile"}

    }
    FEvent { id: childClicked }

    triggers:[
    FTrgEntry   {
            FActScriptCall{onScript: l.list.positionViewAtIndex(sortList.existChar(phoneItemData,dataPool.filterPhone),l.list.Beginning)}
                },
    FTrgTouchArea { touchArea:l.back; touchAction: TA.Out
            FActViewShow { view: "AlphaSortPCA" }
        },
    FTrgEvent { id: trig2; event: childClicked
            FActScriptCall { onScript:console.log(trig2.data) }
        }



    ]



}
