package instrumentalTest.com.ford.syncV4.android.videostreaming;

import android.content.res.AssetManager;
import android.test.AndroidTestCase;

import com.ford.syncV4.android.videostreaming.VideoStreaming;

import junit.framework.TestCase;

import java.io.InputStream;

/**
 * Created by admin on 7/19/13.
 */
public class VideoStreamingTestCase extends AndroidTestCase {

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
        String[] assets = aMnager.list("");
        InputStream stream = aMnager.open("test_video.mp4");
        byte[] bytes =  sut.readTestVideoFileFromStream(stream);
        assertNotNull(bytes);
    }
}
