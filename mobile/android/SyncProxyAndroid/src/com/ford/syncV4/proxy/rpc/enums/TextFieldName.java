package com.ford.syncV4.proxy.rpc.enums;

public enum TextFieldName {
    mainField1,
    mainField2,
    mainField3,
    mainField4,
    statusBar,
    mediaClock,
    mediaTrack,
    alertText1,
    alertText2,
    alertText3,
    scrollableMessageBody,
    initialInteractionText,
    navigationText1,
    navigationText2,
    ETA,
    totalDistance,
    audioPassThruDisplayText1,
    audioPassThruDisplayText2,
    sliderHeader,
    sliderFooter;

    public static TextFieldName valueForString(String value) {
        return valueOf(value);
    }
}
