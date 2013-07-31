/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.batutin.android.androidvideostreaming.screenshot;

import android.content.res.AssetFileDescriptor;
import android.content.res.Resources;
import android.media.MediaCodec;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.util.Log;
import android.view.Surface;

import com.batutin.android.androidvideostreaming.R;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.zip.CRC32;

public class DecoderTest extends MediaPlayerTestBase {
    private static final String TAG = "DecoderTest";

    private Resources mResources;
    short[] mMasterBuffer;

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        mResources = mContext.getResources();

        // read master file into memory
        AssetFileDescriptor masterFd = mResources.openRawResourceFd(R.raw.sinesweepraw);
        long masterLength = masterFd.getLength();
        mMasterBuffer = new short[(int) (masterLength / 2)];
        InputStream is = masterFd.createInputStream();
        BufferedInputStream bis = new BufferedInputStream(is);
        for (int i = 0; i < mMasterBuffer.length; i++) {
            int lo = bis.read();
            int hi = bis.read();
            if (hi >= 128) {
                hi -= 256;
            }
            int sample = hi * 256 + lo;
            mMasterBuffer[i] = (short) sample;
        }
        bis.close();
        masterFd.close();
    }

    // The allowed errors in the following tests are the actual maximum measured
    // errors with the standard decoders, plus 10%.
    // This should allow for some variation in decoders, while still detecting
    // phase and delay errors, channel swap, etc.
    public void testDecodeMp3Lame() throws Exception {
        decode(R.raw.sinesweepmp3lame, 804.f, true);
    }
    public void testDecodeMp3Smpb() throws Exception {
        decode(R.raw.sinesweepmp3smpb, 413.f, true);
    }
    public void testDecodeM4a() throws Exception {
        decode(R.raw.sinesweepm4a, 124.f, true);
    }
    public void testDecodeOgg() throws Exception {
        decode(R.raw.sinesweepogg, 168.f, false);
    }
    public void testDecodeWav() throws Exception {
        decode(R.raw.sinesweepwav, 0.0f, true);
    }
    public void testDecodeFlac() throws Exception {
        decode(R.raw.sinesweepflac, 0.0f, true);
    }

    /**
     * @param testinput the file to decode
     * @param maxerror the maximum allowed root mean squared error
     * @param reconfigure whether to also test reconfiguring the codec
     * @throws java.io.IOException
     */
    private void decode(int testinput, float maxerror, boolean reconfigure) throws IOException {

        short [] decoded = decodeToMemory(testinput, false);

        assertEquals("wrong data size", mMasterBuffer.length, decoded.length);

        long totalErrorSquared = 0;

        for (int i = 0; i < decoded.length; i++) {
            short sample = decoded[i];
            short mastersample = mMasterBuffer[i];
            int d = sample - mastersample;
            totalErrorSquared += d * d;
        }

        long avgErrorSquared = (totalErrorSquared / decoded.length);
        double rmse = Math.sqrt(avgErrorSquared);
        assertTrue("decoding error too big: " + rmse, rmse <= maxerror);

        if (reconfigure) {
            short [] decoded2 = decodeToMemory(testinput, true);
            assertEquals("count different with reconfigure", decoded.length, decoded2.length);
            for (int i = 0; i < decoded.length; i++) {
                assertEquals("samples don't match", decoded[i], decoded2[i]);
            }
        }
    }

    private short[] decodeToMemory(int testinput, boolean reconfigure) throws IOException {

        short [] decoded = new short[0];
        int decodedIdx = 0;

        AssetFileDescriptor testFd = mResources.openRawResourceFd(testinput);

        MediaExtractor extractor;
        MediaCodec codec;
        ByteBuffer[] codecInputBuffers;
        ByteBuffer[] codecOutputBuffers;

        extractor = new MediaExtractor();
        extractor.setDataSource(testFd.getFileDescriptor(), testFd.getStartOffset(),
                testFd.getLength());
        testFd.close();

        assertEquals("wrong number of tracks", 1, extractor.getTrackCount());
        MediaFormat format = extractor.getTrackFormat(0);
        String mime = format.getString(MediaFormat.KEY_MIME);
        assertTrue("not an audio file", mime.startsWith("audio/"));

        codec = MediaCodec.createDecoderByType(mime);
        codec.configure(format, null /* surface */, null /* crypto */, 0 /* flags */);
        codec.start();
        codecInputBuffers = codec.getInputBuffers();
        codecOutputBuffers = codec.getOutputBuffers();

        if (reconfigure) {
            codec.stop();
            codec.configure(format, null /* surface */, null /* crypto */, 0 /* flags */);
            codec.start();
            codecInputBuffers = codec.getInputBuffers();
            codecOutputBuffers = codec.getOutputBuffers();
        }

        extractor.selectTrack(0);

        // start decoding
        final long kTimeOutUs = 5000;
        MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
        boolean sawInputEOS = false;
        boolean sawOutputEOS = false;
        int noOutputCounter = 0;
        while (!sawOutputEOS && noOutputCounter < 50) {
            noOutputCounter++;
            if (!sawInputEOS) {
                int inputBufIndex = codec.dequeueInputBuffer(kTimeOutUs);

                if (inputBufIndex >= 0) {
                    ByteBuffer dstBuf = codecInputBuffers[inputBufIndex];

                    int sampleSize =
                        extractor.readSampleData(dstBuf, 0 /* offset */);

                    long presentationTimeUs = 0;

                    if (sampleSize < 0) {
                        Log.d(TAG, "saw input EOS.");
                        sawInputEOS = true;
                        sampleSize = 0;
                    } else {
                        presentationTimeUs = extractor.getSampleTime();
                    }

                    codec.queueInputBuffer(
                            inputBufIndex,
                            0 /* offset */,
                            sampleSize,
                            presentationTimeUs,
                            sawInputEOS ? MediaCodec.BUFFER_FLAG_END_OF_STREAM : 0);

                    if (!sawInputEOS) {
                        extractor.advance();
                    }
                }
            }

            int res = codec.dequeueOutputBuffer(info, kTimeOutUs);

            if (res >= 0) {
                //Log.d(TAG, "got frame, size " + info.size + "/" + info.presentationTimeUs);

                if (info.size > 0) {
                    noOutputCounter = 0;
                }
                if (info.size > 0 && reconfigure) {
                    // once we've gotten some data out of the decoder, reconfigure it again
                    reconfigure = false;
                    extractor.seekTo(0, MediaExtractor.SEEK_TO_NEXT_SYNC);
                    sawInputEOS = false;
                    codec.stop();
                    codec.configure(format, null /* surface */, null /* crypto */, 0 /* flags */);
                    codec.start();
                    codecInputBuffers = codec.getInputBuffers();
                    codecOutputBuffers = codec.getOutputBuffers();
                    continue;
                }

                int outputBufIndex = res;
                ByteBuffer buf = codecOutputBuffers[outputBufIndex];

                if (decodedIdx + (info.size / 2) >= decoded.length) {
                    decoded = Arrays.copyOf(decoded, decodedIdx + (info.size / 2));
                }

                for (int i = 0; i < info.size; i += 2) {
                    decoded[decodedIdx++] = buf.getShort(i);
                }

                codec.releaseOutputBuffer(outputBufIndex, false /* render */);

                if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                    Log.d(TAG, "saw output EOS.");
                    sawOutputEOS = true;
                }
            } else if (res == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                codecOutputBuffers = codec.getOutputBuffers();

                Log.d(TAG, "output buffers have changed.");
            } else if (res == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                MediaFormat oformat = codec.getOutputFormat();

                Log.d(TAG, "output format has changed to " + oformat);
            } else {
                Log.d(TAG, "dequeueOutputBuffer returned " + res);
            }
        }

        codec.stop();
        codec.release();
        return decoded;
    }

    public void testCodecBasicH264() throws Exception {
        Surface s = getActivity().getSurfaceHolder().getSurface();
        int frames1 = countFrames(
                R.raw.video_480x360_mp4_h264_1000kbps_25fps_aac_stereo_128kbps_44100hz,
                false, -1, s);
        assertEquals("wrong number of frames decoded", 240, frames1);

        int frames2 = countFrames(
                R.raw.video_480x360_mp4_h264_1000kbps_25fps_aac_stereo_128kbps_44100hz,
                false, -1, null);
        assertEquals("different number of frames when using Surface", frames1, frames2);
    }

    public void testCodecBasicH263() throws Exception {
        Surface s = getActivity().getSurfaceHolder().getSurface();
        int frames1 = countFrames(
                R.raw.video_176x144_3gp_h263_300kbps_12fps_aac_stereo_128kbps_22050hz,
                false /* reconfigure */, -1 /* eosframe */, s);
        assertEquals("wrong number of frames decoded", 122, frames1);

        int frames2 = countFrames(
                R.raw.video_176x144_3gp_h263_300kbps_12fps_aac_stereo_128kbps_22050hz,
                false /* reconfigure */, -1 /* eosframe */, null);
        assertEquals("different number of frames when using Surface", frames1, frames2);
    }

    public void testCodecEarlyEOSH263() throws Exception {
        Surface s = getActivity().getSurfaceHolder().getSurface();
        int frames1 = countFrames(
                R.raw.video_176x144_3gp_h263_300kbps_12fps_aac_stereo_128kbps_22050hz,
                false, 64, s);
        assertEquals("wrong number of frames decoded", 64, frames1);
    }

    public void testCodecEarlyEOSH264() throws Exception {
        Surface s = getActivity().getSurfaceHolder().getSurface();
        int frames1 = countFrames(
                R.raw.video_480x360_mp4_h264_1000kbps_25fps_aac_stereo_128kbps_44100hz,
                false, 120, s);
        assertEquals("wrong number of frames decoded", 120, frames1);
    }

    public void testCodecReconfigH264WithoutSurface() throws Exception {
        testCodecReconfig(
                R.raw.video_480x360_mp4_h264_1000kbps_25fps_aac_stereo_128kbps_44100hz, null);
    }

    public void testCodecReconfigH264WithSurface() throws Exception {
        Surface s = getActivity().getSurfaceHolder().getSurface();
        testCodecReconfig(
                R.raw.video_480x360_mp4_h264_1000kbps_25fps_aac_stereo_128kbps_44100hz, s);
    }

    public void testCodecReconfigH263WithoutSurface() throws Exception {
        testCodecReconfig(
                R.raw.video_176x144_3gp_h263_300kbps_12fps_aac_stereo_128kbps_22050hz, null);
    }

    public void testCodecReconfigH263WithSurface() throws Exception {
        Surface s = getActivity().getSurfaceHolder().getSurface();
        testCodecReconfig(
                R.raw.video_176x144_3gp_h263_300kbps_12fps_aac_stereo_128kbps_22050hz, s);
    }

