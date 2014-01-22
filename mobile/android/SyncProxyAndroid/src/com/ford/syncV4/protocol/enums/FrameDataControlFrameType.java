package com.ford.syncV4.protocol.enums;

import com.ford.syncV4.util.ByteEnumer;

import java.util.Vector;

public class FrameDataControlFrameType extends ByteEnumer {
    private static Vector<FrameDataControlFrameType> theList =
            new Vector<FrameDataControlFrameType>();

    public static Vector getList() { return theList; }

	private byte _i = 0x00;

	protected FrameDataControlFrameType(byte value, String name) {super(value, name);}
	public final static FrameDataControlFrameType Heartbeat = new FrameDataControlFrameType((byte)0x0, "Heartbeat");
	public final static FrameDataControlFrameType StartService = new FrameDataControlFrameType((byte)0x01, "StartService");
	public final static FrameDataControlFrameType StartServiceACK = new FrameDataControlFrameType((byte)0x02, "StartServiceACK");
	public final static FrameDataControlFrameType StartServiceNACK = new FrameDataControlFrameType((byte)0x03, "StartServiceNACK");
	public final static FrameDataControlFrameType EndService = new FrameDataControlFrameType((byte)0x04, "EndService");
    public final static FrameDataControlFrameType EndServiceACK = new FrameDataControlFrameType((byte)0x05, "EndServiceACK");
    public final static FrameDataControlFrameType EndServiceNACK = new FrameDataControlFrameType((byte)0x06, "EndServiceNACK");
    public final static FrameDataControlFrameType MobileNaviACK = new FrameDataControlFrameType((byte)0xFE, "MobileNaviACK");
    public final static FrameDataControlFrameType HeartbeatACK = new FrameDataControlFrameType((byte)0xFF, "HeartbeatACK");

	static {
		theList.addElement(Heartbeat);
		theList.addElement(StartService);
		theList.addElement(StartServiceACK);
		theList.addElement(StartServiceNACK);
		theList.addElement(EndService);
        theList.addElement(EndServiceACK);
        theList.addElement(EndServiceNACK);
        theList.addElement(MobileNaviACK);
        theList.addElement(HeartbeatACK);
    }

	public static FrameDataControlFrameType valueOf(String name) {
		return (FrameDataControlFrameType) get(theList, name);
	} // end-method

	public static FrameDataControlFrameType[] values() {
		return theList.toArray(new FrameDataControlFrameType[theList.size()]);
	} // end-method
} // end-class