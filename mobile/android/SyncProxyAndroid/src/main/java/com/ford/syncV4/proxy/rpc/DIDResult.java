package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataResultCode;
import com.ford.syncV4.util.logger.Logger;

import java.util.Hashtable;

public class DIDResult extends RPCStruct {
    public DIDResult() {
    }

    public DIDResult(Hashtable hash) {
        super(hash);
    }

    public VehicleDataResultCode getResultCode() {
        Object obj = store.get(Names.resultCode);
        if (obj instanceof VehicleDataResultCode) {
            return (VehicleDataResultCode) obj;
        } else if (obj instanceof String) {
            try {
                return VehicleDataResultCode.valueForString((String) obj);
            } catch (Exception e) {
                Logger.e(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.resultCode, e
                );
            }
        }
        return null;
    }

    public void setResultCode(VehicleDataResultCode resultCode) {
        if (resultCode != null) {
            store.put(Names.resultCode, resultCode);
        } else {
            store.remove(Names.resultCode);
        }
    }

    public Integer getDidLocation() {
        final Object o = store.get(Names.didLocation);
        if (o instanceof Integer) {
            return (Integer) o;
        }
        return null;
    }

    public void setDidLocation(Integer didLocation) {
        if (didLocation != null) {
            store.put(Names.didLocation, didLocation);
        } else {
            store.remove(Names.didLocation);
        }
    }

    public String getData() {
        final Object o = store.get(Names.data);
        if (o instanceof String) {
            return (String) o;
        }
        return null;
    }

    public void setData(String data) {
        if (data != null) {
            store.put(Names.data, data);
        } else {
            store.remove(Names.data);
        }
    }
}