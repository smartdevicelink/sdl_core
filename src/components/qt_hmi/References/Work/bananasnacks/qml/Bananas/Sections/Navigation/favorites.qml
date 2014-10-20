import QtQuick 2.0
import com.ford.navcore 1.0
import "../../Components"
import "../../Layouts"

Item {
    BackButton { onPress: section.state = 'menu' }
    NavController {
        id: navController
    }

    Flickable {
        height: 300
        width: 400
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        clip: true
        contentWidth: content.width
        contentHeight: content.height

        Column {
            id: content
            width: parent.width
            spacing: 10

            Button {
                text: "Barry Church's home"
                width: parent.width
                onPress: {
                    navController.beginNavigation("USA", "MI", "Davison", "Victoria Sta", "3465");
                    section.state = 'map';
                }
            }
            Button {
                text: "Subway"
                width: parent.width
                onPress: {
                    navController.beginNavigation("USA", "MI", "Dearborn Heights", "Pelham St", "3929");
                    section.state = 'map';
                }
            }
            Button {
                text: "Ford World Headquarters"
                width: parent.width
                onPress: {
                    navController.beginNavigation("USA", "MI", "Dearborn", "American Rd", "1");
                    section.state = 'map';
                }
            }
            Button {
                text: "Qdoba"
                width: parent.width
                onPress: {
                    navController.beginNavigation("USA", "MI", "Allen Park", "Fairlane Dr", "48101");
                    section.state = 'map';
                }
            }
            Button {
                text: "Levagood Park"
                width: parent.width
                onPress: {
                    navController.beginNavigation("USA", "MI", "Dearborn", "Denwood St", "1100");
                    section.state = 'map';
                }
            }

        }
    }
}

