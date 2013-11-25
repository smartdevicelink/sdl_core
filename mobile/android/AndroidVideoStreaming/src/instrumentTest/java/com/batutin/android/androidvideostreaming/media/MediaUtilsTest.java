package com.batutin.android.androidvideostreaming.media;

import android.media.CamcorderProfile;
import android.test.InstrumentationTestCase;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class MediaUtilsTest extends InstrumentationTestCase {

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

    @Override
    public void setUp() throws Exception {
        super.setUp();
        System.setProperty("dexmaker.dexcache", getInstrumentation().getTargetContext().getCacheDir().getPath());
    }
}
