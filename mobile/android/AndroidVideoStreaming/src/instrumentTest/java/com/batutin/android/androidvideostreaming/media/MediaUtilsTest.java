package com.batutin.android.androidvideostreaming.media;

import android.media.CamcorderProfile;
import android.test.AndroidTestCase;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class MediaUtilsTest extends AndroidTestCase {

    private static CamcorderProfile profile;
    static {
        profile = CamcorderProfileUtils.getFirstCameraCamcorderProfile(CamcorderProfile
                .QUALITY_LOW);
    }

    protected static final String MIME_TYPE = "video/avc";
    protected static final int FRAME_INTERVAL = 10;
    protected static final int VIDEO_FRAME_WIDTH = profile.videoFrameWidth;
    protected static final int VIDEO_FRAME_HEIGHT = profile.videoFrameHeight;
    protected static final int VIDEO_BIT_RATE = profile.videoBitRate;
    protected static final int VIDEO_FRAME_RATE = profile.videoFrameRate;

    public MediaUtilsTest() {
    }
}
