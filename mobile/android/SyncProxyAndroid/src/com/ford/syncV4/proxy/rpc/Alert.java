package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class Alert extends RPCRequest {

    public Alert() {
        super("Alert");
    }
    public Alert(Hashtable hash) {
        super(hash);
    }
    public String getAlertText1() {
        return (String) parameters.get( Names.alertText1 );
    }
    public void setAlertText1( String alertText1 ) {
        if (alertText1 != null) {
            parameters.put(Names.alertText1, alertText1 );
        }
    }
    public String getAlertText2() {
        return (String) parameters.get( Names.alertText2 );
    }
    public void setAlertText2( String alertText2 ) {
        if (alertText2 != null) {
            parameters.put(Names.alertText2, alertText2 );
        }
    }
    public Vector<TTSChunk> getTtsChunks() {
        if (parameters.get(Names.ttsChunks) instanceof Vector<?>) {
	    	Vector<?> list = (Vector<?>)parameters.get(Names.ttsChunks);
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
    public void setTtsChunks( Vector<TTSChunk> ttsChunks ) {
        if (ttsChunks != null) {
            parameters.put(Names.ttsChunks, ttsChunks );
        }
    }
    public Integer getDuration() {
        return (Integer) parameters.get( Names.duration );
    }
    public void setDuration( Integer duration ) {
        if (duration != null) {
            parameters.put(Names.duration, duration );
        }
    }
    public Boolean getPlayTone() {
        return (Boolean) parameters.get( Names.playTone );
    }
    public void setPlayTone( Boolean playTone ) {
        if (playTone != null) {
            parameters.put(Names.playTone, playTone );
        }
    }
}
