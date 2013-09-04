// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "Widgets"

FHardwareKey {
    width: 50
    height: 50

    property alias label : theText.text

    Rectangle {
        anchors.fill: parent
        border.color: "black"
        border.width: 1
        radius: 10
        smooth: true

        gradient: Gradient {
            GradientStop {
                id: s1
                color: "#FF646464"
                position: 0.0
            }
            GradientStop {
                id: s2
                color: "#FF484848"
                position: 0.8
            }
            GradientStop {
                id: s3
                color: "#FF323232"
                position: 1.0
            }
        }
    }

    CondText {
        id: theText
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter

    }


    FViewUseCaseSimple {
        condition: pressed === true
        PropertyChanges { target: s1; position: 1.0 }
        PropertyChanges { target: s2; position: 0.2}
        PropertyChanges { target: s3; position: 0.0 }
    }
}
