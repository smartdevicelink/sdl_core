package com.ford.syncV4.protocol;

import com.ford.syncV4.proxy.constants.ProtocolConstants;

import junit.framework.TestCase;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 02.04.14
 * Time: 13:40
 */
public class ProtocolVersionTest extends TestCase {

    public void testCreateProtocolVersionWithDefaultMinValue() {
        ProtocolVersion protocolVersion = new ProtocolVersion();
        assertNotNull(protocolVersion);
        assertEquals(ProtocolConstants.PROTOCOL_VERSION_MIN, protocolVersion.getCurrentVersion());
    }

    public void testCreateProtocolVersionNegotiateToMaxSupported() {
        ProtocolVersion protocolVersion = new ProtocolVersion();
        protocolVersion.setCurrentVersion(ProtocolConstants.PROTOCOL_VERSION_TWO);
        assertEquals(ProtocolConstants.PROTOCOL_VERSION_TWO, protocolVersion.getCurrentVersion());
    }

    public void testCreateProtocolVersionNegotiateToMaxUnSupported() {
        ProtocolVersion protocolVersion = new ProtocolVersion();
        protocolVersion.setCurrentVersion(ProtocolConstants.PROTOCOL_VERSION_FOUR);
        assertEquals(ProtocolConstants.PROTOCOL_VERSION_MAX, protocolVersion.getCurrentVersion());
    }

    public void testCreateProtocolVersionNegotiateToInvalidReturnCurrentValue() {
        ProtocolVersion protocolVersion = new ProtocolVersion();
        byte currentVersion = protocolVersion.getCurrentVersion();
        protocolVersion.setCurrentVersion((byte) 0xff);
        assertEquals(currentVersion, protocolVersion.getCurrentVersion());
    }

    public void testCreateProtocolVersionNegotiateWithOutOfNegativeBoundValue() {
        ProtocolVersion protocolVersion = new ProtocolVersion();
        byte currentVersion = protocolVersion.getCurrentVersion();
        protocolVersion.setCurrentVersion((byte) -256);
        assertEquals(currentVersion, protocolVersion.getCurrentVersion());
    }

    public void testCreateProtocolVersionNegotiateWithOutOfPositiveBoundValue() {
        ProtocolVersion protocolVersion = new ProtocolVersion();
        byte currentVersion = protocolVersion.getCurrentVersion();
        protocolVersion.setCurrentVersion((byte) 256);
        assertEquals(currentVersion, protocolVersion.getCurrentVersion());
    }
}