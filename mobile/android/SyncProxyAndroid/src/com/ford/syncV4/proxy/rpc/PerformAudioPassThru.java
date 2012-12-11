package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.AudioCaptureQuality;
import com.ford.syncV4.proxy.rpc.enums.AudioType;
import com.ford.syncV4.proxy.rpc.enums.SamplingRate;
import com.ford.syncV4.util.DebugTool;

public class PerformAudioPassThru extends RPCRequest {

    public PerformAudioPassThru() {
        super("PerformAudioPassThru");
    }
    public PerformAudioPassThru(Hashtable hash) {
        super(hash);
    }
    public void setInitialPrompt(Vector<TTSChunk> initialPrompt) {
    	if (initialPrompt != null) {
    		parameters.put(Names.initialPrompt, initialPrompt);
    	} else {
    		parameters.remove(Names.initialPrompt);
    	}
    }
    public Vector<TTSChunk> getInitialPrompt() {
    	if (parameters.get(Names.initialPrompt) instanceof Vector<?>) {
	        Vector<?> list = (Vector<?>)parameters.get(Names.initialPrompt);
	        if (list != null && list.size() > 0) {
	            Object obj = list.get(0);
	            if (obj instanceof TTSChunk) {
	                return (Vector<TTSChunk>) list;
	            } else if (obj instanceof Hashtable) {
	                Vector<TTSChunk> newList = new Vector<TTSChunk>();
	                for (Object hashObj : list) {
	                    newList.add(new TTSChunk((Hashtable)hashObj));
	                }
	                return newList;
	            }
	        }
    	}
        return null;
    }
    public void setAudioPassThruDisplayText1(String audioPassThruDisplayText1) {
    	if (audioPassThruDisplayText1 != null) {
    		parameters.put(Names.audioPassThruDisplayText1, audioPassThruDisplayText1);
    	} else {
    		parameters.remove(Names.audioPassThruDisplayText1);
    	}
    }
    public String getAudioPassThruDisplayText1() {
    	return (String) parameters.get(Names.audioPassThruDisplayText1);
    }
    public void setAudioPassThruDisplayText2(String audioPassThruDisplayText2) {
    	if (audioPassThruDisplayText2 != null) {
    		parameters.put(Names.audioPassThruDisplayText2, audioPassThruDisplayText2);
    	} else {
    		parameters.remove(Names.audioPassThruDisplayText2);
    	}
    }
    public String getAudioPassThruDisplayText2() {
    	return (String) parameters.get(Names.audioPassThruDisplayText2);
    }
    public void setSamplingRate(SamplingRate samplingRate) {
    	if (samplingRate != null) {
    		parameters.put(Names.samplingRate, samplingRate);
    	} else {
    		parameters.remove(Names.samplingRate);
    	}
    }
    public SamplingRate getSamplingRate() {
    	Object obj = parameters.get(Names.samplingRate);
    	if (obj instanceof SamplingRate) {
    		return (SamplingRate) obj;
    	} else if (obj instanceof String) {
    		SamplingRate theCode = null;
            try {
                theCode = SamplingRate.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.samplingRate, e);
            }
            return theCode;
    	}
        return null;
    }
    public void setMaxDuration(Integer maxDuration) {
    	if (maxDuration != null) {
    		parameters.put(Names.maxDuration, maxDuration);
    	} else {
    		parameters.remove(Names.maxDuration);
    	}
    }
    public int getMaxDuration() {
    	return (Integer) parameters.get(Names.maxDuration);
    }
    public void setBitsPerSample(AudioCaptureQuality audioQuality) {
    	if (audioQuality != null) {
    		parameters.put(Names.audioQuality, audioQuality);
    	} else {
    		parameters.remove(Names.audioQuality);
    	}
    }
    public AudioCaptureQuality getBitsPerSample() {
    	Object obj = parameters.get(Names.audioQuality);
    	if (obj instanceof AudioCaptureQuality) {
    		return (AudioCaptureQuality) obj;
    	} else if (obj instanceof String) {
    		AudioCaptureQuality theCode = null;
            try {
                theCode = AudioCaptureQuality.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.audioQuality, e);
            }
            return theCode;
    	}
        return null;
    }
    public void setAudioType(AudioType audioType) {
    	if (audioType != null) {
    		parameters.put(Names.audioType, audioType);
    	} else {
    		parameters.remove(Names.audioType);
    	}
    }
    public AudioType getAudioType() {
    	Object obj = parameters.get(Names.audioType);
    	if (obj instanceof AudioType) {
    		return (AudioType) obj;
    	} else if (obj instanceof String) {
    		AudioType theCode = null;
            try {
                theCode = AudioType.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.audioType, e);
            }
            return theCode;
    	}
        return null;
    }
}
