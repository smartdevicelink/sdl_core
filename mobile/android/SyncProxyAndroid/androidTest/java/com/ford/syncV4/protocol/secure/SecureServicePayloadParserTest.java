package com.ford.syncV4.protocol.secure;

import com.ford.syncV4.util.BitConverter;

import junit.framework.TestCase;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 07.03.14
 * Time: 16:40
 */
public class SecureServicePayloadParserTest extends TestCase {

    private static final String START_SECURE_AUDIO_SERVICE_RESPONSE_HEX = "2110000100000025" +
            "0000001920ff00000000000000000000556e6b6e6f776e207175657279206964656e7469666965722e";

    public void testParse() {
        byte[] data = BitConverter.hexToBytes(START_SECURE_AUDIO_SERVICE_RESPONSE_HEX);

        SecureServicePayloadParser parser = new SecureServicePayloadParser();
        parser.parse(data);
    }
}