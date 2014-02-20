package com.ford.syncV4.proxy.rpc.enums;

import java.util.EnumSet;

public enum PredefinedLayout {
    DEFAULT("DEFAULT"),
    MEDIA("MEDIA"),
    NON_MEDIA("NON-MEDIA"),
    ONSCREEN_PRESETS("ONSCREEN_PRESETS"),
    NAV_FULLSCREEN_MAP("NAV_FULLSCREEN_MAP"),
    NAV_LIST("NAV_LIST"),
    NAV_KEYBOARD("NAV_KEYBOARD"),
    GRAPHIC_WITH_TEXT("GRAPHIC_WITH_TEXT"),
    TEXT_WITH_GRAPHIC("TEXT_WITH_GRAPHIC"),
    TILES_ONLY("TILES_ONLY"),
    TEXTBUTTONS_ONLY("TEXTBUTTONS_ONLY"),
    GRAPHIC_WITH_TILES("GRAPHIC_WITH_TILES"),
    TILES_WITH_GRAPHIC("TILES_WITH_GRAPHIC"),
    GRAPHIC_WITH_TEXT_AND_SOFTBUTTONS("GRAPHIC_WITH_TEXT_AND_SOFTBUTTONS"),
    TEXT_AND_SOFTBUTTONS_WITH_GRAPHIC("TEXT_AND_SOFTBUTTONS_WITH_GRAPHIC"),
    GRAPHIC_WITH_TEXTBUTTONS("GRAPHIC_WITH_TEXTBUTTONS"),
    TEXTBUTTONS_WITH_GRAPHIC("TEXTBUTTONS_WITH_GRAPHIC"),
    LARGE_GRAPHIC_WITH_SOFTBUTTONS("LARGE_GRAPHIC_WITH_SOFTBUTTONS"),
    DOUBLE_GRAPHIC_WITH_SOFTBUTTONS("DOUBLE_GRAPHIC_WITH_SOFTBUTTONS"),
    LARGE_GRAPHIC_ONLY("LARGE_GRAPHIC_ONLY");

    String internalName;

    private PredefinedLayout(String internalName) {
        this.internalName = internalName;
    }

    public static PredefinedLayout valueForString(String value) {
        for (PredefinedLayout anEnum : EnumSet.allOf(PredefinedLayout.class)) {
            if (anEnum.toString().equals(value)) {
                return anEnum;
            }
        }

        throw new IllegalArgumentException("Unknown value " + value);
    }

    public String toString() {
        return this.internalName;
    }
}
