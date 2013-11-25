package com.ford.avarsdl.business;

import android.app.Application;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;

import com.ford.avarsdl.util.AppUtils;
import com.ford.avarsdl.util.Logger;

import java.io.File;

/**
 * Created with IntelliJ IDEA.
 * User: ChernyshovYuriy
 * Date: 12.02.13
 */
public class MainApp extends Application {

    private static volatile MainApp mInstance = null;
    private final Handler mUIHandler = new Handler(Looper.getMainLooper());
    private int correlationID = 1;
    private boolean mConnectivityAvailable = false;

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

        AppUtils.setAppInstance(this);

        Logger.initLogger(this, true);
        Logger.i("+++ Create RevSDL Application +++");
        Logger.i("- processors: " + Runtime.getRuntime().availableProcessors());
        Logger.i("OS ver: " + Build.VERSION.RELEASE + ", API lvl: " + Build.VERSION.SDK_INT);
    }

    public File getExternalFilesDir(String type) {
        return super.getExternalFilesDir(type);
    }

    public synchronized int nextCorrelationID() {
        return correlationID++;
    }

    public void runInUIThread(Runnable runnable) {
        mUIHandler.post(runnable);
    }

    public void runInUIThread(Runnable r, long delay) {
        mUIHandler.postDelayed(r, delay);
    }
}