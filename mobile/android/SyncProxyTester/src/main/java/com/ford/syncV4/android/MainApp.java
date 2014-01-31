package com.ford.syncV4.android;

import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import com.ford.syncV4.android.service.IProxyServiceBinder;
import com.ford.syncV4.android.service.IProxyServiceConnection;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.android.service.ProxyServiceBinder;
import com.ford.syncV4.android.service.ProxyServiceConnectionProxy;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/29/14
 * Time: 4:31 PM
 */
public class MainApp extends Application implements IProxyServiceConnection {

    private static final String LOG_TAG = "SyncProxyTester";
    private static volatile MainApp sInstance = null;

    private final ProxyServiceConnectionProxy mProxyServiceConnectionProxy =
            new ProxyServiceConnectionProxy(this);
    private ProxyService mBoundProxyService;
    private IProxyServiceBinder mProxyServiceBinder;
    private final Handler mUIHandler = new Handler(Looper.getMainLooper());

    public MainApp() {
        super();
        sInstance = this;
    }

    /**
     * Double-checked singleton fetching
     * @return
     */
    public static MainApp getInstance() {
        if (sInstance == null) {
            synchronized(MainApp.class) {
                if (sInstance == null) {
                    new MainApp();
                }
            }
        }
        return sInstance;
    }

    @Override
    public void onCreate() {
        super.onCreate();

        Log.i(LOG_TAG, MainApp.class.getSimpleName() + " On Create, processors: " +
                Runtime.getRuntime().availableProcessors());
    }

    @Override
    public void onProxyServiceConnected(ProxyServiceBinder service) {
        Log.i(LOG_TAG, MainApp.class.getSimpleName() + " ProxyService connected " + service);
        mBoundProxyService = service.getService();
        if (mProxyServiceBinder != null) {
            mProxyServiceBinder.onServiceBindComplete();
        }
    }

    @Override
    public void onProxyServiceDisconnected() {
        Log.i(LOG_TAG, MainApp.class.getSimpleName() + " ProxyService disconnected");
        mBoundProxyService = null;
    }

    public void runInUIThread(Runnable r) {
        mUIHandler.post(r);
    }

    public void exitApp() {
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    public ProxyService getBoundProxyService() {
        return mBoundProxyService;
    }

    public void bindProxyToMainApp(IProxyServiceBinder binderCallback) {
        mProxyServiceBinder = binderCallback;
        bindProxyService(this, mProxyServiceConnectionProxy);
    }

    public void unbindProxyFromMainApp() {
        unbindProxyService(this, mProxyServiceConnectionProxy);
    }

    private void bindProxyService(Context context, ProxyServiceConnectionProxy connectionProxy) {
        Log.i(LOG_TAG, MainApp.class.getSimpleName() + " Bind ProxyService, connection proxy: " +
                connectionProxy);
        context.bindService(new Intent(context, ProxyService.class), connectionProxy,
                BIND_AUTO_CREATE);
    }

    private void unbindProxyService(Context context, ProxyServiceConnectionProxy connectionProxy) {
        if (!connectionProxy.isConnected()) {
            Log.v(LOG_TAG, MainApp.class.getSimpleName() + " ServiceConnection is not connected, " +
                    "ignoring unbindService: " + connectionProxy);
            return;
        }
        try {
            Log.i(LOG_TAG, MainApp.class.getSimpleName() + " Unbind Service, " +
                    "connection proxy: " + connectionProxy);
            context.unbindService(connectionProxy);
        } catch (IllegalArgumentException iae) {
            // sometimes this exception is still thrown, in spite of isConnected() check above
            // simply ignore this exception
            Log.w(LOG_TAG, MainApp.class.getSimpleName() + " Unbind IllegalArgumentException: " +
                    iae);
        } catch (Exception e) {
            Log.e(LOG_TAG, MainApp.class.getSimpleName() + " Error unbinding from connection: " +
                    connectionProxy, e);
        }
    }
}