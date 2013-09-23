import QtQuick 2.0

Row {
    property int pages
    property int activePage: 0

    Repeater {
        model: pages
        Image {
            source: index === activePage ? "../res/white_ball.png" : "../res/blue_ball.png"
        }
    }
}
