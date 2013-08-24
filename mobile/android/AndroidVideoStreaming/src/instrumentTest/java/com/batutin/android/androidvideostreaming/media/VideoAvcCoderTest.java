package com.batutin.android.androidvideostreaming.media;

import android.media.CamcorderProfile;

import java.io.InputStream;

import static org.mockito.Mockito.mock;

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
        sut = new VideoAvcCoder(null, null, CamcorderProfile.QUALITY_LOW, mock(VideoAvcCoderListener.class));
    }

    public void testAbcEncoderCreationShouldSucceed() throws Exception {
        VideoAvcCoder e = new VideoAvcCoder(null, null, CamcorderProfile.QUALITY_LOW, mock(VideoAvcCoderListener.class));
        assertNotNull("object should be created", e);
    }

    public void testVideoAvcCoderListenerIsSet() throws Exception {
        VideoAvcCoder e = new VideoAvcCoder(null, null, CamcorderProfile.QUALITY_LOW, mock(VideoAvcCoderListener.class));
        assertNotNull("listener should be set", e.getVideoAvcCoderListener());
    }

    public void testLowQualityCoderCreationShouldSucceed() throws Exception {
        VideoAvcCoder e = VideoAvcCoder.createLowQualityVideoAvcCoder(null, null, mock(VideoAvcCoderListener.class));
        assertNotNull("object should be created", e);
    }

    public void testHighQualityCoderCreationShouldSucceed() throws Exception {
        VideoAvcCoder e = VideoAvcCoder.createHighQualityVideoAvcCoder(null, null, mock(VideoAvcCoderListener.class));
        assertNotNull("object should be created", e);
    }

    public void testVideoAvcEncoderCreationWithNullListenerShouldThrowExept() throws Exception {
        try {
            VideoAvcCoder e = new VideoAvcCoder(null, null, CamcorderProfile.QUALITY_LOW, null);
            assertNull("Should not get here", e);
        } catch (IllegalArgumentException e) {
            assertNotNull("can't create VideoAvcCoder without listener", e);
        }
    }

    public void testVideoEncoderShouldStart() throws Exception {
        final VideoAvcCoder videoAvcCoder = new VideoAvcCoder(null, null, CamcorderProfile.QUALITY_LOW, new VideoAvcCoderListener() {
            @Override
            public void coderStarted(VideoAvcCoder coder) {
                assertNotNull("Coder should start", coder);
            }

            @Override
            public void errorOnCoderStart(VideoAvcCoder coder, String errorDescription) {
                assertNull("Should not get here", errorDescription);
            }

            @Override
            public void coderShouldStop(VideoAvcCoder videoAvcCoder) {
                assertNull("Should not get here", videoAvcCoder);
            }

            @Override
            public void coderStopped(VideoAvcCoder videoAvcCoder) {
                assertNull("Should not get here", videoAvcCoder);
            }

            @Override
            public void errorOnCoderStop(VideoAvcCoder videoAvcCoder, String errorMessage) {
                assertNull("Should not get here", videoAvcCoder);
            }
        });
        videoAvcCoder.start();
    }

    public void testVideoEncoderShouldFailToStart() throws Exception {
        final String errorMessage = "mock error";
        final VideoAvcCoder videoAvcCoder = new VideoAvcCoder(null, null, CamcorderProfile.QUALITY_LOW, new VideoAvcCoderListener() {
            @Override
            public void coderStarted(VideoAvcCoder coder) {
                assertNull("Coder should not start", coder);
            }

            @Override
            public void errorOnCoderStart(VideoAvcCoder coder, String errorDescription) {
                assertEquals("Should get error message", errorMessage, errorDescription);
            }

            @Override
            public void coderShouldStop(VideoAvcCoder videoAvcCoder) {
                assertNull("Should not get here", videoAvcCoder);
            }

            @Override
            public void coderStopped(VideoAvcCoder videoAvcCoder) {
                assertNull("Should not get here", videoAvcCoder);
            }

            @Override
            public void errorOnCoderStop(VideoAvcCoder videoAvcCoder, String errorMessage) {
                assertNull("Should not get here", videoAvcCoder);
            }
        });
        videoAvcCoder.getVideoAvcCoderListener().errorOnCoderStart(null, errorMessage);
    }

    public void testVideoEncoderShouldStop() throws Exception {
        final VideoAvcCoder videoAvcCoder = new VideoAvcCoder(null, null, CamcorderProfile.QUALITY_LOW, new VideoAvcCoderListener() {
            @Override
            public void coderStarted(VideoAvcCoder coder) {
                assertNull("Should not get here", coder);
            }

            @Override
            public void errorOnCoderStart(VideoAvcCoder coder, String errorDescription) {
                assertNull("Should not get here", errorDescription);
            }

            @Override
            public void coderShouldStop(VideoAvcCoder videoAvcCoder) {
                assertNotNull("Coder should stop", videoAvcCoder);
            }

            @Override
            public void coderStopped(VideoAvcCoder videoAvcCoder) {
                assertNull("Should not get here", videoAvcCoder);
            }

            @Override
            public void errorOnCoderStop(VideoAvcCoder videoAvcCoder, String errorMessage) {
                assertNull("Should not get here", videoAvcCoder);
            }
        });
        videoAvcCoder.stop();
    }

    public void testVideoEncoderShouldForceStop() throws Exception {
        final String errorMessage = "mock error";
        final VideoAvcCoder videoAvcCoder = new VideoAvcCoder(null, mock(InputStream.class), CamcorderProfile.QUALITY_LOW, new VideoAvcCoderListener() {
            @Override
            public void coderStarted(VideoAvcCoder coder) {
                assertNull("Should not get here", coder);
            }

            @Override
            public void errorOnCoderStart(VideoAvcCoder coder, String errorDescription) {
                assertNull("Should not get here", errorDescription);
            }

            @Override
            public void coderShouldStop(VideoAvcCoder videoAvcCoder) {
                assertNull("Should not get here", videoAvcCoder);
            }

            @Override
            public void coderStopped(VideoAvcCoder videoAvcCoder) {
                assertNotNull("Should force stop coder", videoAvcCoder);
            }

            @Override
            public void errorOnCoderStop(VideoAvcCoder videoAvcCoder, String errorMessage) {
                assertNull("Should not get here", videoAvcCoder);
            }
        });
        videoAvcCoder.forceStop();
    }

    public void testVideoEncoderShouldFailOnStop() throws Exception {
        final VideoAvcCoder videoAvcCoder = new VideoAvcCoder(null,  null, CamcorderProfile.QUALITY_LOW, new VideoAvcCoderListener() {
            @Override
            public void coderStarted(VideoAvcCoder coder) {
                assertNull("Should not get here", coder);
            }

            @Override
            public void errorOnCoderStart(VideoAvcCoder coder, String errorDescription) {
                assertNull("Should not get here", errorDescription);
            }

            @Override
            public void coderShouldStop(VideoAvcCoder videoAvcCoder) {
                assertNull("Should not get here", videoAvcCoder);
            }

            @Override
            public void coderStopped(VideoAvcCoder videoAvcCoder) {
                assertNull("Should not get here", videoAvcCoder);
            }

            @Override
            public void errorOnCoderStop(VideoAvcCoder videoAvcCoder, String errorMessage) {
                assertNotNull("Should get error since codec not properly initialised", videoAvcCoder);
            }
        });
        videoAvcCoder.forceStop();
    }
}
