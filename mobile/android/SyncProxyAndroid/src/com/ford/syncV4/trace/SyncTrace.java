package com.ford.syncV4.trace;

import java.sql.Timestamp;

import android.bluetooth.BluetoothDevice;
import android.hardware.usb.UsbAccessory;
import android.os.Build;
import android.os.Debug;
import android.os.Process;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.enums.FrameDataControlFrameType;
import com.ford.syncV4.protocol.enums.FrameType;
import com.ford.syncV4.protocol.enums.SessionType;
import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.trace.enums.DetailLevel;
import com.ford.syncV4.trace.enums.InterfaceActivityDirection;
import com.ford.syncV4.trace.enums.Mod;
import com.ford.syncV4.transport.SiphonServer;
import com.ford.syncV4.util.BitConverter;
import com.ford.syncV4.util.DebugTool;
import com.ford.syncV4.util.NativeLogTool;

/* This class handles the global TraceSettings as requested by the users either through the combination of the following
   1. System defaults
   2. Application XML config
   3. Programmatic requests from application itself

   It is manifested in the <applink>...</applink> tags
 */

public class SyncTrace {
	private static final String SYNC_LIB_TRACE_KEY = "42baba60-eb57-11df-98cf-0800200c9a66";
	
	static boolean canWriteLogs = false;
		
	public static final String SYSTEM_LOG_TAG = "SyncTrace";
	
	private static long baseTics  = java.lang.System.currentTimeMillis();
	private final static String KeyStr = SYNC_LIB_TRACE_KEY;
	private static boolean acceptAPITraceAdjustments = true;

	protected static ISTListener m_appTraceListener = null;

	///
	///  The PUBLIC interface to SyncTrace starts here
	///


	public static void setAcceptAPITraceAdjustments(Boolean APITraceAdjustmentsAccepted) {
		if (APITraceAdjustmentsAccepted != null) {
			acceptAPITraceAdjustments = APITraceAdjustmentsAccepted;
		}
	}
	
	public static boolean getAcceptAPITraceAdjustments() {
		return acceptAPITraceAdjustments;
	}
	
	public static void setAppTraceListener(ISTListener listener) {
		m_appTraceListener = listener;
	} // end-method

	public static void setTracingEnable(Boolean enable) {
		if (enable != null) {
			canWriteLogs = enable;
		}
	} // end-method

	public static void setAppTraceLevel(DetailLevel dt) {
		if ( dt != null && acceptAPITraceAdjustments)
			DiagLevel.setLevel(Mod.app, dt);
	} // end-method

	public static void setProxyTraceLevel(DetailLevel dt) {
		if (dt != null && acceptAPITraceAdjustments)
			DiagLevel.setLevel(Mod.proxy, dt);
	} // end-method

	public static void setRpcTraceLevel(DetailLevel dt) {
		if (dt != null && acceptAPITraceAdjustments)
			DiagLevel.setLevel(Mod.rpc, dt);
	} // end-method

	public static void setMarshallingTraceLevel(DetailLevel dt) {
		if (dt != null && acceptAPITraceAdjustments)
			DiagLevel.setLevel(Mod.mar, dt);
	} // end-method

	public static void setProtocolTraceLevel(DetailLevel dt) {
		if (dt != null && acceptAPITraceAdjustments)
			DiagLevel.setLevel(Mod.proto, dt);
	} // end-method

	public static void setTransportTraceLevel(DetailLevel dt) {
		if (dt != null && acceptAPITraceAdjustments)
				DiagLevel.setLevel(Mod.tran, dt);
	} // end-method

	private static String encodeTraceMessage(long timestamp, Mod module, InterfaceActivityDirection msgDirection, String msgBodyXml) {
		StringBuilder sb = new StringBuilder("<msg><dms>");
		sb.append(timestamp);
		sb.append("</dms><pid>");
		sb.append(Process.myPid());
		sb.append("</pid><tid>");
		sb.append(Thread.currentThread().getId());
		sb.append("</tid><mod>");
		sb.append(module.toString());
		sb.append("</mod>");
		if (msgDirection != InterfaceActivityDirection.None) {
			sb.append("<dir>");
			sb.append(interfaceActivityDirectionToString(msgDirection));
			sb.append("</dir>");
		} // end-if
		sb.append(msgBodyXml);
		sb.append("</msg>");

		return sb.toString();
	} // end-method

