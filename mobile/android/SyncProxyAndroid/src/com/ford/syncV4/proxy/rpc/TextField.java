package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.CharacterSet;
import com.ford.syncV4.proxy.rpc.enums.TextFieldName;
import com.ford.syncV4.util.DebugTool;

public class TextField extends RPCStruct {

    public TextField() { }
    public TextField(Hashtable hash) {
        super(hash);
    }
    public TextFieldName getName() {
        Object obj = store.get(Names.name);
        if (obj instanceof TextFieldName) {
            return (TextFieldName) obj;
        } else if (obj instanceof String) {
            TextFieldName theCode = null;
            try {
                theCode = TextFieldName.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.name, e);
            }
            return theCode;
        }
        return null;
    }
    public void setName( TextFieldName name ) {
        if (name != null) {
            store.put(Names.name, name );
        }
    }
    public CharacterSet getCharacterSet() {
        Object obj = store.get(Names.characterSet);
        if (obj instanceof CharacterSet) {
            return (CharacterSet) obj;
        } else if (obj instanceof String) {
            CharacterSet theCode = null;
            try {
                theCode = CharacterSet.valueForString((String) obj);
            } catch (Exception e) {
            	DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.characterSet, e);
            }
            return theCode;
        }
        return null;
    }
    public void setCharacterSet( CharacterSet characterSet ) {
        if (characterSet != null) {
            store.put(Names.characterSet, characterSet );
        }
    }
    public Integer getWidth() {
        return (Integer) store.get( Names.width );
    }
    public void setWidth( Integer width ) {
        if (width != null) {
            store.put(Names.width, width );
        }
    }
    public Integer getRows() {
        return (Integer) store.get( Names.rows );
    }
    public void setRows( Integer rows ) {
        if (rows != null) {
            store.put(Names.rows, rows );
        }
    }
}