//    public void testCodecReconfigOgg() throws Exception {
//        testCodecReconfig(R.raw.sinesweepogg, null);
//    }
//
    public void testCodecReconfigMp3() throws Exception {
        testCodecReconfig(R.raw.sinesweepmp3lame, null);
    }

    public void testCodecReconfigM4a() throws Exception {
        testCodecReconfig(R.raw.sinesweepm4a, null);
    }

    private void testCodecReconfig(int video, Surface s) throws Exception {
        int frames1 = countFrames(video, false /* reconfigure */, -1 /* eosframe */, s);
        int frames2 = countFrames(video, true /* reconfigure */, -1 /* eosframe */, s);
        assertEquals("different number of frames when reusing codec", frames1, frames2);
    }

    private int countFrames(int video, boolean reconfigure, int eosframe, Surface s) throws Exception {
        int numframes = 0;

        AssetFileDescriptor testFd = mResources.openRawResourceFd(video);

        MediaExtractor extractor;
        MediaCodec codec = null;
        ByteBuffer[] codecInputBuffers;
        ByteBuffer[] codecOutputBuffers;

        extractor = new MediaExtractor();
        extractor.setDataSource(testFd.getFileDescriptor(), testFd.getStartOffset(),
                testFd.getLength());

        MediaFormat format = extractor.getTrackFormat(0);
        String mime = format.getString(MediaFormat.KEY_MIME);
        boolean isAudio = mime.startsWith("audio/");

        codec = MediaCodec.createDecoderByType(mime);
//        if (mime.contains("avc")) {
//            codec = MediaCodec.createByCodecName("OMX.google.h264.decoder");
//        } else if (mime.contains("3gpp")) {
//            codec = MediaCodec.createByCodecName("OMX.google.h263.decoder");
//        }
        assertNotNull("couldn't find codec", codec);
        Log.i("@@@@", "using codec: " + codec.toString());
        codec.configure(format, s /* surface */, null /* crypto */, 0 /* flags */);
        codec.start();
        codecInputBuffers = codec.getInputBuffers();
        codecOutputBuffers = codec.getOutputBuffers();

        if (reconfigure) {
            codec.stop();
            codec.configure(format, s /* surface */, null /* crypto */, 0 /* flags */);
            codec.start();
            codecInputBuffers = codec.getInputBuffers();
            codecOutputBuffers = codec.getOutputBuffers();
        }
        Log.i("@@@@", "format: " + format);

        extractor.selectTrack(0);

        // start decoding
        final long kTimeOutUs = 5000;
        MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
        boolean sawInputEOS = false;
        boolean sawOutputEOS = false;
        int deadDecoderCounter = 0;
        int samplecounter = 0;
        ArrayList<Long> timestamps = new ArrayList<Long>();
        while (!sawOutputEOS && deadDecoderCounter < 100) {
            if (!sawInputEOS) {
                int inputBufIndex = codec.dequeueInputBuffer(kTimeOutUs);

                if (inputBufIndex >= 0) {
                    ByteBuffer dstBuf = codecInputBuffers[inputBufIndex];

                    int sampleSize =
                        extractor.readSampleData(dstBuf, 0 /* offset */);

                    long presentationTimeUs = 0;

                    if (sampleSize < 0) {
                        Log.d(TAG, "saw input EOS.");
                        sawInputEOS = true;
                        sampleSize = 0;
                    } else {
                        presentationTimeUs = extractor.getSampleTime();
                        samplecounter++;
                        if (samplecounter == eosframe) {
                            sawInputEOS = true;
                        }
                    }

                    timestamps.add(presentationTimeUs);

                    int flags = extractor.getSampleFlags();

                    codec.queueInputBuffer(
                            inputBufIndex,
                            0 /* offset */,
                            sampleSize,
                            presentationTimeUs,
                            sawInputEOS ? MediaCodec.BUFFER_FLAG_END_OF_STREAM : 0);

                    if (!sawInputEOS) {
                        extractor.advance();
                    }
                }
            }

            int res = codec.dequeueOutputBuffer(info, kTimeOutUs);

            deadDecoderCounter++;
            if (res >= 0) {
                //Log.d(TAG, "got frame, size " + info.size + "/" + info.presentationTimeUs);

                // Some decoders output a 0-sized buffer at the end. Disregard those.
                if (info.size > 0) {
                    deadDecoderCounter = 0;
                    if (reconfigure) {
                        // once we've gotten some data out of the decoder, reconfigure it again
                        reconfigure = false;
                        numframes = 0;
                        extractor.seekTo(0, MediaExtractor.SEEK_TO_NEXT_SYNC);
                        sawInputEOS = false;
                        timestamps.clear();
                        codec.stop();
                        codec.configure(format, s /* surface */, null /* crypto */, 0 /* flags */);
                        codec.start();
                        codecInputBuffers = codec.getInputBuffers();
                        codecOutputBuffers = codec.getOutputBuffers();
                        continue;
                    }

                    if (isAudio) {
                        // for audio, count the number of bytes that were decoded, not the number
                        // of access units
                        numframes += info.size;
                    } else {
                        // for video, count the number of video frames and check the timestamp
                        numframes++;
                        assertTrue("invalid timestamp", timestamps.remove(info.presentationTimeUs));
                    }
                }
                int outputBufIndex = res;
                codec.releaseOutputBuffer(outputBufIndex, true /* render */);

                if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                    Log.d(TAG, "saw output EOS.");
                    sawOutputEOS = true;
                }
            } else if (res == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                codecOutputBuffers = codec.getOutputBuffers();

                Log.d(TAG, "output buffers have changed.");
            } else if (res == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                MediaFormat oformat = codec.getOutputFormat();

                Log.d(TAG, "output format has changed to " + oformat);
            } else {
                Log.d(TAG, "no output");
            }
        }

        codec.stop();
        codec.release();
        testFd.close();
        return numframes;
    }

    public void testEOSBehaviorH264() throws Exception {
        // this video has an I frame at 44
        testEOSBehavior(R.raw.video_480x360_mp4_h264_1000kbps_25fps_aac_stereo_128kbps_44100hz, 44);
        testEOSBehavior(R.raw.video_480x360_mp4_h264_1000kbps_25fps_aac_stereo_128kbps_44100hz, 45);
        testEOSBehavior(R.raw.video_480x360_mp4_h264_1000kbps_25fps_aac_stereo_128kbps_44100hz, 55);
    }

    public void testEOSBehaviorH263() throws Exception {
        // this video has an I frame every 12 frames.
        testEOSBehavior(R.raw.video_176x144_3gp_h263_300kbps_12fps_aac_stereo_128kbps_22050hz, 24);
        testEOSBehavior(R.raw.video_176x144_3gp_h263_300kbps_12fps_aac_stereo_128kbps_22050hz, 25);
        testEOSBehavior(R.raw.video_176x144_3gp_h263_300kbps_12fps_aac_stereo_128kbps_22050hz, 48);
        testEOSBehavior(R.raw.video_176x144_3gp_h263_300kbps_12fps_aac_stereo_128kbps_22050hz, 50);
    }

    private void testEOSBehavior(int movie, int stopatsample) throws Exception {

        int numframes = 0;

        long [] checksums = new long[stopatsample];

        AssetFileDescriptor testFd = mResources.openRawResourceFd(movie);

        MediaExtractor extractor;
        MediaCodec codec = null;
        ByteBuffer[] codecInputBuffers;
        ByteBuffer[] codecOutputBuffers;

        extractor = new MediaExtractor();
        extractor.setDataSource(testFd.getFileDescriptor(), testFd.getStartOffset(),
                testFd.getLength());

        MediaFormat format = extractor.getTrackFormat(0);
        String mime = format.getString(MediaFormat.KEY_MIME);
        boolean isAudio = mime.startsWith("audio/");

        codec = MediaCodec.createDecoderByType(mime);
//        if (mime.contains("avc")) {
//            codec = MediaCodec.createByCodecName("OMX.google.h264.decoder");
//        } else if (mime.contains("3gpp")) {
//            codec = MediaCodec.createByCodecName("OMX.google.h263.decoder");
//        }
        assertNotNull("couldn't find codec", codec);
        Log.i("@@@@", "using codec: " + codec.toString());
        codec.configure(format, null /* surface */, null /* crypto */, 0 /* flags */);
        codec.start();
        codecInputBuffers = codec.getInputBuffers();
        codecOutputBuffers = codec.getOutputBuffers();

        extractor.selectTrack(0);

        // start decoding
        final long kTimeOutUs = 5000;
        MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
        boolean sawInputEOS = false;
        boolean sawOutputEOS = false;
        int deadDecoderCounter = 0;
        int samplenum = 0;
        while (!sawOutputEOS && deadDecoderCounter < 100) {
            if (!sawInputEOS) {
                int inputBufIndex = codec.dequeueInputBuffer(kTimeOutUs);

                if (inputBufIndex >= 0) {
                    ByteBuffer dstBuf = codecInputBuffers[inputBufIndex];

                    int sampleSize =
                        extractor.readSampleData(dstBuf, 0 /* offset */);
//                    Log.i("@@@@", "read sample " + samplenum + ":" + extractor.getSampleFlags()
//                            + " @ " + extractor.getSampleTime() + " size " + sampleSize);
                    samplenum++;

                    long presentationTimeUs = 0;

                    if (sampleSize < 0 || samplenum >= (stopatsample + 100)) {
                        Log.d(TAG, "saw input EOS.");
                        sawInputEOS = true;
                        sampleSize = 0;
                    } else {
                        presentationTimeUs = extractor.getSampleTime();
                    }

                    int flags = extractor.getSampleFlags();

                    codec.queueInputBuffer(
                            inputBufIndex,
                            0 /* offset */,
                            sampleSize,
                            presentationTimeUs,
                            sawInputEOS ? MediaCodec.BUFFER_FLAG_END_OF_STREAM : 0);

                    if (!sawInputEOS) {
                        extractor.advance();
                    }
                }
            }

            int res = codec.dequeueOutputBuffer(info, kTimeOutUs);

            deadDecoderCounter++;
            if (res >= 0) {

                // Some decoders output a 0-sized buffer at the end. Disregard those.
                if (info.size > 0) {
                    deadDecoderCounter = 0;

                    if (isAudio) {
                        // for audio, count the number of bytes that were decoded, not the number
                        // of access units
                        numframes += info.size;
                    } else {
                        // for video, count the number of video frames
                        long sum = checksum(codecOutputBuffers[res], info.size);
                        if (numframes < checksums.length) {
                            checksums[numframes] = sum;
                        }
                        numframes++;
                    }
                }
//                Log.d(TAG, "got frame, size " + info.size + "/" + info.presentationTimeUs +
//                        "/" + numframes + "/" + info.flags);

                int outputBufIndex = res;
                codec.releaseOutputBuffer(outputBufIndex, true /* render */);

                if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                    Log.d(TAG, "saw output EOS.");
                    sawOutputEOS = true;
                }
            } else if (res == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                codecOutputBuffers = codec.getOutputBuffers();

                Log.d(TAG, "output buffers have changed.");
            } else if (res == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                MediaFormat oformat = codec.getOutputFormat();

                Log.d(TAG, "output format has changed to " + oformat);
            } else {
                Log.d(TAG, "no output");
            }
        }

        codec.stop();
        codec.release();
        extractor.release();


        // We now have checksums for every frame.
        // Now decode again, but signal EOS right before an index frame, and ensure the frames
        // prior to that are the same.

        extractor = new MediaExtractor();
        extractor.setDataSource(testFd.getFileDescriptor(), testFd.getStartOffset(),
                testFd.getLength());

        codec = MediaCodec.createDecoderByType(mime);