	private static String interfaceActivityDirectionToString(InterfaceActivityDirection iaDirection) {
		String str = "";
		switch (iaDirection) {
			case Receive:
				str = "rx";
				break;
			case Transmit:
				str = "tx";
				break;
		} // end-switch
		return str;
	} // end-method

	static String B64EncodeForXML(String data) {
		return Mime.base64Encode(data);
		// Base64 only available in 2.2, when AppLink base is 2.2 use: return Base64.encodeToString(data.getBytes(), Base64.DEFAULT);
	} // end-method
	
	public static void logProxyEvent(String eventText, String token) {
		if (DiagLevel.getLevel(Mod.proxy) == DetailLevel.OFF || !token.equals(KeyStr)) {
			return;
		} // end-if

		String msg = SyncTrace.B64EncodeForXML(eventText);
		String xml = SyncTrace.encodeTraceMessage(SyncTrace.getBaseTicsDelta(), Mod.proxy, InterfaceActivityDirection.None, "<d>" + msg + "</d>");
		writeXmlTraceMessage(xml);
	} // end-method

	public static void logAppEvent(String eventText) {
		if (DiagLevel.getLevel(Mod.app) == DetailLevel.OFF) {
			return;
		} // end-if

		long timestamp = SyncTrace.getBaseTicsDelta();
		String msg = SyncTrace.B64EncodeForXML(eventText);
		String xml = SyncTrace.encodeTraceMessage(timestamp, Mod.app, InterfaceActivityDirection.None, "<d>" + msg + "</d>");
		writeXmlTraceMessage(xml);
	} // end-method

	public static void logRPCEvent(InterfaceActivityDirection msgDirection, RPCMessage rpcMsg, String token) {
		DetailLevel dl = DiagLevel.getLevel(Mod.rpc);
		if (dl == DetailLevel.OFF || !token.equals(KeyStr)) {
			return;
		} // end-if

		long timestamp = SyncTrace.getBaseTicsDelta();
		String xml = SyncTrace.encodeTraceMessage(timestamp, Mod.rpc, msgDirection, rpc2Xml(dl, rpcMsg));
		writeXmlTraceMessage(xml);
	} // end-method

	private static String rpc2Xml(DetailLevel dl, RPCMessage rpcMsg) {
		StringBuilder rpcAsXml = new StringBuilder();
		rpcAsXml.append("<op>");
		rpcAsXml.append(rpcMsg.getFunctionName());
		rpcAsXml.append("</op>");
		boolean hasCorrelationID = false;
		Integer correlationID = -1;
		if (rpcMsg instanceof RPCRequest) {
			hasCorrelationID = true;
			correlationID = ((RPCRequest)rpcMsg).getCorrelationID();
		} else if (rpcMsg instanceof RPCResponse) {
			hasCorrelationID = true;
			correlationID = ((RPCResponse)rpcMsg).getCorrelationID();
		} // end-if
		if (hasCorrelationID) {
			rpcAsXml.append("<cid>");
			rpcAsXml.append(correlationID);
			rpcAsXml.append("</cid>");
		} // end-if
		rpcAsXml.append("<type>");
		rpcAsXml.append(rpcMsg.getMessageType());
		rpcAsXml.append("</type>");
		//rpcAsXml.append(newline);

		if (dl == DetailLevel.VERBOSE) {
			OpenRPCMessage orpcmsg = new OpenRPCMessage(rpcMsg);
			String rpcParamList = orpcmsg.msgDump();
			String msg = SyncTrace.B64EncodeForXML(rpcParamList);
			rpcAsXml.append("<d>");
			rpcAsXml.append(msg);
			rpcAsXml.append("</d>");
		} // end-if
		return rpcAsXml.toString();
	} // end-method

