package com.ford.syncV4.streaming;

import android.util.Log;

import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.SessionType;

import java.io.IOException;
import java.io.InputStream;

public class H264Packetizer extends AbstractPacketizer implements Runnable{

	public final static String TAG = "H264Packetizer";

	private Thread t = null;

	public H264Packetizer(IStreamListener streamListener, InputStream is, byte rpcSessionID) throws IOException {
		super(streamListener, is, rpcSessionID);
	}

	public void start() throws IOException {
		if (t == null) {
			t = new Thread(this);
			t.start();
		}
	}

	public void stop() {
		try {
			is.close();
		} catch (IOException ignore) {}
		t.interrupt();
		t = null;
	}

	public void run() {
		int length;

		try {
			while (!Thread.interrupted()) {
				length = is.read(buffer);
				
				if (length >= 0) {
					ProtocolMessage pm = new ProtocolMessage();
					pm.setSessionID(_rpcSessionID);
					pm.setSessionType(SessionType.Mobile_Nav);
					pm.setFunctionID(0);
					pm.setCorrID(0);
					pm.setData(buffer, length);
					
			        _streamListener.sendH264(pm);
				}
			}
		} catch (IOException e) {
            Log.e("SyncProxyTester", e.toString());
		}
	}
}
