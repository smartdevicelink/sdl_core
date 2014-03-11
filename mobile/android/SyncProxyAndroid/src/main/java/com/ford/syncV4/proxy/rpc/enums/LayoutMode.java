package com.ford.syncV4.proxy.rpc.enums;

public enum LayoutMode {
    ICON_ONLY,
    ICON_WITH_SEARCH,
    LIST_ONLY,
    LIST_WITH_SEARCH,
    KEYBOARD;

    public static LayoutMode valueForString(String value) {
        return valueOf(value);
    }
}
