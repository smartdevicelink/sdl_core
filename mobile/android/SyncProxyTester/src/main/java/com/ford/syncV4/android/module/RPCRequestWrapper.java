package com.ford.syncV4.android.module;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.03.14
 * Time: 12:36
 */

import com.ford.syncV4.proxy.RPCRequest;

/**
 * Wraps the {@link com.ford.syncV4.proxy.RPCRequest} class to add some extra fields (pause after
 * the request; whether to generate invalid JSON; custom JSON to set in
 * request).
 */
class RPCRequestWrapper {
    private RPCRequest request = null;
    private long pause = 0;
    private boolean generateInvalidJSON = false;
    private String customJSON = null;

    public RPCRequestWrapper(RPCRequest request, long pause,
                             boolean generateInvalidJSON) {
        this(request, pause, generateInvalidJSON, null);
    }

    public RPCRequestWrapper(RPCRequest request, long pause,
                             boolean generateInvalidJSON, String customJSON) {
        this.request = request;
        this.pause = pause;
        this.generateInvalidJSON = generateInvalidJSON;
        this.customJSON = customJSON;
    }

    public long getPause() {
        return pause;
    }

    public void setPause(long pause) {
        this.pause = pause;
    }

    public RPCRequest getRequest() {
        return request;
    }

    public boolean isGenerateInvalidJSON() {
        return generateInvalidJSON;
    }

    public void setGenerateInvalidJSON(boolean generateInvalidJSON) {
        this.generateInvalidJSON = generateInvalidJSON;
    }

    public String getCustomJSON() {
        return customJSON;
    }

    public void setCustomJSON(String customJSON) {
        this.customJSON = customJSON;
    }
}