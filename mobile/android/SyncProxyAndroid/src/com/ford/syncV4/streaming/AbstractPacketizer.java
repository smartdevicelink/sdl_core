package com.ford.syncV4.streaming;

import java.io.IOException;
import java.io.InputStream;

abstract public class AbstractPacketizer {

	protected IStreamListener _streamListener = null;

	protected byte _rpcSessionID = 0;
	
	protected InputStream is = null;
	protected byte[] buffer = new byte[1000];
	protected boolean upts = false;
	
	//protected long ts = 0, intervalBetweenReports = 5000, delta = 0;
	protected long intervalBetweenReports = 5000, delta = 0;

	public AbstractPacketizer(IStreamListener streamListener, InputStream is, byte rpcSessionID) throws IOException {
        this._streamListener = streamListener;
		this.is = is;
		_rpcSessionID = rpcSessionID;
	}

	public abstract  void start() throws IOException;

	public abstract void stop();

	public static String printBuffer(byte[] buffer, int start,int end) {
		String str = "";
		for (int i=start;i<end;i++) str+=","+Integer.toHexString(buffer[i]&0xFF);
		return str;
	}
}
