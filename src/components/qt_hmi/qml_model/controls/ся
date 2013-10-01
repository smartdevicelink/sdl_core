import QtQuick 2.0

Image {
    id: playBtn
    anchors.verticalCenter: parent.verticalCenter
    state: 'Play'
    property string sourceOnPressed: ""
    property string sourceOnReleased: ""

    MouseArea {
        anchors.fill: parent
        onPressed: {
            playBtn.source = playBtn.sourceOnPressed
        }
        onReleased: {
            playBtn.source = playBtn.sourceOnReleased
            playBtn.state == 'Play' ? playBtn.state = 'Pause' : playBtn.state = 'Play'
        }
        onClicked: {

        }
    }
    states: [
        State {
            name: "Play"
            PropertyChanges {
                target: playBtn
                source: "../res/buttons/player_play_btn.png"
                sourceOnPressed: "../res/buttons/player_play_pressed_btn.png"
                sourceOnReleased: "../res/buttons/player_pause_btn.png"
            }
        },

        State {
            name: "Pause"
            PropertyChanges {
                target: playBtn
                source: "../res/buttons/player_pause_btn.png"
                sourceOnPressed: "../res/buttons/player_pause_pressed_btn.png"
                sourceOnReleased: "../res/buttons/player_play_btn.png"
            }
        }
    ]
}
