package com.ford.syncV4.protocol.enums;

import com.ford.syncV4.util.ByteEnumer;

import java.util.Vector;

public class ServiceType extends ByteEnumer {

    private static Vector sVector = new Vector();

    protected ServiceType(byte value, String name) {
        super(value, name);
    }

    public final static ServiceType Heartbeat = new ServiceType((byte) 0, "Heartbeat_Service");
    public final static ServiceType RPC = new ServiceType((byte) 0x07, "RPC");
    public final static ServiceType Bulk_Data = new ServiceType((byte) 0xF, "Bulk_Data");
    public final static ServiceType Mobile_Nav = new ServiceType((byte) 0xB, "Mobile_Nav");
    public final static ServiceType Audio_Service = new ServiceType((byte) 10, "Audio_Service");
    // TODO : Replace byte value when it will be considered
    public final static ServiceType Secure_Service = new ServiceType((byte) 0x11, "Secure_Service");

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