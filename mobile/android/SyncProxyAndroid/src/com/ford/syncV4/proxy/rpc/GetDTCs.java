package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class GetDTCs extends RPCRequest {

    public GetDTCs() {
        super("GetDTCs");
    }
    public GetDTCs(Hashtable hash) {
        super(hash);
    }
    public void setEcuName(Integer ecuName) {
    	if (ecuName != null) {
    		parameters.put(Names.ecuName, ecuName);
    	} else {
    		parameters.remove(Names.ecuName);
    	}
    }
    public Integer getEcuName() {
    	return (Integer) parameters.get(Names.ecuName);
    }

    public void setDTCMask(Integer dtcMask) {
        if (dtcMask != null) {
            parameters.put(Names.dtcMask, dtcMask);
        } else {
            parameters.remove(Names.dtcMask);
        }
    }
    public Integer getDTCMask() {
        return (Integer) parameters.get(Names.dtcMask);
    }
}
