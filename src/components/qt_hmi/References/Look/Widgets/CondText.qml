// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

BaseText {
    FontLoader {
        id: testFont; source: "../Fonts/HelveticaNeueLTCom-LtCn.ttf"
    }

    font.family: testFont.name
    font.pixelSize: 30

    height: 51
}
