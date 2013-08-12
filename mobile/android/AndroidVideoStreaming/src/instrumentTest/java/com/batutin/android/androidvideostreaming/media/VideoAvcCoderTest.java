package com.batutin.android.androidvideostreaming.media;

/**
 * Created by Andrew Batutin on 8/5/13.
 */
public class VideoAvcCoderTest extends MediaUtilsTest {


    public VideoAvcCoderTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
    }

    public void testAvcEncoderCreation() throws Exception {
            VideoAvcCoder e = new VideoAvcCoder(null, null);
            assertNotNull("object should be created", e);
    }

}
