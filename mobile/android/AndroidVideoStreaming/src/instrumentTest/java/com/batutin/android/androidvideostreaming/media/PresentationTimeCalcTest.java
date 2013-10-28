package com.batutin.android.androidvideostreaming.media;

import android.test.AndroidTestCase;

/**
 * Created by Andrew Batutin on 8/12/13.
 */
public class PresentationTimeCalcTest extends AndroidTestCase {


    PresentationTimeCalc sut;

    public PresentationTimeCalcTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        sut = new PresentationTimeCalc(10);
    }

    public void testPresentationTimeIsNullForZeroIndex() throws Exception {
        long pt = sut.computePresentationTime(0);
        assertEquals(0, pt);
    }

    public void testPresentationTimeCalcInitWithNegativeFrameIndexShouldThrow() throws Exception {
        try {
            PresentationTimeCalc pc = new PresentationTimeCalc(-1);
            assertFalse("should not get here", true);
        } catch (IllegalArgumentException e) {
            assertNotNull(e);
        }
    }

    public void testPresentationTimeEqual100000() throws Exception {
        long pt = sut.computePresentationTime(1);
        assertEquals(100000, pt);
    }

    public void testPresentationTimeForNegativeIndexEqual0() throws Exception {
        long pt = sut.computePresentationTime(-1);
        assertEquals(0, pt);
    }
}
