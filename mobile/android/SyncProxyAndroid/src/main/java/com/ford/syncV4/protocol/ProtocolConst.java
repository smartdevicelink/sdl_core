package com.ford.syncV4.protocol;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 12.03.14
 * Time: 13:22
 */

/**
 * Helper class to hold all constants which are belongs to protocol
 */
public class ProtocolConst {

    public static final int PROTOCOL_V2_HEADER_SIZE = 12;

    public static final byte PROTECT_SERVICE_REQUEST_ID = (byte) 0x01;
    public static final byte PROTECT_SERVICE_RESPONSE_ID = (byte) 0x02;
}