	public static void logMarshallingEvent(InterfaceActivityDirection msgDirection, byte[] marshalledMessage, String token) {
		DetailLevel dl = DiagLevel.getLevel(Mod.mar);
		if (dl == DetailLevel.OFF || !token.equals(KeyStr)) {
			return;
		} // end-fif

		long timestamp = SyncTrace.getBaseTicsDelta();
		StringBuilder msg = new StringBuilder();
		msg.append("<sz>");
		msg.append(marshalledMessage.length);
		msg.append("</sz>");
		if (dl == DetailLevel.VERBOSE) {
			msg.append("<d>");
			msg.append(Mime.base64Encode(marshalledMessage));
			// Base64 only available in 2.2, when AppLink base is 2.2 use: msg.append(Base64.encodeToString(marshalledMessage, Base64.DEFAULT));
			msg.append("</d>");
		} // end-if
		String xml = SyncTrace.encodeTraceMessage(timestamp, Mod.mar, msgDirection, msg.toString());
		writeXmlTraceMessage(xml);
	} // end-method

	public static void logProtocolEvent(InterfaceActivityDirection frameDirection, ProtocolFrameHeader frameHeader, byte[] frameData, int frameDataOffset, int frameDataLength, String token) {
		DetailLevel dl = DiagLevel.getLevel(Mod.proto);
		if (dl == DetailLevel.OFF || !token.equals(KeyStr)) {
			return;
		} // end-if

		StringBuffer protoMsg = new StringBuffer();
		protoMsg.append("<frame>");
		protoMsg.append(SyncTrace.getProtocolFrameHeaderInfo(frameHeader, frameData));
		if (dl == DetailLevel.VERBOSE) {
			if (frameData != null && frameDataLength > 0) {
				protoMsg.append("<d>");
				String bytesInfo = "";
				bytesInfo = Mime.base64Encode(frameData, frameDataOffset, frameDataLength);
				// Base64 only available in 2.2, when AppLink base is 2.2 use: bytesInfo = Base64.encodeToString(frameData, frameDataOffset, frameDataLength, Base64.DEFAULT);
				protoMsg.append(bytesInfo);
				protoMsg.append("</d>");
			} // end-if
		} // end-if
		protoMsg.append("</frame>");
		String xml = SyncTrace.encodeTraceMessage(SyncTrace.getBaseTicsDelta(), Mod.proto, frameDirection, protoMsg.toString());
		writeXmlTraceMessage(xml);
	} // end-method

	private static String getProtocolFrameType(FrameType f) {
		if (f == FrameType.Control)
			return "Control";
		else if (f == FrameType.Consecutive)
			return "Consecutive";
		else if (f == FrameType.First)
			return "First";
		else if (f == FrameType.Single)
			return "Single";

		return "Unknown";
	} // end-method

	private static String getProtocolSessionType(SessionType serviceType) {
		String s;
		if (serviceType == SessionType.RPC )
			s = "rpc";
		else if (serviceType == SessionType.Bulk_Data)
			s = "bulk";
		else
			s = "Unknown";
		return s;
	} // end-method

	private static String getProtocolFrameHeaderInfo(ProtocolFrameHeader hdr, byte[] buf) {
		StringBuilder sb = new StringBuilder();
		sb.append("<hdr>");
		sb.append("<ver>");
		sb.append(hdr.getVersion());
		sb.append("</ver><cmp>");
		sb.append(hdr.isCompressed());
		sb.append("</cmp><ft>");
		sb.append(getProtocolFrameType(hdr.getFrameType()));
		sb.append("</ft><st>");
		sb.append(getProtocolSessionType(hdr.getSessionType()));
		sb.append("</st><sid>");
		sb.append(hdr.getSessionID());
		sb.append("</sid><sz>");
		sb.append(hdr.getDataSize());
		sb.append("</sz>");

		int frameData = hdr.getFrameData();
		if (hdr.getFrameType() == FrameType.Control) {
			sb.append("<ca>");
			if (frameData == FrameDataControlFrameType.StartSession.getValue()) 
				sb.append("StartSession");
			else if (frameData == FrameDataControlFrameType.StartSessionACK.getValue())
				sb.append("StartSessionACK");
			else if (frameData == FrameDataControlFrameType.StartSessionNACK.getValue())
				sb.append("StartSessionNACK");
			else if (frameData == FrameDataControlFrameType.EndSession.getValue())
				sb.append("EndSession");
			sb.append("</ca>");
		} else if (hdr.getFrameType() == FrameType.Consecutive ) {
			sb.append("<fsn>");
			if (frameData == 0 )
				sb.append("lastFrame");
			else
				sb.append(String.format("%02X",frameData)); 
			sb.append("</fsn>");
		} else if (hdr.getFrameType() == FrameType.First ) {
			int totalSize = BitConverter.intFromByteArray(buf, 0);			
			int numFrames = BitConverter.intFromByteArray(buf, 4);
			sb.append("<total>" + totalSize + "</total><numframes>" + numFrames + "</numframes>");
		} else if (hdr.getFrameType() == FrameType.Single ) {
			sb.append("<single/>");
		}

		sb.append("</hdr>");

		return sb.toString();
	} // end-method

