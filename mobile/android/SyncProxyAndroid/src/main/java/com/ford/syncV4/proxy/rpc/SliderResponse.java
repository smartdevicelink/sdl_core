package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;

public class SliderResponse extends RPCResponse {

    public SliderResponse() {
        super("Slider");
    }
    public SliderResponse(Hashtable hash) {
        super(hash);
    }
    public void setSliderPosition(Integer sliderPosition) {
    	if (sliderPosition != null) {
    		parameters.put(Names.sliderPosition, sliderPosition);
    	} else {
    		parameters.remove(Names.sliderPosition);
    	}
    }
    public Integer getSliderPosition() {
    	return (Integer) parameters.get(Names.sliderPosition);
    }
}