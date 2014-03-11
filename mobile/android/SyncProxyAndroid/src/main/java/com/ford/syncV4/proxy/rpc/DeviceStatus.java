package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.DeviceLevelStatus;
import com.ford.syncV4.proxy.rpc.enums.PrimaryAudioSource;
import com.ford.syncV4.util.DebugTool;

public class DeviceStatus extends RPCStruct {

    public DeviceStatus() {}
    public DeviceStatus(Hashtable hash) {
        super(hash);
    }
    public void setVoiceRecOn(Boolean voiceRecOn) {
        if (voiceRecOn != null) {
        	store.put(Names.voiceRecOn, voiceRecOn);
        } else {
        	store.remove(Names.voiceRecOn);
        }
    }
    public Boolean getVoiceRecOn() {
        return (Boolean) store.get(Names.voiceRecOn);
    }
    public void setBtIconOn(Boolean btIconOn) {
        if (btIconOn != null) {
        	store.put(Names.btIconOn, btIconOn);
        } else {
        	store.remove(Names.btIconOn);
        }
    }
    public Boolean getBtIconOn() {
        return (Boolean) store.get(Names.btIconOn);
    }
    public void setCallActive(Boolean callActive) {
        if (callActive != null) {
        	store.put(Names.callActive, callActive);
        } else {
        	store.remove(Names.callActive);
        }
    }
    public Boolean getCallActive() {
        return (Boolean) store.get(Names.callActive);
    }
    public void setPhoneRoaming(Boolean phoneRoaming) {
        if (phoneRoaming != null) {
        	store.put(Names.phoneRoaming, phoneRoaming);
        } else {
        	store.remove(Names.phoneRoaming);
        }
    }
    public Boolean getPhoneRoaming() {
        return (Boolean) store.get(Names.phoneRoaming);
    }
    public void setTextMsgAvailable(Boolean textMsgAvailable) {
        if (textMsgAvailable != null) {
        	store.put(Names.textMsgAvailable, textMsgAvailable);
        } else {
        	store.remove(Names.textMsgAvailable);
        }
    }
    public Boolean getTextMsgAvailable() {
        return (Boolean) store.get(Names.textMsgAvailable);
    }
    public void setBattLevelStatus(DeviceLevelStatus battLevelStatus) {
        if (battLevelStatus != null) {
        	store.put(Names.battLevelStatus, battLevelStatus);
        } else {
        	store.remove(Names.battLevelStatus);
        }
    }
    public DeviceLevelStatus getBattLevelStatus() {
        Object obj = store.get(Names.battLevelStatus);
        if (obj instanceof DeviceLevelStatus) {
            return (DeviceLevelStatus) obj;
        } else if (obj instanceof String) {
        	DeviceLevelStatus theCode = null;
            try {
                theCode = DeviceLevelStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.battLevelStatus, e);
            }
            return theCode;
        }
        return null;
    }
    public void setStereoAudioOutputMuted(Boolean stereoAudioOutputMuted) {
        if (stereoAudioOutputMuted != null) {
        	store.put(Names.stereoAudioOutputMuted, stereoAudioOutputMuted);
        } else {
        	store.remove(Names.stereoAudioOutputMuted);
        }
    }
    public Boolean getStereoAudioOutputMuted() {
        return (Boolean) store.get(Names.stereoAudioOutputMuted);
    }
    public void setMonoAudioOutputMuted(Boolean monoAudioOutputMuted) {
        if (monoAudioOutputMuted != null) {
        	store.put(Names.monoAudioOutputMuted, monoAudioOutputMuted);
        } else {
        	store.remove(Names.monoAudioOutputMuted);
        }
    }
    public Boolean getMonoAudioOutputMuted() {
        return (Boolean) store.get(Names.monoAudioOutputMuted);
    }
    public void setSignalLevelStatus(DeviceLevelStatus signalLevelStatus) {
        if (signalLevelStatus != null) {
        	store.put(Names.signalLevelStatus, signalLevelStatus);
        } else {
        	store.remove(Names.signalLevelStatus);
        }
    }
    public DeviceLevelStatus getSignalLevelStatus() {
        Object obj = store.get(Names.signalLevelStatus);
        if (obj instanceof DeviceLevelStatus) {
            return (DeviceLevelStatus) obj;
        } else if (obj instanceof String) {
        	DeviceLevelStatus theCode = null;
            try {
                theCode = DeviceLevelStatus.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.signalLevelStatus, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPrimaryAudioSource(PrimaryAudioSource primaryAudioSource) {
        if (primaryAudioSource != null) {
        	store.put(Names.primaryAudioSource, primaryAudioSource);
        } else {
        	store.remove(Names.primaryAudioSource);
        }
    }
    public PrimaryAudioSource getPrimaryAudioSource() {
        Object obj = store.get(Names.primaryAudioSource);
        if (obj instanceof PrimaryAudioSource) {
            return (PrimaryAudioSource) obj;
        } else if (obj instanceof String) {
        	PrimaryAudioSource theCode = null;
            try {
                theCode = PrimaryAudioSource.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.primaryAudioSource, e);
            }
            return theCode;
        }
        return null;
    }
    public void setECallEventActive(Boolean eCallEventActive) {
        if (eCallEventActive != null) {
        	store.put(Names.eCallEventActive, eCallEventActive);
        } else {
        	store.remove(Names.eCallEventActive);
        }
    }
    public Boolean getECallEventActive() {
        return (Boolean) store.get(Names.eCallEventActive);
    }
}