	public static String getBTDeviceInfo(BluetoothDevice btDevice) {
		StringBuilder sb = new StringBuilder();
		sb.append("<btp>");
		String btdn = btDevice.getName();
		sb.append("<btn>");
		sb.append(SyncTrace.B64EncodeForXML(btdn));
		sb.append("</btn>");
		sb.append("<bta>" + btDevice.getAddress() + "</bta>");
		sb.append("<bts>" + btDevice.getBondState() + "</bts>");
		sb.append("</btp>");
		return sb.toString();
	} // end-method

    public static String getUSBAccessoryInfo(UsbAccessory accessory) {
        // TODO: split info into different tags?
        StringBuilder sb = new StringBuilder();
        sb.append("<usba>");
        sb.append(SyncTrace.B64EncodeForXML(accessory.toString()));
        sb.append("</usba>");
        return sb.toString();
    }

	public static void logTransportEvent(String preamble, String transportSpecificInfoXml, InterfaceActivityDirection msgDirection, byte buf[], int byteLength, String token) {
		logTransportEvent(preamble, transportSpecificInfoXml, msgDirection, buf, 0, byteLength, token);
	} // end-method

	private static void checkB64(String x, byte[] buf, int offset, int byteLength) {
		if ((x.length() % 4) != 0) {
			NativeLogTool.logWarning(SyncTrace.SYSTEM_LOG_TAG, "b64 string length (" + x.length() + ") isn't multiple of 4: buf.length=" + buf.length + ", offset=" + offset + ", len=" + byteLength);
		} // end-if
	} // end-method

	public static void logTransportEvent(String preamble, String transportSpecificInfoXml, InterfaceActivityDirection msgDirection, byte buf[], int offset, int byteLength, String token) {
		if (DiagLevel.getLevel(Mod.tran) == DetailLevel.OFF || !token.equals(KeyStr)) {
			return;
		} // end-if

		StringBuilder msg = new StringBuilder();
		if (transportSpecificInfoXml != null && transportSpecificInfoXml.length() > 0) {
			msg.append(transportSpecificInfoXml);
		} // end-if
		if (preamble != null && preamble.length() > 0) {
			msg.append("<desc>");
			msg.append(preamble);
			msg.append("</desc>");
		} // end-if
		if (buf != null) {
			msg.append("<sz>");
			msg.append(byteLength);
			msg.append("</sz>");
			DetailLevel dl = DiagLevel.getLevel(Mod.tran);
			if (dl == DetailLevel.VERBOSE) {
				if (buf != null && byteLength > 0) {
					msg.append("<d>");
					String bytesInfo = Mime.base64Encode(buf, offset, byteLength);
					checkB64(bytesInfo, buf, offset, byteLength);
					msg.append(bytesInfo);
					msg.append("</d>");
				} // end-if
			} // end-if
		} // end-if
		String xml = SyncTrace.encodeTraceMessage(SyncTrace.getBaseTicsDelta(), Mod.tran, msgDirection, msg.toString());
		writeXmlTraceMessage(xml);
	} // end-method

	// Package-scoped
	static long getBaseTicsDelta() {
		return java.lang.System.currentTimeMillis() - getBaseTics();
	}

	// Package-scoped
	static long getBaseTics() {
		return baseTics;
	} // end-method

	public static Boolean writeMessageToSiphonServer(String info) {
		return SiphonServer.sendFormattedTraceMessage(info);
	}

