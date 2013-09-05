// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

FHardwareKey {
    width: 50
    height: 50

    property alias image : theImage.source

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

    FWdgtImage {
        id: theImage
        //anchors.fill: parent
        anchors.centerIn: parent
        //fillMode: Image.PreserveAspectFit
        smooth: true
    }


    FViewUseCaseSimple {
        condition: pressed === true
        PropertyChanges { target: s1; position: 1.0 }
        PropertyChanges { target: s2; position: 0.2}
        PropertyChanges { target: s3; position: 0.0 }
    }
}
