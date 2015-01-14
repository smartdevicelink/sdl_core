// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

QtObject {
    // this property can be FStyle, but only if the instance is created in place and not assigned. Otherwise we need to use variant
    //property FStyle style
    property variant style
    property variant styleTarget

    onStyleChanged: {
        if(style !== undefined && style !== null) {
            style.styleUpdated.connect(applyStyle);
            applyStyle();
        }
    }

    onStyleTargetChanged: {
        applyStyle();
    }

    function applyStyle()
    {
        if(style !== undefined && style !== null && styleTarget !== undefined && styleTarget !== null) {
            setStyledProperties(style, styleTarget)
        }
    }

    function setStyledProperties(style, target)
    {
        for(var prop in style) {
            // first filter unwanted properties
            if(prop !== "objectName" && prop.slice(-7) !== "Changed" ) {
                // process only properties owned by style & target
                if (target.hasOwnProperty(prop)) {
                    // assign the value
                    target[prop] = style[prop];
                }
            }
        }
    }
}
