package com.batutin.android.androidvideostreaming.media;

import android.test.AndroidTestCase;

/**
 * Created by Andrew Batutin on 8/5/13.
 */
public class AvcEncoderTest extends AndroidTestCase {


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