	private static void writeXmlTraceMessage(String msg) {
		try {			
			// Attempt to write formatted message to the Siphon
			if (false == writeMessageToSiphonServer(msg)) {
				// If writing to the Siphon fails, write to the native log
				NativeLogTool.logInfo(SyncTrace.SYSTEM_LOG_TAG, msg);
			}
			
			ISTListener localTraceListener = m_appTraceListener;

			if (localTraceListener != null) {
				try {
					localTraceListener.logXmlMsg(msg, KeyStr);
				} catch (Exception ex) {
					DebugTool.logError("Failure calling ISTListener: " + ex.toString(), ex);
				} // end-catch
			} // end-if
		} catch (Exception ex) {
			NativeLogTool.logError(SyncTrace.SYSTEM_LOG_TAG, "Failure writing XML trace message: " + ex.toString());
		}
	} // end-method
	
	// Package-scoped
	public static String getLogHeader(String dumpReason, int seqNo) {
		final String Sep = "-";
		StringBuilder write = new StringBuilder("<?xml version=\"1.0\"?>" + "<logs>");
		write.append("<info>");
		StringBuilder infoBlock = new StringBuilder();
		String hostInfo = Build.BRAND + Sep + Build.MANUFACTURER + Sep + Build.MODEL + "(" + Build.HOST + ")";
		infoBlock.append("<host>" + SyncTrace.B64EncodeForXML(hostInfo) + "</host>");
		String osv = Build.VERSION.RELEASE + " (" + Build.VERSION.CODENAME + ")";
		infoBlock.append("<osv>" + SyncTrace.B64EncodeForXML(osv) + "</osv>");
		infoBlock.append(TraceDeviceInfo.getTelephonyHeader());

		long heapSize = Debug.getNativeHeapFreeSize() / 1024;
		long heapAllocated = Debug.getNativeHeapAllocatedSize() / 1024;
		infoBlock.append("<mem><hf>" + heapSize + "KB</hf><ha>" + heapAllocated + "KB</ha></mem>");
		infoBlock.append("<np>" + Runtime.getRuntime().availableProcessors() + "</np>");
		infoBlock.append("<pid>" + Process.myPid() + "</pid>");
		infoBlock.append("<tid>" + Thread.currentThread().getId() + "</tid>");

		// String dateStamp = (String)
		// DateFormat.format("yy-MM-dd hh:mm:ss SSS", new Timestamp(baseTics));
		Timestamp stamp = new Timestamp(SyncTrace.getBaseTics());
		String GMTtime = stamp.toGMTString().substring(0, 19);
		long fracSec = stamp.getNanos() / 1000000; // divide by a million
		String fracSecStr = String.format("%03d", fracSec);
		infoBlock.append("<utc>" + GMTtime + "." + fracSecStr + "</utc>");

		infoBlock.append(TraceDeviceInfo.getLogHeaderBluetoothPairs());
		infoBlock.append(getApplinkTraceRoot(dumpReason, seqNo));

		write.append(infoBlock);

		write.append("</info>" + "<msgs>");
		return write.toString();
	} // end-method
	
	private static String getApplinkTraceRoot(String dumpReason, int seqNo) {
		StringBuilder write = new StringBuilder("<applinktraceroot>" + "<sequencenum>" + seqNo
				+ "</sequencenum>" + "<dumpreason>" + dumpReason
				+ "</dumpreason><tracelevel>");

		write.append("<tran>" + DiagLevel.getLevel(Mod.tran) + "</tran>");
		write.append("<proto>" + DiagLevel.getLevel(Mod.proto) + "</proto>");
		write.append("<mar>" + DiagLevel.getLevel(Mod.mar) + "</mar>");
		write.append("<rpc>" + DiagLevel.getLevel(Mod.rpc) + "</rpc>");
		write.append("<proxy>" + DiagLevel.getLevel(Mod.proxy) + "</proxy>");
		write.append("<app>" + DiagLevel.getLevel(Mod.app) + "</app>");

		write.append("</tracelevel>");
		write.append("</applinktraceroot>");
		return write.toString();
	} // end-method
} // end-class