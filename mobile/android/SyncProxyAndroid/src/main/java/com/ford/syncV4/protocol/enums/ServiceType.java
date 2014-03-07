package com.ford.syncV4.protocol.enums;

import com.ford.syncV4.util.ByteEnumer;

import java.util.Vector;

public class ServiceType extends ByteEnumer {

    public static final byte HEARTBEAT_SERVICE_ID = (byte) 0;
    public static final byte RPC_SERVICE_ID = (byte) 0x07;
    public static final byte BULK_DATA_SERVICE_ID = (byte) 0xF;
    public static final byte MOBILE_NAV_SERVICE_ID = (byte) 0xB;
    public static final byte AUDIO_SERVICE_ID = (byte) 0xA;
    public static final byte SECURE_SERVICE_ID = (byte) 0x10;

    private static Vector sVector = new Vector();

    protected ServiceType(byte value, String name) {
        super(value, name);
    }

    public final static ServiceType Heartbeat = new ServiceType(HEARTBEAT_SERVICE_ID,
            "Heartbeat_Service");
    public final static ServiceType RPC = new ServiceType(RPC_SERVICE_ID,
            "RPC");
    public final static ServiceType Bulk_Data = new ServiceType(BULK_DATA_SERVICE_ID,
            "Bulk_Data");
    public final static ServiceType Mobile_Nav = new ServiceType(MOBILE_NAV_SERVICE_ID,
            "Mobile_Nav");
    public final static ServiceType Audio_Service = new ServiceType(AUDIO_SERVICE_ID,
            "Audio_Service");
    public final static ServiceType Secure_Service = new ServiceType(SECURE_SERVICE_ID,
            "Secure_Service");

    static {
        sVector.addElement(RPC);
        sVector.addElement(Bulk_Data);
        sVector.addElement(Mobile_Nav);
        sVector.addElement(Audio_Service);
        sVector.addElement(Heartbeat);
        sVector.addElement(Secure_Service);
    }

    public static ServiceType valueOf(byte passedButton) {
        return (ServiceType) get(sVector, passedButton);
    }

    public static ServiceType[] values() {
        return (ServiceType[]) sVector.toArray();
    }

    public static boolean containsService(ServiceType serviceType) {
        return sVector.contains(serviceType);
    }
}