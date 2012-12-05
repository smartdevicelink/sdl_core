package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.util.DebugTool;

public class Show extends RPCRequest {

    public Show() {
        super("Show");
    }
    public Show(Hashtable hash) {
        super(hash);
    }
    public String getMainField1() {
        return (String) parameters.get( Names.mainField1 );
    }
    public void setMainField1( String mainField1 ) {
        if (mainField1 != null) {
            parameters.put(Names.mainField1, mainField1 );
        }
    }
    public String getMainField2() {
        return (String) parameters.get( Names.mainField2 );
    }
    public void setMainField2( String mainField2 ) {
        if (mainField2 != null) {
            parameters.put(Names.mainField2, mainField2 );
        }
    }
    public TextAlignment getAlignment() {
        Object obj = parameters.get(Names.alignment);
        if (obj instanceof TextAlignment) {
            return (TextAlignment) obj;
        } else if (obj instanceof String) {
            TextAlignment theCode = null;
            try {
                theCode = TextAlignment.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.alignment, e);
            }
            return theCode;
        }
        return null;
    }
    public void setAlignment( TextAlignment alignment ) {
        if (alignment != null) {
            parameters.put(Names.alignment, alignment );
        }
    }
    public String getStatusBar() {
        return (String) parameters.get( Names.statusBar );
    }
    public void setStatusBar( String statusBar ) {
        if (statusBar != null) {
            parameters.put(Names.statusBar, statusBar );
        }
    }
    public String getMediaClock() {
        return (String) parameters.get( Names.mediaClock );
    }
    public void setMediaClock( String mediaClock ) {
        if (mediaClock != null) {
            parameters.put(Names.mediaClock, mediaClock );
        }
    }
    public String getMediaTrack() {
        return (String) parameters.get( Names.mediaTrack );
    }
    public void setMediaTrack( String mediaTrack ) {
        if (mediaTrack != null) {
            parameters.put(Names.mediaTrack, mediaTrack );
        }
    }
}
