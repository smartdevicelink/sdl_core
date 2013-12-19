package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

public class Speak extends RPCRequest {

    public Speak() {
        super("Speak");
    }
    public Speak(Hashtable hash) {
        super(hash);
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
}
