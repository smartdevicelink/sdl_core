package com.ford.syncV4.protocol.enums;

import java.util.Vector;

import com.ford.syncV4.util.ByteEnumer;

public class FrameType extends ByteEnumer {

	private static Vector theList = new Vector();
	public static Vector getList() { return theList; } 
	
	byte i = 0x00;
	
	protected FrameType(byte value, String name) {super(value, name);}
	public final static FrameType Control = new FrameType((byte)0x00, "Control");
	public final static FrameType Single = new FrameType((byte)0x01, "Single");
	public final static FrameType First = new FrameType((byte)0x02, "First");
	public final static FrameType Consecutive = new FrameType((byte)0x03, "Consecutive");
	
	static {
		theList.addElement(Control);
		theList.addElement(Single);
		theList.addElement(First);
		theList.addElement(Consecutive);
	}
	
	public static FrameType valueOf(byte passed) {
		return (FrameType) get(theList, passed);
	}
	
	public static FrameType[] values() {
		return (FrameType[]) theList.toArray();
	}
}
