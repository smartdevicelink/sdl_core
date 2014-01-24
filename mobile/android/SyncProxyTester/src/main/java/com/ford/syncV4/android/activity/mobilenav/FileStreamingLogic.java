package com.ford.syncV4.android.activity.mobilenav;

import android.os.AsyncTask;

import com.ford.syncV4.android.activity.SyncProxyTester;

import java.io.IOException;
import java.io.OutputStream;

public class FileStreamingLogic {

    private StaticFileReader staticFileReader;
    private OutputStream outputStream;
    private ServicePreviewFragmentInterface context;
    private Integer fileResID;
    private boolean mIsStreamingComplete = true;

    public FileStreamingLogic(ServicePreviewFragmentInterface mobileNavPreviewFragment) {
        context = mobileNavPreviewFragment;
        mIsStreamingComplete = true;
    }

    public OutputStream getOutputStream() {
        return outputStream;
    }

    public void setOutputStream(OutputStream stream) {
        this.outputStream = stream;
    }

    public Integer getFileResID() {
        return fileResID;
    }

    public void setFileResID(Integer fileResID) {
        this.fileResID = fileResID;
    }

    public void cancelStreaming() {
        mIsStreamingComplete = true;
        if (staticFileReader != null) {
            staticFileReader.cancel(true);
        }
    }

    public void startFileStreaming() {
        mIsStreamingComplete = false;
        if (staticFileReader == null || staticFileReader.getStatus() == AsyncTask.Status.FINISHED){
            createStaticFileReader();
        }
        staticFileReader.execute(fileResID);
    }

    public boolean isStreamingComplete() {
        return mIsStreamingComplete;
    }

    public void createStaticFileReader() {

        staticFileReader = new StaticFileReader(context.getActivity(), new DataReaderListener() {

            @Override
            public void onStartReading() {
                context.dataStreamingStarted();
            }

            @Override
            public void onDataReceived(final byte[] data) {
                if (outputStream != null && data != null) {
                    try {
                        outputStream.write(data);
                    } catch (IOException e) {
                       cancelStreaming();
                       SyncProxyTester tester = (SyncProxyTester) context.getActivity();
                       tester.logError(e);
                    }
                }
            }

            @Override
            public void onCancelReading() {
                mIsStreamingComplete = true;
                context.dataStreamingStopped();
            }

            @Override
            public void onEndReading() {
                mIsStreamingComplete = true;
                context.dataStreamingStopped();
            }
        });
    }
}