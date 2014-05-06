package com.ford.syncV4.protocol.enums;

import com.ford.syncV4.util.ByteEnumer;

import java.util.Vector;

public class ServiceType extends ByteEnumer {

<<<<<<< HEAD
    public static final byte HEARTBEAT_SERVICE_ID = (byte) 0;
    public static final byte RPC_SERVICE_ID = (byte) 0x07;
    public static final byte BULK_DATA_SERVICE_ID = (byte) 0xF;
    public static final byte MOBILE_NAV_SERVICE_ID = (byte) 0xB;
    public static final byte AUDIO_SERVICE_ID = (byte) 0xA;
    public static final byte SECURE_SERVICE_ID = (byte) 0x10;

    public static final String HEARTBEAT_SERVICE_NAME = "Heartbeat_Service";
    public static final String RPC_NAME = "RPC";
    public static final String BULK_DATA_NAME = "Bulk_Data";
    public static final String MOBILE_NAV_NAME = "Mobile_Nav";
    public static final String AUDIO_SERVICE_NAME = "Audio_Service";
    

    private static Vector sVector = new Vector();
=======
    private static Vector theList = new Vector();

    public static Vector getList() {
        return theList;
    }
>>>>>>> develop

    protected ServiceType(byte value, String name) {
        super(value, name);
    }
<<<<<<< HEAD
    //TODO rename to control service
    public final static ServiceType Heartbeat = new ServiceType(HEARTBEAT_SERVICE_ID,
            HEARTBEAT_SERVICE_NAME);
    public final static ServiceType RPC = new ServiceType(RPC_SERVICE_ID,
            RPC_NAME);
    public final static ServiceType Bulk_Data = new ServiceType(BULK_DATA_SERVICE_ID,
            BULK_DATA_NAME);
    public final static ServiceType Mobile_Nav = new ServiceType(MOBILE_NAV_SERVICE_ID,
            MOBILE_NAV_NAME);
    public final static ServiceType Audio_Service = new ServiceType(AUDIO_SERVICE_ID,
            AUDIO_SERVICE_NAME);


    static {
        sVector.addElement(RPC);
        sVector.addElement(Bulk_Data);
        sVector.addElement(Mobile_Nav);
        sVector.addElement(Audio_Service);
        sVector.addElement(Heartbeat);
    }

    public static ServiceType valueOf(byte passedButton) {
        return (ServiceType) get(sVector, passedButton);
    }

    public static ServiceType[] values() {
        return (ServiceType[]) sVector.toArray();
    }

    public static boolean containsService(ServiceType serviceType) {
        return sVector.contains(serviceType);
=======

    public final static ServiceType Heartbeat = new ServiceType((byte) 0, "Heartbeat_Service");
    public final static ServiceType RPC = new ServiceType((byte) 0x07, "RPC");
    public final static ServiceType Bulk_Data = new ServiceType((byte) 0xF, "Bulk_Data");
    public final static ServiceType Mobile_Nav = new ServiceType((byte) 0xB, "Mobile_Nav");
    public final static ServiceType Audio_Service = new ServiceType((byte) 10, "Audio_Service");

    static {
        theList.addElement(RPC);
        theList.addElement(Bulk_Data);
        theList.addElement(Mobile_Nav);
        theList.addElement(Audio_Service);
        theList.addElement(Heartbeat);
    }

    public static ServiceType valueOf(byte passedButton) {
        return (ServiceType) get(theList, passedButton);
    }

    public static ServiceType[] values() {
        return (ServiceType[]) theList.toArray();
>>>>>>> develop
    }
}