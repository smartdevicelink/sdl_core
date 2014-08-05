package com.ford.syncV4.proxy.systemrequest;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 05.08.14
 * Time: 11:53
 */

/**
 * This is an interface which provides methods to indicate a progress statuses (Error or Success)
 * of the {@link com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandler} implementation class
 */
public interface IOnSystemRequestHandlerCallback {

    /**
     * This is a method to indicate an error which occur while processing notification
     *
     * @param appId   Application Id
     * @param message error message
     */
    public void onError(String appId, String message);

    /**
     * This is a method to indicate an error which occur while processing notification
     *
     * @param appId   Application Id
     * @param message success message
     */
    public void onSuccess(String appId, String message);
}