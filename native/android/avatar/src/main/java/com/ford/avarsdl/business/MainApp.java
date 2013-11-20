package com.ford.avarsdl.business;

import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import yuriy.chernyshov.yakimbi.business.flickr_service.*;
import yuriy.chernyshov.yakimbi.business.net.ConnectivityReceiver;
import yuriy.chernyshov.yakimbi.business.net.IConnectivityEvents;

import java.io.File;

/**
 * Created with IntelliJ IDEA.
 * User: ChernyshovYuriy
 * Date: 12.02.13
 */
public class MainApp extends Application implements IConnectivityEvents {

    public static final String APP_TAG = "Yakimbi";

    private static volatile MainApp mInstance = null;
    private final Handler mUIHandler = new Handler(Looper.getMainLooper());
    private final ConnectivityReceiver mConnectivityReceiver = new ConnectivityReceiver(this);

    //private List<WeakReference<IConnectivityEvents>> mConnectivityListeners = new ArrayList<WeakReference<IConnectivityEvents>>();
    private boolean mConnectivityAvailable = false;

    private volatile boolean isProgressTerminated = false;

    public MainApp() {
        super();
        mInstance = this;
    }

    public static MainApp getInstance() {
        if (mInstance == null) {
            synchronized (MainApp.class) {
                if (mInstance == null) {
                    new MainApp();
                }
            }
        }
        return mInstance;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(APP_TAG, "Yakimbi App stared");

        // In case we need to bind to Service right from the Application
        //bindFlickrService(this, mFlickrServiceConnectionProxy);

        startConnectivityReceiver();
    }

    public void bindFlickrService(Context context, FlickrServiceConnectionProxy connectionProxy) {
        Log.i(APP_TAG, "BindStorageService(), connection proxy: " + connectionProxy);
        context.bindService(new Intent(context, FlickrService.class), connectionProxy, BIND_AUTO_CREATE);
    }

    public void unbindFlickrService(Context context, FlickrServiceConnectionProxy connectionProxy) {
        if (!connectionProxy.isConnected()) {
            Log.v(APP_TAG, "ServiceConnection is not connected, ignoring unbindService: " + connectionProxy);
            return;
        }
        try {
            Log.i(APP_TAG, "Unbind Service(), connection proxy: " + connectionProxy);
            context.unbindService(connectionProxy);
        } catch (IllegalArgumentException iae) {
            // sometimes this exception is still thrown, in spite of isConnected() check above
            // simply ignore this exception
            Log.w(APP_TAG, "Unbind IllegalArgumentException: " + iae);
        } catch (Exception e) {
            Log.e(APP_TAG, "Error unbinding from connection: " + connectionProxy, e);
        }
    }

    public File getExternalFilesDir(String type) {
        return super.getExternalFilesDir(type);
    }

    public void runInUIThread(Runnable runnable) {
        mUIHandler.post(runnable);
    }

    public void runInUIThread(Runnable r, long delay) {
        mUIHandler.postDelayed(r, delay);
    }

    public boolean isProgressTerminated() {
        return isProgressTerminated;
    }

    public void setProgressTerminated(boolean progressTerminated) {
        isProgressTerminated = progressTerminated;
    }

    private void startConnectivityReceiver() {
        try {
            Log.v(APP_TAG, "Register ConnectivityReceiver");
            registerReceiver(mConnectivityReceiver, new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION));
        } catch (Throwable th) {
            Log.e(APP_TAG, "Can't register mConnectivityReceiver", th);
        }
        requestConnectivityStatus();
    }

    /*public void stopConnectivityReceiver() {
        try {
            Log.v(APP_TAG, "Unregister ConnectivityReceiver");
            unregisterReceiver(mConnectivityReceiver);
        } catch (IllegalArgumentException e) {
            Log.d(APP_TAG, "ConnectivityReceiver actually is not registered, ignoring");
        }
    }*/

    public boolean isConnectivityAvailable() {
        return mConnectivityAvailable;
    }

    /*public synchronized void addConnectivityListener(IConnectivityEvents listener) {
        mConnectivityListeners.add(new WeakReference<IConnectivityEvents>(listener));
    }

    public synchronized void removeConnectivityListener(IConnectivityEvents listener) {
        for (WeakReference<IConnectivityEvents> ref : mConnectivityListeners) {
            IConnectivityEvents cb = ref.get();
            if (listener.equals(cb)) {
                mConnectivityListeners.remove(ref);
                return;
            }
        }
    }*/

    public void requestConnectivityStatus() {
        mConnectivityReceiver.requestConnectivityStatus();
    }

    @Override
    public synchronized void onConnectivityChanged(boolean available) {
        mConnectivityAvailable = available;
        /*for (WeakReference<IConnectivityEvents> ref : mConnectivityListeners) {
            IConnectivityEvents callback = ref.get();
            if (callback != null) {
                callback.onConnectivityChanged(available);
            }
        }*/
    }
}