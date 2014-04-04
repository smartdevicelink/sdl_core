package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.logger.Logger;

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
                Logger.e("Failed to parse " + getClass().getSimpleName() + "." + Names.dtcList, e);
            }
            return theCode;
        }
    }
}