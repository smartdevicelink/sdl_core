package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.constants.Names;

public class ListFilesResponse extends RPCResponse {

    public ListFilesResponse() {
        super("ListFiles");
    }
    public ListFilesResponse(Hashtable hash) {
        super(hash);
    }
    public void setFilenames(Vector<String> filenames) {
        if (filenames != null) {
            parameters.put(Names.filenames, filenames);
        } else {
        	parameters.remove(Names.filenames);
        }
    }
    public Vector<String> getFilenames() {
        if (parameters.get(Names.filenames) instanceof Vector<?>) {
        	Vector<?> list = (Vector<?>)parameters.get(Names.filenames);
        	if (list != null && list.size()>0) {
        		Object obj = list.get(0);
        		if (obj instanceof String) {
        			return (Vector<String>) list;
        		}
        	}
        }
    	return null;
    }
    public void setSpaceAvailable(Integer spaceAvailable) {
        if (spaceAvailable != null) {
            parameters.put(Names.spaceAvailable, spaceAvailable);
        } else {
        	parameters.remove(Names.spaceAvailable);
        }
    }
    public Integer getSpaceAvailable() {
        return (Integer) parameters.get(Names.spaceAvailable);
    }
}