package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;

public class VehicleType extends RPCStruct {

    public VehicleType() { }
    public VehicleType(Hashtable hash) {
        super(hash);
    }
    public String getMake() {
        return (String) store.get(Names.make);
    }
    public void setMake(String make) {
        if (make != null) {
            store.put(Names.make, make);
        } else {
        	store.remove(Names.make);
        }
    }
    public String getModel() {
        return (String) store.get(Names.model);
    }
    public void setModel(String model) {
        if (model != null) {
            store.put(Names.model, model);
        } else {
        	store.remove(Names.model);
        }
    }
    public String getModelYear() {
        return (String) store.get(Names.modelYear);
    }
    public void setModelYear(String modelYear) {
        if (modelYear != null) {
            store.put(Names.modelYear, modelYear);
        } else {
        	store.remove(Names.modelYear);
        }
    }
    public String getTrim() {
        return (String) store.get(Names.trim);
    }
    public void setTrim(String trim) {
        if (trim != null) {
            store.put(Names.trim, trim);
        } else {
        	store.remove(Names.trim);
        }
    }
}
