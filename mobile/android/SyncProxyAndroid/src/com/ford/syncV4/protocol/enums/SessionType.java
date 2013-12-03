package com.ford.syncV4.protocol.enums;

import com.ford.syncV4.util.ByteEnumer;

import java.util.Vector;


public class SessionType extends ByteEnumer {

	private static Vector theList = new Vector();
	public static Vector getList() { return theList; } 
	
	byte i = 0x00;
	
	protected SessionType(byte value, String name) {super(value, name);}
	public final static SessionType RPC = new SessionType((byte)0x07, "RPC");
	public final static SessionType Bulk_Data = new SessionType((byte)0xF, "Bulk_Data");
    public final static SessionType Mobile_Nav = new SessionType((byte)0xB, "Mobile_Nav");

	static {
		theList.addElement(RPC);
		theList.addElement(Bulk_Data);
        theList.addElement(Mobile_Nav);
	}
	
	public static SessionType valueOf(byte passedButton) {
		return (SessionType) get(theList, passedButton);
	}
	
	public static SessionType[] values() {
		return (SessionType[]) theList.toArray();
	}
}
