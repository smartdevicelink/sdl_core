package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.AudioType;
import com.ford.syncV4.proxy.rpc.enums.BitsPerSample;
import com.ford.syncV4.proxy.rpc.enums.SamplingRate;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;

public class AudioPassThruCapabilities extends RPCStruct {
    public AudioPassThruCapabilities() {
    }

    public AudioPassThruCapabilities(Hashtable hash) {
        super(hash);
    }

    public SamplingRate getSamplingRate() {
        Object obj = store.get(Names.samplingRate);
        if (obj instanceof SamplingRate) {
            return (SamplingRate) obj;
        } else if (obj instanceof String) {
            try {
                return SamplingRate.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.samplingRate, e);
            }
        }
        return null;
    }

    public void setSamplingRate(SamplingRate samplingRate) {
        if (samplingRate != null) {
            store.put(Names.samplingRate, samplingRate);
        } else {
            store.remove(Names.samplingRate);
        }
    }

    public BitsPerSample getBitsPerSample() {
        Object obj = store.get(Names.bitsPerSample);
        if (obj instanceof BitsPerSample) {
            return (BitsPerSample) obj;
        } else if (obj instanceof String) {
            try {
                return BitsPerSample.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.bitsPerSample, e);
            }
        }
        return null;
    }

    public void setBitsPerSample(BitsPerSample bitsPerSample) {
        if (bitsPerSample != null) {
            store.put(Names.bitsPerSample, bitsPerSample);
        } else {
            store.remove(Names.bitsPerSample);
        }
    }

    public AudioType getAudioType() {
        Object obj = store.get(Names.audioType);
        if (obj instanceof AudioType) {
            return (AudioType) obj;
        } else if (obj instanceof String) {
            try {
                return AudioType.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.audioType, e);
            }
        }
        return null;
    }

    public void setAudioType(AudioType audioType) {
        if (audioType != null) {
            store.put(Names.audioType, audioType);
        } else {
            store.remove(Names.audioType);
        }
    }
}
