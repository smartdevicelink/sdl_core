package com.ford.syncV4.proxy.rpc.enums;

public enum ImageFieldName {
    /**
     * The image field for SoftButton
     */
    softButtonImage,
    /**
     * The first image field for Choice
     */
    choiceImage,
    /**
     * The secondary image field for Choice
     */
    choiceSecondaryImage,
    /**
     * The image field for vrHelpItem
     */
    vrHelpItem,
    /**
     * The image field for Turn
     */
    turnIcon,
    /**
     * The image field for the menu icon in SetGlobalProperties
     */
    menuIcon,
    /**
     * The image field for AddCommand
     */
    cmdIcon,
    /**
     * The image field for the app icon (set by setAppIcon)
     */
    appIcon,
    /**
     * The image field for Show
     */
    graphic,
    /**
     * The primary image field for ShowConstantTBT
     */
    showConstantTBTIcon,
    /**
     * The secondary image field for ShowConstantTBT
     */
    showConstantTBTNextTurnIcon,
    /**
     * The optional image of a destination / location
     */
    locationImage;

    public static ImageFieldName valueForString(String value) {
        return valueOf(value);
    }
}