//        if (mime.contains("avc")) {
//            codec = MediaCodec.createByCodecName("OMX.google.h264.decoder");
//        } else if (mime.contains("3gpp")) {
//            codec = MediaCodec.createByCodecName("OMX.google.h263.decoder");
//        }
        codec.configure(format, null /* surface */, null /* crypto */, 0 /* flags */);
        codec.start();
        codecInputBuffers = codec.getInputBuffers();
        codecOutputBuffers = codec.getOutputBuffers();

        extractor.selectTrack(0);

        // start decoding
        info = new MediaCodec.BufferInfo();
        sawInputEOS = false;
        sawOutputEOS = false;
        deadDecoderCounter = 0;
        samplenum = 0;
        numframes = 0;
        while (!sawOutputEOS && deadDecoderCounter < 100) {
            if (!sawInputEOS) {
                int inputBufIndex = codec.dequeueInputBuffer(kTimeOutUs);

                if (inputBufIndex >= 0) {
                    ByteBuffer dstBuf = codecInputBuffers[inputBufIndex];

                    int sampleSize =
                        extractor.readSampleData(dstBuf, 0 /* offset */);
//                    Log.i("@@@@", "read sample " + samplenum + ":" + extractor.getSampleFlags()
//                            + " @ " + extractor.getSampleTime() + " size " + sampleSize);
                    samplenum++;

                    long presentationTimeUs = extractor.getSampleTime();

                    if (sampleSize < 0 || samplenum >= stopatsample) {
                        Log.d(TAG, "saw input EOS.");
                        sawInputEOS = true;
                        if (sampleSize < 0) {
                            sampleSize = 0;
                        }
                    }

                    int flags = extractor.getSampleFlags();

                    codec.queueInputBuffer(
                            inputBufIndex,
                            0 /* offset */,
                            sampleSize,
                            presentationTimeUs,
                            sawInputEOS ? MediaCodec.BUFFER_FLAG_END_OF_STREAM : 0);

                    if (!sawInputEOS) {
                        extractor.advance();
                    }
                }
            }

            int res = codec.dequeueOutputBuffer(info, kTimeOutUs);

            deadDecoderCounter++;
            if (res >= 0) {

                // Some decoders output a 0-sized buffer at the end. Disregard those.
                if (info.size > 0) {
                    deadDecoderCounter = 0;

                    if (isAudio) {
                        // for audio, count the number of bytes that were decoded, not the number
                        // of access units
                        numframes += info.size;
                    } else {
                        // for video, count the number of video frames
                        long sum = checksum(codecOutputBuffers[res], info.size);
                        if (numframes < checksums.length) {
                            assertEquals("frame data mismatch at frame " + numframes,
                                    checksums[numframes], sum);
                        }
                        numframes++;
                    }
                }
//                Log.d(TAG, "got frame, size " + info.size + "/" + info.presentationTimeUs +
//                        "/" + numframes + "/" + info.flags);

                int outputBufIndex = res;
                codec.releaseOutputBuffer(outputBufIndex, true /* render */);

                if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                    Log.d(TAG, "saw output EOS.");
                    sawOutputEOS = true;
                }
            } else if (res == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                codecOutputBuffers = codec.getOutputBuffers();

                Log.d(TAG, "output buffers have changed.");
            } else if (res == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                MediaFormat oformat = codec.getOutputFormat();

                Log.d(TAG, "output format has changed to " + oformat);
            } else {
                Log.d(TAG, "no output");
            }
        }

        codec.stop();
        codec.release();
        extractor.release();

        assertEquals("I!=O", samplenum, numframes);
        assertTrue("last frame didn't have EOS", sawOutputEOS);
        assertEquals(stopatsample, numframes);

        testFd.close();
    }

    private long checksum(ByteBuffer buf, int size) {
        assertTrue(size != 0);
        assertTrue(size <= buf.capacity());
        CRC32 crc = new CRC32();
        int pos = buf.position();
        buf.rewind();
        for (int i = 0; i < buf.capacity(); i++) {
            crc.update(buf.get());
        }
        buf.position(pos);
        return crc.getValue();
    }

    public void testFlush() throws Exception {
        testFlush(R.raw.loudsoftwav);
        testFlush(R.raw.loudsoftogg);
        testFlush(R.raw.loudsoftmp3);
        testFlush(R.raw.loudsoftaac);
        testFlush(R.raw.loudsoftfaac);
        testFlush(R.raw.loudsoftitunes);
    }

    private void testFlush(int resource) throws Exception {

        AssetFileDescriptor testFd = mResources.openRawResourceFd(resource);

        MediaExtractor extractor;
        MediaCodec codec;
        ByteBuffer[] codecInputBuffers;
        ByteBuffer[] codecOutputBuffers;

        extractor = new MediaExtractor();
        extractor.setDataSource(testFd.getFileDescriptor(), testFd.getStartOffset(),
                testFd.getLength());
        testFd.close();

        assertEquals("wrong number of tracks", 1, extractor.getTrackCount());
        MediaFormat format = extractor.getTrackFormat(0);
        String mime = format.getString(MediaFormat.KEY_MIME);
        assertTrue("not an audio file", mime.startsWith("audio/"));

        codec = MediaCodec.createDecoderByType(mime);
        codec.configure(format, null /* surface */, null /* crypto */, 0 /* flags */);
        codec.start();
        codecInputBuffers = codec.getInputBuffers();
        codecOutputBuffers = codec.getOutputBuffers();

        extractor.selectTrack(0);

        // decode a bit of the first part of the file, and verify the amplitude
        short maxvalue1 = getAmplitude(extractor, codec);

        // flush the codec and seek the extractor a different position, then decode a bit more
        // and check the amplitude
        extractor.seekTo(8000000, 0);
        codec.flush();
        short maxvalue2 = getAmplitude(extractor, codec);

        assertTrue("first section amplitude too low", maxvalue1 > 20000);
        assertTrue("second section amplitude too high", maxvalue2 < 5000);
        codec.stop();
        codec.release();

    }
    
    private short getAmplitude(MediaExtractor extractor, MediaCodec codec) {
        short maxvalue = 0;
        int numBytesDecoded = 0;
        final long kTimeOutUs = 5000;
        ByteBuffer[] codecInputBuffers = codec.getInputBuffers();
        ByteBuffer[] codecOutputBuffers = codec.getOutputBuffers();
        MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();

        while(numBytesDecoded < 44100 * 2) {
            int inputBufIndex = codec.dequeueInputBuffer(kTimeOutUs);

            if (inputBufIndex >= 0) {
                ByteBuffer dstBuf = codecInputBuffers[inputBufIndex];

                int sampleSize = extractor.readSampleData(dstBuf, 0 /* offset */);
                long presentationTimeUs = extractor.getSampleTime();

                codec.queueInputBuffer(
                        inputBufIndex,
                        0 /* offset */,
                        sampleSize,
                        presentationTimeUs,
                        0 /* flags */);

                extractor.advance();
            }
            int res = codec.dequeueOutputBuffer(info, kTimeOutUs);

            if (res >= 0) {

                int outputBufIndex = res;
                ByteBuffer buf = codecOutputBuffers[outputBufIndex];

                for (int i = 0; i < info.size; i += 2) {
                    short sample = buf.getShort(i);
                    if (maxvalue < sample) {
                        maxvalue = sample;
                    }
                    int idx = (numBytesDecoded + i) / 2;
                }

                numBytesDecoded += info.size;

                codec.releaseOutputBuffer(outputBufIndex, false /* render */);
            } else if (res == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                codecOutputBuffers = codec.getOutputBuffers();
            } else if (res == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                MediaFormat oformat = codec.getOutputFormat();
            }
        }
        return maxvalue; 
    }
    
}

