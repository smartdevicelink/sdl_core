package com.ford.syncV4.trace;

import java.util.Hashtable;
import java.util.Iterator;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.RPCStruct;

class OpenRPCMessage extends RPCMessage {
	private OpenRPCMessage() {super("");}
	public OpenRPCMessage(RPCMessage rpcm) {
		super(rpcm);
	} // end-method

	public OpenRPCMessage(RPCStruct rpcs) {
		super(rpcs);
	} // end-method

	public String msgDump() {
		StringBuilder pd = new StringBuilder();
		
		pd.append(this.getFunctionName() + " " + this.getMessageType());
		
		msgDump(pd);

		return pd.toString();
	} // end-method

	public void msgDump(StringBuilder pd) {
		pd.append("[");

		dumpParams(parameters, pd);
		
		pd.append("]");

		return;
	} // end-method

	private void dumpParams(Hashtable ht, StringBuilder pd) {
		Iterator keySet = ht.keySet().iterator();
		Object obj = null;
		String key = "";
		boolean isFirstParam = true;

		while (keySet.hasNext()) {
			key = (String)keySet.next();
			obj = ht.get(key);
			if (isFirstParam) {
				isFirstParam = false;
			} else {
				pd.append(", ");
			} // end-if

			dumpParamNode(key, obj, pd);

		} // end-while
	} // end-method
	
	private void dumpParamNode(String key, Object obj, StringBuilder pd) {

		if (obj instanceof Hashtable) {
			pd.append("[");
			dumpParams((Hashtable)obj, pd);
			pd.append("]");
		} else if (obj instanceof RPCStruct) {
			pd.append("[");
			OpenRPCMessage orpcm = new OpenRPCMessage((RPCStruct)obj);
			orpcm.msgDump(pd);
			pd.append("]");
		} else if (obj instanceof Vector) {
			pd.append("[");
			Vector vec = (Vector)obj;
			for (int idx=0;idx < vec.size();idx++) {
				if (idx > 0) {
					pd.append(", ");
				}
				dumpParamNode(key, vec.elementAt(idx), pd);
			} // end-for
			pd.append("]");
		} else {
			pd.append("\"" + key + "\" = \"" + obj.toString() + "\"");
		}
	} // end-method
} // end-class OpenRPCMessage
