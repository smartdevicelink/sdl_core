package com.batutin.android.androidvideostreaming.media;

import android.media.CamcorderProfile;

/**
 * Created by Andrew Batutin on 8/5/13.
 */
public class VideoAvcCoderTest extends MediaUtilsTest {

    private VideoAvcCoder sut;

    public VideoAvcCoderTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        sut = new VideoAvcCoder(null, null, CamcorderProfile.QUALITY_LOW);
    }

    public void testAbcEncoderCreationShouldSucceed() throws Exception {
        VideoAvcCoder e = new VideoAvcCoder(null, null, CamcorderProfile.QUALITY_LOW);
        assertNotNull("object should be created", e);
    }

    public void testLowQualityCoderCreationShouldSucceed() throws Exception {
        VideoAvcCoder e = VideoAvcCoder.createLowQualityVideoAvcCoder(null, null);
        assertNotNull("object should be created", e);
    }

    public void testHighQualityCoderCreationShouldSucceed() throws Exception {
        VideoAvcCoder e = VideoAvcCoder.createHighQualityVideoAvcCoder(null, null);
        assertNotNull("object should be created", e);
    }
}
