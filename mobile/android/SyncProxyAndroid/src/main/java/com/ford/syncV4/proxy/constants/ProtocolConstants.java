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
     * Undefined Protocol version
     */
    public static final byte PROTOCOL_VERSION_UNDEFINED = 0x00;

    /**
     * Protocol version 1
     */
    public static final byte PROTOCOL_VERSION_ONE = 0x01;

    /**
     * Protocol version 2
     */
    public static final byte PROTOCOL_VERSION_TWO = 0x02;

    /**
     * Protocol version 3
     */
    public static final byte PROTOCOL_VERSION_THREE = 0x03;

    /**
     * Protocol version 4 (for the Test Cases)
     */
    public static final byte PROTOCOL_VERSION_FOUR = 0x04;

    /**
     * Define Protocol minimum support version
     */
    public static final byte PROTOCOL_VERSION_MIN = PROTOCOL_VERSION_ONE;

    /**
     * Define Protocol maximum support version
     */
    public static final byte PROTOCOL_VERSION_MAX = PROTOCOL_VERSION_TWO;

    /**
     * Size of the message header of the Protocol version 1
     */
    public static final int PROTOCOL_FRAME_HEADER_SIZE_V_1 = 8;

    /**
     * Size of the message header of the Protocol version 2
     */
    public static final int PROTOCOL_FRAME_HEADER_SIZE_V_2 = 12;

    /**
     * Size of the message header of the Protocol by default
     */
    public static final int PROTOCOL_FRAME_HEADER_SIZE_DEFAULT = PROTOCOL_FRAME_HEADER_SIZE_V_1;
}