package instrumentalTest.com.ford.syncV4.android.videostreaming;

import android.content.res.AssetManager;
import android.test.AndroidTestCase;

import com.ford.syncV4.android.videostreaming.VideoStreaming;

import junit.framework.Assert;
import junit.framework.TestCase;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by admin on 7/19/13.
 */
public class VideoStreamingTestCase extends AndroidTestCase {

    public static final String FILE_NAME = "test_video.mp4";
    private VideoStreaming sut;

    public VideoStreamingTestCase() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        sut = new VideoStreaming();
    }

    public void testReadTestVideoFileFromStreamNotNull() throws Exception {
        AssetManager aMnager = this.getContext().getAssets();
        InputStream stream = aMnager.open(FILE_NAME);
        byte[] bytes =  sut.readTestVideoFileFromStream(stream);
        assertNotNull(bytes);
    }

    public void testReadFileWithNullStreamShouldThrowsException() throws Exception {
        try {
            sut.readTestVideoFileFromStream(null);
            Assert.fail("read with null stream should throws illegal arg exception");
        }catch (IllegalArgumentException e){}
    }
}
