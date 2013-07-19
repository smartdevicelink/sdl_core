package instrumentalTest.com.ford.syncV4.android.videostreaming;

import android.test.AndroidTestCase;

import com.ford.syncV4.android.videostreaming.AssetsReader;

import junit.framework.Assert;

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
        byte[] buffer = sut.readFileFromAssets(FILE_NAME);
        assertNotNull("Be null buffer should not", buffer);
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
