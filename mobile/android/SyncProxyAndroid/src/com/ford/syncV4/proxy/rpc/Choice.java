package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;
import java.util.Vector;

public class Choice extends RPCStruct {

    public Choice() {
    }

    public Choice(Hashtable hash) {
        super(hash);
    }

    public Integer getChoiceID() {
        return (Integer) store.get(Names.choiceID);
    }

    public void setChoiceID(Integer choiceID) {
        if (choiceID != null) {
            store.put(Names.choiceID, choiceID);
        }
    }

    public String getMenuName() {
        return (String) store.get(Names.menuName);
    }

    public void setMenuName(String menuName) {
        if (menuName != null) {
            store.put(Names.menuName, menuName);
        }
    }

    public Vector<String> getVrCommands() {
        if (store.get(Names.vrCommands) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) store.get(Names.vrCommands);
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof String) {
                    return (Vector<String>) list;
                }
            }
        }
        return null;
    }

    public void setVrCommands(Vector<String> vrCommands) {
        if (vrCommands != null) {
            store.put(Names.vrCommands, vrCommands);
        }
    }

    public Image getImage() {
        Object obj = store.get(Names.image);
        if (obj instanceof Image) {
            return (Image) obj;
        } else if (obj instanceof Hashtable) {
            try {
                return new Image((Hashtable) obj);
            } catch (Exception e) {
                DebugTool.logError("Failed to parse " + getClass().getSimpleName() + "." + Names.image, e);
            }
        }
        return null;
    }

    public void setImage(Image image) {
        if (image != null) {
            store.put(Names.image, image);
        } else {
            store.remove(Names.image);
        }
    }

    public String getSecondaryText() {
        return (String) store.get(Names.secondaryText);
    }

    public void setSecondaryText(String secondaryText) {
        if (secondaryText != null) {
            store.put(Names.secondaryText, secondaryText);
        }
    }

    public String getTertiaryText() {
        return (String) store.get(Names.tertiaryText);
    }

    public void setTertiaryText(String tertiaryText) {
        if (tertiaryText != null) {
            store.put(Names.tertiaryText, tertiaryText);
        }
    }

    public void setSecondaryImage(Image image) {
        if (image != null) {
            store.put(Names.secondaryImage, image);
        } else {
            store.remove(Names.secondaryImage);
        }
    }
}
