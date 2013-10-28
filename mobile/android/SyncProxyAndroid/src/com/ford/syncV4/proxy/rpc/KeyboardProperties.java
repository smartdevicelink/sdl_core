package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.KeyboardLayout;
import com.ford.syncV4.proxy.rpc.enums.KeypressMode;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.util.DebugTool;

import java.util.Hashtable;
import java.util.Vector;

public class KeyboardProperties extends RPCStruct {
    private static final KeypressMode KEYPRESS_MODE_DEFAULT =
            KeypressMode.RESEND_CURRENT_ENTRY;

    public KeyboardProperties() {
        store.put(Names.keypressMode, KEYPRESS_MODE_DEFAULT);
    }

    public KeyboardProperties(Hashtable hash) {
        super(hash);
        if (!store.containsKey(Names.keypressMode)) {
            store.put(Names.keypressMode, KEYPRESS_MODE_DEFAULT);
        }
    }

    public Language getLanguage() {
        Object obj = store.get(Names.language);
        if (obj instanceof Language) {
            return (Language) obj;
        } else if (obj instanceof String) {
            Language theCode = null;
            try {
                theCode = Language.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.language, e);
            }
            return theCode;
        }
        return null;
    }

    public void setLanguage(Language language) {
        if (language != null) {
            store.put(Names.language, language);
        } else {
            store.remove(Names.language);
        }
    }

    public KeyboardLayout getKeyboardLayout() {
        Object obj = store.get(Names.keyboardLayout);
        if (obj instanceof KeyboardLayout) {
            return (KeyboardLayout) obj;
        } else if (obj instanceof String) {
            KeyboardLayout theCode = null;
            try {
                theCode = KeyboardLayout.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.keyboardLayout, e);
            }
            return theCode;
        }
        return null;
    }

    public void setKeyboardLayout(KeyboardLayout keyboardLayout) {
        if (keyboardLayout != null) {
            store.put(Names.keyboardLayout, keyboardLayout);
        } else {
            store.remove(Names.keyboardLayout);
        }
    }

    public KeypressMode getKeypressMode() {
        Object obj = store.get(Names.keypressMode);
        if (obj instanceof KeypressMode) {
            return (KeypressMode) obj;
        } else if (obj instanceof String) {
            KeypressMode theCode = null;
            try {
                theCode = KeypressMode.valueForString((String) obj);
            } catch (Exception e) {
                DebugTool.logError(
                        "Failed to parse " + getClass().getSimpleName() + "." +
                                Names.keypressMode, e);
            }
            return theCode;
        }
        return KEYPRESS_MODE_DEFAULT;
    }

    public void setKeypressMode(KeypressMode keypressMode) {
        if (keypressMode != null) {
            store.put(Names.keypressMode, keypressMode);
        } else {
            store.put(Names.keypressMode, KEYPRESS_MODE_DEFAULT);
        }
    }

    public Boolean getSendDynamicEntry() {
        final Object obj = store.get(Names.sendDynamicEntry);
        if (obj instanceof Boolean) {
            return (Boolean) obj;
        }
        return null;
    }

    public void setSendDynamicEntry(Boolean sendDynamicEntry) {
        if (sendDynamicEntry != null) {
            store.put(Names.sendDynamicEntry, sendDynamicEntry);
        } else {
            store.remove(Names.sendDynamicEntry);
        }
    }

    public Vector<String> getLimitedCharacterList() {
        final Object listObj = store.get(Names.limitedCharacterList);
        if (listObj instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) listObj;
            if (list != null && list.size() > 0) {
                Object obj = list.get(0);
                if (obj instanceof String) {
                    return (Vector<String>) list;
                }
            }
        }
        return null;
    }

    public void setLimitedCharacterList(Vector<String> limitedCharacterList) {
        if (limitedCharacterList != null) {
            store.put(Names.limitedCharacterList, limitedCharacterList);
        } else {
            store.remove(Names.limitedCharacterList);
        }
    }

    public String getAutoCompleteText() {
        final Object obj = store.get(Names.autoCompleteText);
        if (obj instanceof String) {
            return (String) obj;
        }
        return null;
    }

    public void setAutoCompleteText(String autoCompleteText) {
        if (autoCompleteText != null) {
            store.put(Names.autoCompleteText, autoCompleteText);
        } else {
            store.remove(Names.autoCompleteText);
        }
    }
}
