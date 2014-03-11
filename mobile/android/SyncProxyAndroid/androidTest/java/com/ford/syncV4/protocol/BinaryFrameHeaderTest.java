package com.ford.syncV4.protocol;

import junit.framework.Assert;
import junit.framework.TestCase;

public class BinaryFrameHeaderTest extends TestCase {
    public void testParseJsonSizeZeroShouldReturnCorrectJsonSize() {
        final byte[] bytes =
                { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00 };
        BinaryFrameHeader header = BinaryFrameHeader.parseBinaryHeader(bytes);
        Assert.assertEquals("Parsed json size is incorrect", 0,
                header.getJsonSize());
    }

    public void testParseJsonSizeOneShouldReturnCorrectSize() {
        final byte[] bytes =
                { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x01, 0x00 };
        BinaryFrameHeader header = BinaryFrameHeader.parseBinaryHeader(bytes);
        Assert.assertEquals("Parsed json size is incorrect", 1,
                header.getJsonSize());
    }

    public void testParseJsonSizeHalfMaxShouldThrowOutOfMemory() {
        try {
            final byte maxByte = (byte) 0xFF;
            final byte[] bytes =
                    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F,
                            maxByte, maxByte, maxByte };
            BinaryFrameHeader.parseBinaryHeader(bytes);
            fail("Should have thrown OutOfMemoryError");
        } catch (OutOfMemoryError e) {
            // success
        }
    }

    public void testParseJsonSizeHalfMaxPlusOneShouldThrowOutOfMemory() {
        // this 0x80000000 number is also the first negative number after
        // 0x7FFFFFFF
        try {
            final byte[] bytes =
                    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            (byte) 0x80, 0x00, 0x00, 0x00 };
            BinaryFrameHeader.parseBinaryHeader(bytes);
            fail("Should have thrown OutOfMemoryError");
        } catch (OutOfMemoryError e) {
            // success
        }
    }

    public void testParseJsonSizeMaxShouldThrowOutOfMemory() {
        try {
            final byte maxByte = (byte) 0xFF;
            final byte[] bytes =
                    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, maxByte,
                            maxByte, maxByte, maxByte, 0x00 };
            BinaryFrameHeader.parseBinaryHeader(bytes);
            fail("Should have thrown OutOfMemoryError");
        } catch (OutOfMemoryError e) {
            // success
        }
    }
}
