package com.batutin.android.androidvideostreaming.media;

/**
 * Created by Andrew Batutin on 8/5/13.
 */
public class AvcEncoderTest extends MediaUtilsTest {


    public AvcEncoderTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
    }

    public void testAvcEncoderCreation() throws Exception {
            AvcEncoder e = new AvcEncoder(null, null);
            assertNotNull("object should be created", e);
    }

}
