package com.ford.syncV4.android.activity.mobilenav;

import android.app.Activity;

/**
 * Created by Andrew Batutin on 1/23/14.
 */
public interface ServicePreviewFragmentInterface {

    public void dataStreamingStarted();

    public void dataStreamingStopped();

    public void dataStreamingCanceled();

    public Activity getActivity();
}