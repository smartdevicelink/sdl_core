package com.ford.syncV4.protocol;

import junit.framework.Assert;
import junit.framework.TestCase;

public class ProtocolFrameHeaderTest extends TestCase {
    public void testParseDataSizeZero() {
        final byte[] bytes =
                { 0x20, 0x07, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00 };
        ProtocolFrameHeader header =
                ProtocolFrameHeader.parseWiProHeader(bytes);
        Assert.assertEquals("Parsed data size is incorrect", 0,
                header.getDataSize());
    }

    public void testParseDataSizeOne() {
        final byte[] bytes =
                { 0x20, 0x07, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
                        0x00, 0x00 };
        ProtocolFrameHeader header =
                ProtocolFrameHeader.parseWiProHeader(bytes);
        Assert.assertEquals("Parsed data size is incorrect", 1,
                header.getDataSize());
    }

    public void testParseDataSizeMax() {
        final byte maxByte = (byte) 0xFF;
        final byte[] bytes =
                { 0x20, 0x07, 0x01, 0x01, maxByte, maxByte, maxByte, maxByte,
                        0x00, 0x00, 0x00, 0x00 };
        ProtocolFrameHeader header =
                ProtocolFrameHeader.parseWiProHeader(bytes);
        Assert.assertEquals("Parsed data size is incorrect", 0xFFFFFFFF,
                header.getDataSize());
    }
}
