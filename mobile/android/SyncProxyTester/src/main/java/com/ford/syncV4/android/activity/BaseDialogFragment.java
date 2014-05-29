package com.ford.syncV4.android.activity;

import android.app.DialogFragment;
import android.os.Bundle;

import com.ford.syncV4.util.logger.Logger;

import java.lang.reflect.InvocationTargetException;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 28.05.14
 * Time: 13:32
 */
public class BaseDialogFragment extends DialogFragment {

    protected static final String APP_ID_KEY = "APP_ID_KEY";
    protected static final String ARG_KEY_IS_TRANSPORT_VISIBLE = "ARG_KEY_IS_TRANSPORT_VISIBLE";

    private static final String LOG_TAG = BaseDialogFragment.class.getSimpleName();

    public static BaseDialogFragment newInstance(String className, String appId,
                                                 Boolean isTransportViewVisible) {

        BaseDialogFragment baseDialogFragment = null;
        try {
            baseDialogFragment = (BaseDialogFragment) Class.forName(className).getConstructor()
                    .newInstance();
        } catch (java.lang.InstantiationException e) {
            Logger.e(LOG_TAG + " new instance InstantiationException:" + e.getMessage());
        } catch (IllegalAccessException e) {
            Logger.e(LOG_TAG + " new instance IllegalAccessException:" + e.getMessage());
        } catch (InvocationTargetException e) {
            Logger.e(LOG_TAG + " new instance InvocationTargetException:" + e.getMessage());
        } catch (NoSuchMethodException e) {
            Logger.e(LOG_TAG + " new instance NoSuchMethodException:" + e.getMessage());
        } catch (ClassNotFoundException e) {
            Logger.e(LOG_TAG + " new instance ClassNotFoundException:" + e.getMessage());
        }
        Bundle bundle = new Bundle();
        bundle.putString(APP_ID_KEY, appId);
        if (isTransportViewVisible != null) {
            bundle.putBoolean(ARG_KEY_IS_TRANSPORT_VISIBLE, isTransportViewVisible);
        }
        baseDialogFragment.setArguments(bundle);
        return baseDialogFragment;
    }

    public static BaseDialogFragment newInstance(String className, String appId) {
        return newInstance(className, appId, null);
    }
}