package com.ford.syncV4.test;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 20.03.14
 * Time: 10:26
 */

/**
 * This class provide all interfaces that are necessary to perform Test Cases over SDK
 */
public interface ITestConfigCallback {

    /**
     * Indicated that RPC Service has been started and
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface} is ready to be created. This callback
     * need to STOP call {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface}
     */
    public void onRPCServiceComplete();
}