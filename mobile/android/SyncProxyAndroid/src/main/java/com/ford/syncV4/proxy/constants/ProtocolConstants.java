package com.ford.syncV4.proxy.constants;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 01.04.14
 * Time: 16:02
 */

/**
 * This class holds all constants necessary for the protocol
 */
public class ProtocolConstants {

    /**
     * Define Protocol minimum support version
     */
    public static final byte PROTOCOL_VERSION_MIN = (byte) 1;

    /**
     * Define Protocol maximum support version
     */
    public static final byte PROTOCOL_VERSION_MAX = (byte) 2;

    /**
     * Define Protocol default support version
     */
    public static final byte PROTOCOL_VERSION_DEFAULT = PROTOCOL_VERSION_MIN;

    /**
     * Size of the message header of the Protocol version 1
     */
    public static final int HEADER_SIZE_V_1 = 8;

    /**
     * Size of the message header of the Protocol version 2
     */
    public static final int HEADER_SIZE_V_2 = 12;

    /**
     * Size of the message header of the Protocol by default
     */
    public static final int HEADER_SIZE_DEFAULT = HEADER_SIZE_V_1;
}