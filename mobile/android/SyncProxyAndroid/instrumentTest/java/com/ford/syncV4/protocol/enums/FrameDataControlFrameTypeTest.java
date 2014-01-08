package com.ford.syncV4.protocol.enums;

import junit.framework.TestCase;

import java.util.Arrays;

import static org.hamcrest.CoreMatchers.hasItem;
import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.hasItemInArray;

/**
 * Tests for FrameDataControlFrameType class.
 *
 * Created by enikolsky on 2013-12-31.
 */
public class FrameDataControlFrameTypeTest extends TestCase {
    public void testHeartbeatTypeValueShouldBe0x00() {
        assertThat(FrameDataControlFrameType.Heartbeat.getValue(),
                is((byte) 0x00));
    }

    public void testHeartbeatTypeNameShouldMatchLiteral() {
        assertThat(FrameDataControlFrameType.Heartbeat.getName(),
                is("Heartbeat"));
    }

    public void testHeartbeatACKTypeValueShouldBe0xFF() {
        assertThat(FrameDataControlFrameType.HeartbeatACK.getValue(),
                is((byte) 0xFF));
    }

    public void testHeartbeatACKTypeNameShouldMatchLiteral() {
        assertThat(FrameDataControlFrameType.HeartbeatACK.getName(),
                is("HeartbeatACK"));
    }

    public void testListShouldContainHeartbeat() {
        assertThat(FrameDataControlFrameType.getList(),
                hasItem(FrameDataControlFrameType.Heartbeat));
    }

    public void testListShouldContainHeartbeatACK() {
        assertThat(FrameDataControlFrameType.getList(),
                hasItem(FrameDataControlFrameType.HeartbeatACK));
    }

    public void testValuesShouldContainHeartbeat() {
        assertThat(FrameDataControlFrameType.values(),
                hasItemInArray(FrameDataControlFrameType.Heartbeat));
    }

    public void testValuesShouldContainHeartbeatACK() {
        assertThat(FrameDataControlFrameType.values(),
                hasItemInArray(FrameDataControlFrameType.HeartbeatACK));
    }

    public void testValueOfHeartbeatShouldReturnObject() {
        assertThat(FrameDataControlFrameType.valueOf("Heartbeat"),
                is(FrameDataControlFrameType.Heartbeat));
    }

    public void testValueOfHeartbeatACKShouldReturnObject() {
        assertThat(FrameDataControlFrameType.valueOf("HeartbeatACK"),
                is(FrameDataControlFrameType.HeartbeatACK));
    }

    public void testFrameDataEndSession() throws Exception {
        assertNotNull("enum should have EndSession value", FrameDataControlFrameType.EndSession);
    }

    public void testFrameDataEndSessionACK() throws Exception {
        assertNotNull("enum should have EndSession value", FrameDataControlFrameType.EndSessionACK);
    }

    public void testEndSessionACKValueIs0x005() throws Exception {
        assertEquals("EndSessionACK should have 0x005 value", (byte) 0x05, FrameDataControlFrameType.EndSessionACK.getValue());
    }

    public void testGetValueReturnsEndSessionACK() throws Exception {
        assertEquals("value of EndSessionACK string should be FrameData.EndSessionACK", FrameDataControlFrameType.EndSessionACK, FrameDataControlFrameType.valueOf("EndSessionACK"));
    }

    public void testFrameDataValuesHasEndSessionACKValue() throws Exception {
        assertTrue("FrameData list should have EndSessionACK value", Arrays.asList(FrameDataControlFrameType.values()).contains(FrameDataControlFrameType.EndSessionACK));
    }
}
