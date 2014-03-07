package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class Slider extends RPCRequest {

    public Slider() {
        super("Slider");
    }
    public Slider(Hashtable hash) {
        super(hash);
    }
    public void setNumTicks(Integer numTicks) {
    	if (numTicks != null) {
    		parameters.put(Names.numTicks, numTicks);
    	} else {
    		parameters.remove(Names.numTicks);
    	}
    }
    public Integer getNumTicks() {
    	return (Integer) parameters.get(Names.numTicks);
    }
    public void setPosition(Integer position) {
    	if (position != null) {
    		parameters.put(Names.position, position);
    	} else {
    		parameters.remove(Names.position);
    	}
    }
    public Integer getPosition() {
    	return (Integer) parameters.get(Names.position);
    }
    public void setSliderHeader(String sliderHeader) {
    	if (sliderHeader != null) {
    		parameters.put(Names.sliderHeader, sliderHeader);
    	} else {
    		parameters.remove(Names.sliderHeader);
    	}
    }
    public String getSliderHeader() {
    	return (String) parameters.get(Names.sliderHeader);
    }
    public void setSliderFooter(Vector<String> sliderFooter) {
    	if (sliderFooter != null) {
    		parameters.put(Names.sliderFooter, sliderFooter);
    	} else {
    		parameters.remove(Names.sliderFooter);
    	}
    }
    public Vector<String> getSliderFooter() {
        if (parameters.get(Names.sliderFooter) instanceof Vector<?>) {
        	Vector<?> list = (Vector<?>)parameters.get(Names.sliderFooter);
        	if (list != null && list.size()>0) {
        		Object obj = list.get(0);
        		if (obj instanceof String) {
        			return (Vector<String>) list;
        		}
        	}
        }
    	return null;
    }
    public void setTimeout(Integer timeout) {
    	if (timeout != null) {
    		parameters.put(Names.timeout, timeout);
    	} else {
    		parameters.remove(Names.timeout);
    	}
    }
    public Integer getTimeout() {
    	return (Integer) parameters.get(Names.timeout);
    }
}
