package com.ford.syncV4.android.activity;

import com.ford.syncV4.proxy.RPCRequest;

/**
 * Created by Andrew Batutin on 4/14/14.
 */
public class MessageFilter {
    private static boolean encrypt;

    public static void setEncrypt(boolean encrypt) {
        MessageFilter.encrypt = encrypt;
    }

    public static boolean isEncrypt() {
        return encrypt;
    }

    public static void filter(RPCRequest msg) {
        msg.setDoEncryption(encrypt);
    }
}
