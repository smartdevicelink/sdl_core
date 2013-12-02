import QtQuick 2.0
import "../../Components"
import "../../Layouts"

EmptyLayout {
    back_button: true

    Grid {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        GraphicalButton {
            text: "Home"
            image: "../../Assets/HomeIcon.png"
            onPress: {
                section.state = 'map';
            }
        }
        GraphicalButton {
            text: "Work"
            image: "../../Assets/WorkIcon.png"
            onPress: {
                section.state = 'map';
            }
        }
        GraphicalButton {
            text: "Recent Destination"
            image: "../../Assets/RecentDestinationsIcon.png"
            onPress: section.state = 'recent_destinations'
        }
        GraphicalButton {
            text: "Keyboard"
            image: "../../Assets/KeyboardIcon.png"
            onPress: section.state = 'keyboard_search'
        }
        GraphicalButton {
            text: "Favorites"
            image: "../../Assets/FavoritesIcon.png"
            onPress: section.state = 'favorites'
        }
        GraphicalButton {
            text: "Point of Interest"
            image: "../../Assets/PoiIcon.png"
            onPress: section.state = 'poi'
        }
        GraphicalButton {
            text: "Scout"
            image: "../../Assets/ScoutIcon.png"
        }
        GraphicalButton {
            text: "SYNC Services"
            image: "../../Assets/ServicesIcon.png"
            onPress: section.state = "video_moving_map"
        }
    }
}
