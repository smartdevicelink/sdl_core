package com.batutin.android.androidvideostreaming.test.reader;

import android.test.AndroidTestCase;

import com.batutin.android.androidvideostreaming.reader.AssetsReader;

import junit.framework.Assert;

import java.io.InputStream;

/**
 * Created by admin on 7/19/13.
 */
public class AssetsReaderTestCase extends AndroidTestCase {

    public static final String FILE_NAME = "test_video.mp4";
    private AssetsReader sut;

    public AssetsReaderTestCase() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        sut = new AssetsReader(this.getContext());
    }

    public void testAssetsReaderShouldReadFromAssets() throws Exception {
        InputStream inputStream = sut.readFileFromAssets(FILE_NAME);
        assertNotNull("Be null inputStream should not", inputStream);
    }

    public void testAssertReaderShouldHaveContextInitialised() throws Exception {
        AssetsReader reader = new AssetsReader(this.getContext());
        assertNotNull("Be initialised, in order to load video file from assets android context object should.", reader.getContext());
    }

    public void testAssertReaderReadFileFromAssetsWithNullContextShouldThrowException() {
        sut.setContext(null);
        try {
            sut.readFileFromAssets(FILE_NAME);
            Assert.fail("with null context illegal argument exception thrown should be");
        } catch (IllegalArgumentException e) {
        }
    }
}
