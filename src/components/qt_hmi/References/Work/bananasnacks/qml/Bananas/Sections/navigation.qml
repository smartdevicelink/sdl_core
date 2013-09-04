import QtQuick 2.0
import "../Components"
import "../Layouts"

Rectangle {
    id: section
    anchors.fill: parent

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#3C7855" }
        GradientStop { position: 0.4; color: "#000000" }
    }

    Section {
        id: page
        source: "Navigation/map.qml"
    }

    states: [
        State {
            name: ''
            PropertyChanges { target: page; source: "Navigation/map.qml" }
        },
        State {
            name: 'menu'
            PropertyChanges { target: page; source: "Navigation/menu.qml" }
        },
        State {
            name: 'keyboard_search'
            PropertyChanges { target: page; source: "Navigation/keyboard_search.qml" }
        },
        State {
            name: 'favorites'
            PropertyChanges { target: page; source: "Navigation/favorites.qml" }
        },
        State {
            name: 'recent_destinations'
            PropertyChanges { target: page; source: "Navigation/recent_destinations.qml" }
        },
        State {
            name: 'poi'
            PropertyChanges { target: page; source: "Navigation/poi.qml" }
        },
        State {
            name: 'video_moving_map'
            PropertyChanges { target: page; source: "Navigation/video_moving_map.qml" }
        }
    ]
}
