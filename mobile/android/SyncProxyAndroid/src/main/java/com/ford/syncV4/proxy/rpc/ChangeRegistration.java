package com.ford.syncV4.proxy.rpc;

import java.util.Hashtable;
import java.util.Vector;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.util.logger.Logger;

public class ChangeRegistration extends RPCRequest {

    public ChangeRegistration() {
        super(Names.ChangeRegistration);
    }

    public ChangeRegistration(Hashtable hash) {
        super(hash);
    }

    /**
     * Requested voice engine (VR+TTS) language registration
     *
     * @param language {@link com.ford.syncV4.proxy.rpc.enums.Language}
     */
    public void setLanguage(Language language) {
        if (language != null) {
            parameters.put(Names.language, language);
        } else {
            parameters.remove(Names.language);
        }
    }

    /**
     * Requested voice engine (VR+TTS) language registration
     *
     * @return {@link com.ford.syncV4.proxy.rpc.enums.Language}
     */
    public Language getLanguage() {
        Object obj = parameters.get(Names.language);
        if (obj instanceof Language) {
            return (Language) obj;
        } else if (obj instanceof String) {
            Language theCode = null;
            try {
                theCode = Language.valueForString((String) obj);
            } catch (Exception e) {
                Logger.e("Failed to parse " + getClass().getSimpleName() + "." + Names.language, e);
            }
            return theCode;
        }
        return null;
    }

    /**
     * Request display language registration
     *
     * @param hmiDisplayLanguage {@link com.ford.syncV4.proxy.rpc.enums.Language}
     */
    public void setHmiDisplayLanguage(Language hmiDisplayLanguage) {
        if (hmiDisplayLanguage != null) {
            parameters.put(Names.hmiDisplayLanguage, hmiDisplayLanguage);
        } else {
            parameters.remove(Names.hmiDisplayLanguage);
        }
    }

    /**
     * Request display language registration
     *
     * @return {@link com.ford.syncV4.proxy.rpc.enums.Language}
     */
    public Language getHmiDisplayLanguage() {
        Object obj = parameters.get(Names.hmiDisplayLanguage);
        if (obj instanceof Language) {
            return (Language) obj;
        } else if (obj instanceof String) {
            Language theCode = null;
            try {
                theCode = Language.valueForString((String) obj);
            } catch (Exception e) {
                Logger.e("Failed to parse " + getClass().getSimpleName() + "." + Names.hmiDisplayLanguage, e);
            }
            return theCode;
        }
        return null;
    }

    /**
     * Request new app name registration
     *
     * @return Application name
     */
    public Object getAppName() {
        return parameters.get(Names.appName);
    }

    /**
     * Request new app name registration
     *
     * @param appName Application name, must be String
     */
    public void setAppName(Object appName) {
        if (appName != null) {
            parameters.put(Names.appName, appName);
        } else {
            parameters.remove(Names.appName);
        }
    }

    /**
     * Request new ttsName registration
     *
     * @param ttsName array of the {@link com.ford.syncV4.proxy.rpc.TTSChunk}'s
     */
    public void setTtsName(Vector<TTSChunk> ttsName) {
        if (ttsName != null) {
            parameters.put(Names.ttsName, ttsName);
        } else {
            parameters.remove(Names.ttsName);
        }
    }

    /**
     * Request new ttsName registration
     *
     * @return array of the {@link com.ford.syncV4.proxy.rpc.TTSChunk}'s
     */
    public Vector<TTSChunk> getTtsName() {
        if (parameters.get(Names.ttsName) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) parameters.get(Names.ttsName);
            if (list == null) {
                return null;
            }
            if (list.size() == 0) {
                return (Vector<TTSChunk>) list;
            }
            Object obj = list.get(0);
            if (obj instanceof TTSChunk) {
                return (Vector<TTSChunk>) list;
            } else if (obj instanceof Hashtable) {
                Vector<TTSChunk> newList = new Vector<TTSChunk>();
                for (Object hashObj : list) {
                    newList.add(new TTSChunk((Hashtable) hashObj));
                }
                return newList;
            }
        }
        return null;
    }

    /**
     * Request new app short name registration
     *
     * @return abbreviated version of the app name
     */
    public Object getNgnMediaScreenAppName() {
        return parameters.get(Names.ngnMediaScreenAppName);
    }

    /**
     * Request new app short name registration
     *
     * @param ngnMediaScreenAppName abbreviated version of the app name
     */
    public void setNgnMediaScreenAppName(Object ngnMediaScreenAppName) {
        if (ngnMediaScreenAppName != null) {
            parameters.put(Names.ngnMediaScreenAppName, ngnMediaScreenAppName);
        } else {
            parameters.remove(Names.ngnMediaScreenAppName);
        }
    }

    /**
     * Request new VR synonyms registration
     *
     * @return additional voice recognition command
     */
    public Vector<Object> getVrSynonyms() {
        if (parameters.get(Names.vrSynonyms) instanceof Vector<?>) {
            Vector<?> list = (Vector<?>) parameters.get(Names.vrSynonyms);
            if (list == null) {
                return null;
            }
            if (list.size() == 0) {
                return (Vector<Object>) list;
            }
            Object obj = list.get(0);
            if (obj instanceof Object) {
                return (Vector<Object>) list;
            }
        }
        return null;
    }

    /**
     * Request new VR synonyms registration
     *
     * @param vrSynonyms additional voice recognition command
     */
    public void setVrSynonyms(Vector<Object> vrSynonyms) {
        if (vrSynonyms != null) {
            parameters.put(Names.vrSynonyms, vrSynonyms);
        } else {
            parameters.remove(Names.vrSynonyms);
        }
    }
}