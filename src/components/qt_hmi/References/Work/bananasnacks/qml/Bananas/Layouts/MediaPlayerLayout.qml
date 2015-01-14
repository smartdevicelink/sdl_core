import QtQuick 2.0
import "../Components"

MediaLayout {

    Row {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100
        anchors.left: parent.left
        anchors.leftMargin: 20
        spacing: 10

        ImageButton {
            icon: "../Assets/Rewind.png"
            onPress: MediaPlayer.previous()
        }

        ImageButton {
            icon: "../Assets/Pause.png"
            visible: MediaPlayer.playing
            onPress: MediaPlayer.pause()
        }
        ImageButton {
            icon: "../Assets/Play.png"
            visible: ! MediaPlayer.playing
            onPress: MediaPlayer.play()
        }
        ImageButton {
            icon: "../Assets/Forward.png"
            onPress: MediaPlayer.next()
        }

    }
}
