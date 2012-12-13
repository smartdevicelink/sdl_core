package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.HMILevel;
import com.ford.syncV4.proxy.rpc.enums.MediaClockFormat;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataResultCode;
import com.ford.syncV4.util.DebugTool;

public class GetDTCsResponse extends RPCResponse {

    public GetDTCsResponse() {
        super("GetDTCs");
    }
    public GetDTCsResponse(Hashtable hash) {
        super(hash);
    }
    public void setDtcList(DTC dtcList) {
    	if (dtcList != null) {
    		parameters.put(Names.dtcList, dtcList);
    	} else {
    		parameters.remove(Names.dtcList);
    	}
    }
    public DTC getDtcList() {
    	Object obj = parameters.get(Names.dtcList);
        if (obj instanceof DTC) {
            return (DTC) obj;
        } else {
        	DTC theCode = null;
            try {
                theCode = new DTC((Hashtable) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.dtcList, e);
            }
            return theCode;
        }
    }
}