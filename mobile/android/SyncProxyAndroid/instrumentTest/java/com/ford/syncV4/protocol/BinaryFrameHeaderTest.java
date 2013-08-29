package com.ford.syncV4.protocol;

import junit.framework.Assert;
import junit.framework.TestCase;

public class BinaryFrameHeaderTest extends TestCase {
    public void testParseJsonSizeZero() {
        final byte[] bytes =
                { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00 };
        BinaryFrameHeader header = BinaryFrameHeader.parseBinaryHeader(bytes);
        Assert.assertEquals("Parsed json size is incorrect", 0,
                header.getJsonSize());
    }

    public void testParseJsonSizeOne() {
        final byte[] bytes =
                { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x01, 0x00 };
        BinaryFrameHeader header = BinaryFrameHeader.parseBinaryHeader(bytes);
        Assert.assertEquals("Parsed json size is incorrect", 1,
                header.getJsonSize());
    }

    public void testParseJsonSizeHalfMax() {
        final byte maxByte = (byte) 0xFF;
        final byte[] bytes =
                { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F,
                        maxByte, maxByte, maxByte, 0x00 };
        BinaryFrameHeader header = BinaryFrameHeader.parseBinaryHeader(bytes);
        Assert.assertEquals("Parsed json size is incorrect", 0x7FFFFFFF,
                header.getJsonSize());
    }
}
