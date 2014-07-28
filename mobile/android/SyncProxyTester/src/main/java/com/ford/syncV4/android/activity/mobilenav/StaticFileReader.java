package com.ford.syncV4.android.activity.mobilenav;

import android.app.Activity;
import android.os.AsyncTask;

import com.ford.syncV4.util.logger.Logger;

import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.WeakReference;

/**
 * Created by Andrew Batutin on 9/11/13
 */
public class StaticFileReader extends AsyncTask<Integer, byte[], Void> {

    // A weak reference to the enclosing class
    // We do this to avoid memory leaks during Java Garbage Collection
    private WeakReference<Activity> mOuterContext;

    private DataReaderListener mListener;

    public StaticFileReader(Activity context, DataReaderListener listener) {
        mOuterContext = new WeakReference<Activity>(context);
        mListener = listener;
    }

    public void clear() {
        mOuterContext.clear();
        mListener = null;
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();
        mListener.onStartReading();
    }

    @Override
    protected Void doInBackground(Integer... ids) {
        Thread.currentThread().setName(this.getClass().getSimpleName());
        if (ids != null && ids.length > 0) {
            readFileFromRaw(ids[0]);
        }
        return null;
    }

    private synchronized void readFileFromRaw(Integer id) {
        // Get an actual reference to the Activity from the WeakReference.
        final Activity activity = mOuterContext.get();

        // Open the input stream
        final InputStream is = activity.getResources().openRawResource(id);
        final byte[] buffer = new byte[1000];
        int length;
        try {
            while ((length = is.read(buffer)) != -1 && !isCancelled()) {
                mListener.onDataReceived(buffer);
            }
            is.close();
        } catch (IOException e) {
            Logger.e("StaticFileReader", e.toString());
        }
    }

    @Override
    protected void onProgressUpdate(byte[]... values) {
        super.onProgressUpdate(values);
    }

    @Override
    protected void onPostExecute(Void aVoid) {
        super.onPostExecute(aVoid);
        mListener.onEndReading();
    }

    @Override
    protected void onCancelled() {
        super.onCancelled();
        mListener.onCancelReading();
    }
}
