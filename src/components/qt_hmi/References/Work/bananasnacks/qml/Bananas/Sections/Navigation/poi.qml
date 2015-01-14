import QtQuick 2.0
import com.ford.navcore 1.0
import "../../Components"
import "../../Layouts"

EmptyLayout {
    anchors.fill: parent
    back_button: true
    id: poi_search

    Component.onCompleted: state = "search_type"

    states: [
        State {
            name: 'search_type'
            PropertyChanges {
                target: search_type
                visible: true
            }
        },
        State {
            name: 'categories'
            PropertyChanges {
                target: category_select
                visible: true
            }
        },
        State {
            name: 'select_state'
            PropertyChanges {
                target: text_entry
                visible: true
            }
        },
        State {
            name: 'results'
            PropertyChanges {
                target: results
                visible: true
            }
        },
        State {
            name: 'result'
            PropertyChanges {
                target: result
                visible: true
            }
        }
    ]

    NavController {
        id: navController
    }

    Grid {
        id: search_type
        property string type
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        visible: false

        GraphicalButton {
            text: "Near me"
            image: "../../Assets/HomeIcon.png"
            onPress: {
                search_type.type = "nearby"
                poi_search.state = "categories"
            }
        }

        GraphicalButton {
            text: "Along route"
            image: "../../Assets/HomeIcon.png"
            onPress: {
                search_type.type = "route"
                poi_search.state = "categories"
            }
        }

        GraphicalButton {
            text: "In city"
            image: "../../Assets/HomeIcon.png"
            onPress: {
                search_type.type = "city"
                poi_search.state = "select_state"
            }
        }
    }

    ListView {
        id: category_select
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height - 120
        width: parent.width
        spacing: 10
        clip: true

        GraphicalButton {
            text: "Search by name"
            image: "../../Assets/HomeIcon.png"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
        }

        visible: false
        model: NavPoiCategoryListModel { id: poi_categories }

        delegate: Button {
            property int category_id: id
            text: name
            width: 400
            anchors.horizontalCenter: parent.horizontalCenter
            onPress: {
                navController.poiSearch(search_type.type, category_id, "USA", state_suggestions.getSelected(), city_suggestions.getSelected());
                poi_search.state = "results"
            }
        }
    }

    ListView {
        id: results
        visible: false
        anchors.topMargin: 80
        anchors.top: parent.top
        anchors.bottomMargin: 80
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 400
        height: parent.height - 120
        spacing: 10
        clip: true

        model: NavPoiSuggestionListModel {
            id: poi_suggestions
        }

        delegate: Button {
            text: name
            width: parent.width
            onPress: {
                results.currentIndex = index
                poi_search.state = "result"
            }
        }
    }

    Item {
        anchors.fill: parent
        anchors.topMargin: 80
        anchors.bottomMargin: 80
        id: result
        visible: false

        H1 {
            id: poi_name
            anchors.horizontalCenter: parent.horizontalCenter
            text: (results.currentItem ? results.currentItem.text : "")
        }

        Button {
            text: "Begin navigation"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: poi_name.bottom
            onPress: {
                poi_suggestions.navigateToResult(results.currentIndex);
                section.state = 'map'
            }
        }
    }

    TextEntry {
        id: text_entry
        anchors.topMargin: 80
        input_border.color: "#3C7855"

        states: [
            State {
                name: "city_select"
                PropertyChanges {
                    target: text_entry
                    autocompleteModel: city_suggestions
                }
            }

        ]

        NavStateSuggestionListModel { id: state_suggestions }
        NavCitySuggestionListModel { id: city_suggestions; stateListModel: state_suggestions }
        autocompleteModel: state_suggestions
        visible: false

        onSelected: {
            if (text_entry.state === '') {
                text = "";
                text_entry.state = 'city_select';
            }
            else {
                poi_search.state = 'categories'
            }
        }
    }
}
