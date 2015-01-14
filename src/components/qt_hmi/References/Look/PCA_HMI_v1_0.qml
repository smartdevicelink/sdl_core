import QtQuick 2.0
import com.ford.hmiframework 1.0
import "Models"
import "Widgets"
import "Styles"

Rectangle {
    width: 1360
    height: 768
    color: "black"

    // global events
    FEvent { id: cornerClicked }
    FEvent { id: closeNormalScreens }

    //global timers
    FTimer {id: testTimer; interval: 5000; repeat: false}

    // global datapools
    FDataPool {
        id: dataPool

        property string version: "V1.0.0"
        property string currSource: "FM"
        property string currStation: "91.9"
        property string currStatText: "WUOM"
        property string currSong: "Fresh Air"
        property string currAlbumArt: "album artRazorFish.png"
        property int layout: 2 //AM/FM:0, AMhd:1, FMhd:2, Sirius:3, USB:4, BT:5
        property int currFMpreset: 2
        property int currAMpreset: 1
        property int currSATpreset: 3
        property string curStationDT: ""
        property int statusBarChange: 0
        property int curFanSpeed: 1
        property int drvTemp: 70
        property int passTemp: 70

        property bool mainPCAmenuShown: false


        property string fm1pre1: "101.1"
        property string fm1pre2: "97.1"
        property string fm1pre3: "94.7"
        property string fm1pre4: "91.7"
        property string fm1pre5: "101.9"
        property string fm1pre6: "88.7"

        property string fm1stat1: "WRIF"
        property string fm1stat2: "WXYT"
        property string fm1stat3: "WCSX"
        property string fm1stat4: "WUOM"
        property string fm1stat5: "WDET"
        property string fm1stat6: "CIMX"

        property string fm1song1: "Ace of Spades"
        property string fm1song2: "Sports Talk"
        property string fm1song3: "Born to Run"
        property string fm1song4: "Fresh Air"
        property string fm1song5: "The Craig Fahle Show"
        property string fm1song6: "State of Love and Trust"

        property string fm2pre1: "101.1"
        property string fm2pre2: "97.1"
        property string fm2pre3: "94.7"
        property string fm2pre4: "91.7"
        property string fm2pre5: "101.9"
        property string fm2pre6: "88.7"

        property string fm2stat1: "WRIF"
        property string fm2stat2: "WXYT"
        property string fm2stat3: "WCSX"
        property string fm2stat4: "WUOM"
        property string fm2stat5: "WDET"
        property string fm2stat6: "CIMX"

        property string fm2song1: "Ace of Spades"
        property string fm2song2: "Sports Talk"
        property string fm2song3: "Born to Run"
        property string fm2song4: "Fresh Air"
        property string fm2song5: "The Craig Fahle Show"
        property string fm2song6: "State of Love and Trust"

        property string fm3pre1: "101.1"
        property string fm3pre2: "97.1"
        property string fm3pre3: "94.7"
        property string fm3pre4: "91.7"
        property string fm3pre5: "101.9"
        property string fm3pre6: "88.7"

        property string fm3stat1: "WRIF"
        property string fm3stat2: "WXYT"
        property string fm3stat3: "WCSX"
        property string fm3stat4: "WUOM"
        property string fm3stat5: "WDET"
        property string fm3stat6: "CIMX"

        property string fm3song1: "Ace of Spades"
        property string fm3song2: "Sports Talk"
        property string fm3song3: "Born to Run"
        property string fm3song4: "Fresh Air"
        property string fm3song5: "The Craig Fahle Show"
        property string fm3song6: "State of Love and Trust"


        /*List Filtered Variables*/
        property string navEnrty:""
        property string filter: ""
        property variant uniqueChars: undefined
        property string navList1stItem:""
        property string filterPhone:""
    }

    // global conditions
    FConditionPool {
        id: condPool

        property bool amfmActiveSource: true
        property bool settingsOpen: false
        property bool dualBtnSt: true
        property bool drvHeatSeatSt: false
        property bool drvCoolSeatSt: false
        property bool passCoolSeatSt: false
        property bool passHeatSeatSt: false
        property bool heatWheelSt: false
        property bool heatWinSheildSt: true
        property bool defrostSt: true
        property bool pannelSt: false
        property bool floorSt: false

        property bool showMenu: true

    }


    Component.onCompleted: {
        FThemeProvider.resourcePath = Qt.resolvedUrl("Resources");
        FThemeProvider.theme = "Theme1";


    }
    property QtObject filtermodel: ListModel {
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

    QtObject{
        id:sortList

        //Check if char exist on Phone DataModel
        function existChar(model, character){
            //console.log(model.get(3).staticItem.charAt(0).toLowerCase())
            for (var i = 0; i < model.count; i++) {
                var u =model.get(i).staticItem.charAt(0).toLowerCase()
                if (character=== u){
                    console.log(i)
                    return i;
                }

            }

        }

        // Check whether given character exists in array
        function exist(array, character) {
            console.log(array[0]);
            for (var i = 0; i < array.length; i++) {

                if (array[i] == character) return true;
            }
            return false;
        }

        // Check whether given filter matches the string (e.g. "ce" would match "Cecilia")
        // The check is case-insensitive
        function checkFilter(filter, string) {
            if (filter.length > string.length) return false;

            var filterlower = filter.toLowerCase();
            var stringlower = string.toLowerCase();

            for (var i = 0; i < filter.length; i++) {
                if (filterlower[i] != stringlower[i])
                    return false;
            }

            return true;
        }

        // Updates the filtermodel based on the current filter
        function updateFilterModel(name) {
            if (dataPool.filter.length > 0) {
                for (var i = filtermodel.count-1; i >= 0; i--) {
                    if (!checkFilter(dataPool.filter, filtermodel.get(i).staticItem))
                        filtermodel.remove(i)
                }
            }

            updateUniqueChars()
        }

        // Updates the array of unique first letters in the current index of the model
        function updateUniqueChars() {

            if (filtermodel.get(0) === undefined)
            {
                dataPool.navList1stItem=""
                return;

            }

            var characters = new Array();

            // If filter model has at most 1 item in it, no point in going
            // any further with filtering
            dataPool.navList1stItem=filtermodel.get(0).staticItem
            if (filtermodel.count > 1) {

                for (var i = 0; i < filtermodel.count; i++) {
                    var name = filtermodel.get(i).staticItem;

                    if (!exist(characters, name[dataPool.filter.length])) {
                        characters.push(name[dataPool.filter.length])
                    }
                }

                characters.sort()


            }


            dataPool.uniqueChars = characters

        }


        // Fills the filtermodel with contents from the source list's model
        function fillFilterModel(fullModel) {
            if (fullModel === undefined) return;
            filtermodel.clear();
            for (var i = 0; i < fullModel.count; i++) {
                filtermodel.append(fullModel.get(i))
            }

        }

    }

    FDisplay {

       /* modelPath:Qt.resolvedUrl(".")
        id: display
        //activeVariant: "ClassicTop"
        //activeVariant: "ClassicBottom"
        //activeVariant: "Tangier"
        viewCacheSize:0

        //width: 800
        //height: 480
//        xPixel: 800
//        yPixel: 480
        anchors.fill: parent
//        anchors { left: parent.left; top: parent.top;}

        priorityLayerCount: 9
        //initialView: "AutocompleteList"
        initialView: "Off"*/



        modelPath:Qt.resolvedUrl(".")
        id: display
        //activeVariant: "ClassicTop"
        //activeVariant: "ClassicBottom"
        //activeVariant: "Tangier"
        viewCacheSize:0


        width: 800
        height: 480

        xPixel: 800
        yPixel: 480

        anchors {
            horizontalCenter: parent.horizontalCenter;
            verticalCenter: parent.verticalCenter;
            verticalCenterOffset: 55
        }

        priorityLayerCount: 9
        initialView: "Off"
        //initialView: "NavTilesView"
    }

    Item
    {
        id: fHardwareKeybezel1
        width: 800
        height: 375
        x:10
        y:500

        // function keys
        TextHardkey {
            id: hk_OnOff
            width: 100
            height: 50
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            label: "On/Off"
        }

        BaseText {
            id: info
            text: dataPool.version
            color: "black"
            anchors.top: hk_OnOff.top
            anchors.left: hk_OnOff.right
            anchors.leftMargin: 20
        }


        FHardwareKey {
                   id: buttonUp
                   hardwareControlID: "buttonUp"
                   //commName: "buttonBack"

                   x:0
                   y:0
                   width: 60
                   height:25


                   keyboardKey: Qt.Key_Up
                   visible: false
               }

        FHardwareKey {
                   id: buttonDown
                   hardwareControlID: "buttonDown"
                   //commName: "buttonBack"

                   x:0
                   y:0
                   width: 60
                   height:25


                   keyboardKey: Qt.Key_Down
                   visible: false
               }

    }

}
