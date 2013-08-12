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

    public static final String MIME_TYPE = "video/avc";
    public static final int FRAME_INTERVAL = 10;
    public static final int VIDEO_FRAME_WIDTH = profile.videoFrameWidth;
    public static final int VIDEO_FRAME_HEIGHT = profile.videoFrameHeight;
    public static final int VIDEO_BIT_RATE = profile.videoBitRate;
    public static final int VIDEO_FRAME_RATE = profile.videoFrameRate;

    public MediaUtilsTest() {
    }
}
