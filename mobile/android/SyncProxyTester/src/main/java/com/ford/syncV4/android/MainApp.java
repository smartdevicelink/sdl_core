package com.ford.syncV4.android;

import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Looper;

import com.ford.syncV4.android.manager.LastUsedHashIdsManager;
import com.ford.syncV4.android.service.IProxyServiceBinder;
import com.ford.syncV4.android.service.IProxyServiceConnection;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.android.service.ProxyServiceBinder;
import com.ford.syncV4.android.service.ProxyServiceConnectionProxy;
import com.ford.syncV4.util.logger.Logger;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/29/14
 * Time: 4:31 PM
 */
public class MainApp extends Application implements IProxyServiceConnection {

    private static volatile MainApp sInstance = null;

    private final ProxyServiceConnectionProxy mProxyServiceConnectionProxy =
            new ProxyServiceConnectionProxy(this);
    private ProxyService mBoundProxyService;
    private IProxyServiceBinder mProxyServiceBinder;
    /**
     * This manager keep last used hash id's which are in use at the
     * {@link com.ford.syncV4.proxy.rpc.RegisterAppInterface#getHashID()}
      */
    private LastUsedHashIdsManager mLastUsedHashIdsManager;
    private final Handler mUIHandler = new Handler(Looper.getMainLooper());

    public MainApp() {
        super();
        sInstance = this;

        mLastUsedHashIdsManager = new LastUsedHashIdsManager();
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

        Logger.initLogger(this);

        Logger.i(MainApp.class.getSimpleName() + " On Create, processors:" +
                Runtime.getRuntime().availableProcessors());
    }

    @Override
    public void onProxyServiceConnected(ProxyServiceBinder service) {
        Logger.i(MainApp.class.getSimpleName() + " ProxyService connected " + service);
        mBoundProxyService = service.getService();
        if (mProxyServiceBinder != null) {
            mProxyServiceBinder.onServiceBindComplete();
        }
    }

    @Override
    public void onProxyServiceDisconnected() {
        Logger.i(MainApp.class.getSimpleName() + " ProxyService disconnected");
        mBoundProxyService = null;
    }

    public void runInUIThread(Runnable r) {
        mUIHandler.post(r);
    }

    public void exitApp() {
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    /**
     * Return {@link com.ford.syncV4.android.manager.LastUsedHashIdsManager} reference
     *
     * @return {@link com.ford.syncV4.android.manager.LastUsedHashIdsManager}
     */
    public LastUsedHashIdsManager getLastUsedHashIdsManager() {
        return mLastUsedHashIdsManager;
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
        Logger.i(MainApp.class.getSimpleName() + " Bind ProxyService, connection proxy: " +
                connectionProxy);
        context.bindService(new Intent(context, ProxyService.class), connectionProxy,
                BIND_AUTO_CREATE);
    }

    private void unbindProxyService(Context context, ProxyServiceConnectionProxy connectionProxy) {
        if (!connectionProxy.isConnected()) {
            Logger.w(MainApp.class.getSimpleName() + " ServiceConnection is not connected, " +
                    "ignoring unbindService: " + connectionProxy);
            return;
        }
        try {
            Logger.i(MainApp.class.getSimpleName() + " Unbind Service, " +
                    "connection proxy: " + connectionProxy);
            context.unbindService(connectionProxy);
        } catch (IllegalArgumentException iae) {
            // sometimes this exception is still thrown, in spite of isConnected() check above
            // simply ignore this exception
            Logger.w(MainApp.class.getSimpleName() + " Unbind IllegalArgumentException: " +
                    iae);
        } catch (Exception e) {
            Logger.e(MainApp.class.getSimpleName() + " Error unbinding from connection: " +
                    connectionProxy, e);
        }
    }
}