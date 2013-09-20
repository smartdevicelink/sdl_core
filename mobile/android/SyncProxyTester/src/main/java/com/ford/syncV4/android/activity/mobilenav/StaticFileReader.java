package com.ford.syncV4.android.activity.mobilenav;

import android.app.Activity;
import android.os.AsyncTask;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Andrew Batutin on 9/11/13.
 */
public class StaticFileReader extends AsyncTask<Integer, byte[], Void> {

    private final Activity mContext;
    private DataReaderListener mListener;

    public StaticFileReader(Activity context, DataReaderListener listener) {
        mContext = context;
        mListener = listener;
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();
        mListener.onStartReading();
    }

    @Override
    protected Void doInBackground(Integer... ids) {
        if (ids.length > 0) {
            readFileFromRaw(ids[0]);
        }
        return null;
    }

    private void readFileFromRaw(Integer id) {
        // Open the input stream
        InputStream is = mContext.getResources().openRawResource(id);
        byte[] buffer = new byte[100];
        int length;
        try {
            int i = 0;
            while ((length = is.read(buffer)) != -1 && !isCancelled()) {
                mListener.onDataReceived(buffer);
                Log.d("SyncProxyTester", "i = " + i);
                i++;
            }
            is.close();
        } catch (IOException e) {
            e.printStackTrace();
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
