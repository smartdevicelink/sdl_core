// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

QtObject {
    id: _style

    Component.onCompleted: {
        FThemeProvider.themeChanged.connect(themeChanged);
    }

    // This property is used to assign a "style" filename to the style. This file loads the style data and updates the properties of the style
    property string styleFile: ""

    property bool themed: false

    onStyleFileChanged: {
        loadStyle(_style);
    }

    function themeChanged() {
        if (themed)
        {
            loadStyle(_style);
        }
    }

    // indicate, that the style has been updated to the attached widgets
    signal styleUpdated

    function loadStyle(style)
    {
        if(styleFile.length > 0) {
            console.log( "Loading style: " + styleFile + " (" + FThemeProvider.theme + ")")
            var xhr = new XMLHttpRequest();
            xhr.open("GET", FThemeProvider.resourcePath + FThemeProvider.getFileSubPath(styleFile + ".qmlstyle") + "/" + styleFile + ".qmlstyle");
            xhr.onreadystatechange = function() {
                if (xhr.readyState === XMLHttpRequest.DONE) {
                    try {
                        var a = JSON.parse(xhr.responseText);
                        for (var b in a) {
                            if (style.hasOwnProperty(b)) {
                                style[b] = a[b];
                            }
                        }
                        styleUpdated();
                    }
                    catch (ex) {
                        console.log("Style exception (" + styleFile + "): " + ex);
                    }
                }
            }
            xhr.send();
        }
    }
}
