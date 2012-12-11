package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.util.DebugTool;

public class PutFile extends RPCRequest {

    public PutFile() {
        super("PutFile");
    }
    public PutFile(Hashtable hash) {
        super(hash);
    }
    public void setSyncFileName(String syncFileName) {
        if (syncFileName != null) {
            parameters.put(Names.syncFileName, syncFileName);
        } else {
        	parameters.remove(Names.syncFileName);
        }
    }
    public String getSyncFileName() {
        return (String) parameters.get(Names.syncFileName);
    }
    public void setFileType(FileType fileType) {
        if (fileType != null) {
            parameters.put(Names.fileType, fileType);
        } else {
        	parameters.remove(Names.fileType);
        }
    }
    public FileType getFileType() {
        Object obj = parameters.get(Names.fileType);
        if (obj instanceof FileType) {
            return (FileType) obj;
        } else if (obj instanceof String) {
        	FileType theCode = null;
            try {
                theCode = FileType.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.fileType, e);
            }
            return theCode;
        }
        return null;
    }
    public void setPersistentFile(Boolean persistentFile) {
        if (persistentFile != null) {
            parameters.put(Names.persistentFile, persistentFile);
        } else {
        	parameters.remove(Names.persistentFile);
        }
    }
    public Boolean getPersistentFile() {
        return (Boolean) parameters.get(Names.persistentFile);
    }
    public void setFileData(byte[] fileData) {
        if (fileData != null) {
            store.put(Names.bulkData, fileData);
        } else {
        	store.remove(Names.bulkData);
        }
    }
    public byte[] getFileData() {
        return (byte[]) store.get(Names.bulkData);
    }
}
