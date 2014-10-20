// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

Image {
    property string bitmap: ""
    property bool themed: false

    fillMode: Image.PreserveAspectFit
    asynchronous: true

    onBitmapChanged: {
        applyThemedImage();
    }

    function applyThemedImage() {
        if (bitmap !== "") {
            if (themed) {
                source = FThemeProvider.resourcePath + "/" + FThemeProvider.getFileSubPath(bitmap) + bitmap;
            }
            else {
                source = FThemeProvider.resourcePath + "/" + bitmap;
            }
        }
        else {
            source = "";
        }
    }

    Component.onCompleted: {
        if (themed)
        {
            FThemeProvider.themeChanged.connect(applyThemedImage);
        }
    }

    Component.onDestruction: {
        if (themed)
        {
            FThemeProvider.themeChanged.disconnect(applyThemedImage);
        }
    }
}
