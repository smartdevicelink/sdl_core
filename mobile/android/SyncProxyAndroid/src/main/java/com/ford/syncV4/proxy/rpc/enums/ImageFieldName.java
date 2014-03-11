package com.ford.syncV4.proxy.rpc.enums;

public enum ImageFieldName {
    softButtonImage,
    choiceImage,
    choiceSecondaryImage,
    vrHelpItem,
    turnIcon,
    menuIcon,
    cmdIcon,
    appIcon,
    graphic,
    showConstantTBTIcon,
    showConstantTBTNextTurnIcon;

    public static ImageFieldName valueForString(String value) {
        return valueOf(value);
    }
}
