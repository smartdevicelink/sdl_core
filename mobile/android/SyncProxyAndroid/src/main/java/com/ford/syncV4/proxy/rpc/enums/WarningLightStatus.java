package com.ford.syncV4.proxy.rpc.enums;

public enum WarningLightStatus {
    OFF("WLS_OFF"),
    ON("WLS_ON"),
    FLASH("WLS_FLASH"),
    NOT_USED("WLS_NOT_USED");

    final String internalName;

    private WarningLightStatus(String internalName) {
        this.internalName = internalName;
    }

    public static WarningLightStatus valueForString(String value) {
        for (WarningLightStatus status : WarningLightStatus.values()) {
            if (status.toString().equals(value)) {
                return status;
            }
        }

        throw new IllegalArgumentException("Unknown value " + value);
    }

    @Override
    public String toString() {
        return internalName;
    }
}
