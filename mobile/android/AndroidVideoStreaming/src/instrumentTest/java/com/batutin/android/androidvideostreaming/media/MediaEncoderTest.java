package com.batutin.android.androidvideostreaming.media;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class MediaEncoderTest extends MediaUtilsTest {

    private MediaEncoder sut;

    public MediaEncoderTest() {

    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        sut = new MediaEncoder();
    }

    public void testEncoderCreation() throws Exception {
        MediaEncoder encoder = new MediaEncoder();
        assertNotNull(encoder.getEncoder());
    }


}
