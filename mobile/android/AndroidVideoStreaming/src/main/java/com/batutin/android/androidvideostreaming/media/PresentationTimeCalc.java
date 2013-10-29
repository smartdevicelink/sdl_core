package com.batutin.android.androidvideostreaming.media;

public class PresentationTimeCalc {


    private int frameRate;

    public PresentationTimeCalc(int frameRate) {
        if (frameRate < 1) {
            throw new IllegalArgumentException("frame rate must be >= 1");
        }
        this.frameRate = frameRate;
    }

    /**
     * Generates the presentation time for frame N, in microseconds.
     */
    public long computePresentationTime(int frameIndex) {
        if (frameIndex < 0) return 0;
        return frameIndex * 1000000 / frameRate;
    }
}