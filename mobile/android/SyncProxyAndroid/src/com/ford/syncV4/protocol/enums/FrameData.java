package com.ford.syncV4.protocol.enums;

import java.util.Vector;

import com.ford.syncV4.util.ByteEnumer;

public class FrameData extends ByteEnumer {

	private static Vector theList = new Vector();
	public static Vector getList() { return theList; } 
	
	byte i = 0x00;
	
	protected FrameData(byte value, String name) {super(value, name);}
	public final static FrameData StartSession = new FrameData((byte)0x01, "StartSession");
	public final static FrameData StartSessionACK = new FrameData((byte)0x02, "StartSessionACK");
	public final static FrameData StartSessionNACK = new FrameData((byte)0x03, "StartSessionNACK");
	public final static FrameData EndSession = new FrameData((byte)0x04, "EndSession");
	
	public final static FrameData SingleFrame = new FrameData((byte)0x00, "SingleFrame");
	public final static FrameData FirstFrame = new FrameData((byte)0x00, "FirstFrame");
	public final static FrameData ConsecutiveFrame = new FrameData((byte)0x00, "ConsecutiveFrame");
	public final static byte LastFrame = (byte)0x00;
	
	static {
		theList.addElement(StartSession);
		theList.addElement(StartSessionACK);
		theList.addElement(StartSessionNACK);
		theList.addElement(EndSession);	
	}
	
	public static FrameData valueOf(String passedButton) {
		return (FrameData) get(theList, passedButton);
	}
	
	public static FrameData[] values() {
		return (FrameData[]) theList.toArray();
	}
}
