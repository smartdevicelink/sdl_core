package com.ford.syncV4.proxy.rpc.enums;

public enum VehicleDataNotificationStatus {
    NOT_SUPPORTED("VDNS_NOT_SUPPORTED"),
    NORMAL("VDNS_NORMAL"),
    ACTIVE("VDNS_ACTIVE"),
    NOT_USED("VDNS_NOT_USED");

    final String internalName;

    private VehicleDataNotificationStatus(String internalName) {
        this.internalName = internalName;
    }

    public static VehicleDataNotificationStatus valueForString(String value) {
        for (VehicleDataNotificationStatus status : VehicleDataNotificationStatus
                .values()) {
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
