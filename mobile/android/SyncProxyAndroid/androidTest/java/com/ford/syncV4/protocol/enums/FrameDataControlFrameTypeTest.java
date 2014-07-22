package com.ford.syncV4.protocol.enums;

import junit.framework.TestCase;

import java.util.Arrays;


import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.collection.IsArrayContaining.hasItemInArray;
import static org.hamcrest.core.Is.is;
import static org.hamcrest.core.IsCollectionContaining.hasItem;


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
        assertNotNull("enum should have EndService value", FrameDataControlFrameType.EndService);
    }

    public void testFrameDataEndSessionACK() throws Exception {
        assertNotNull("enum should have EndService value", FrameDataControlFrameType.EndServiceACK);
    }

    public void testEndSessionACKValueIs0x005() throws Exception {
        assertEquals("EndServiceACK should have 0x005 value", (byte) 0x05, FrameDataControlFrameType.EndServiceACK.getValue());
    }

    public void testGetValueReturnsEndSessionACK() throws Exception {
        assertEquals("value of EndServiceACK string should be FrameData.EndServiceACK", FrameDataControlFrameType.EndServiceACK, FrameDataControlFrameType.valueOf("EndServiceACK"));
    }

    public void testFrameDataValuesHasEndSessionACKValue() throws Exception {
        assertTrue("FrameData list should have EndServiceACK value", Arrays.asList(FrameDataControlFrameType.values()).contains(FrameDataControlFrameType.EndServiceACK));
    }

    public void testMobileNaviACKIs0xFE() throws Exception {
        assertThat(FrameDataControlFrameType.MobileNaviACK.getValue(),
                is((byte) 0xFE));
    }

    public void testEndSessionNACKValueIs0x06() throws Exception {
        assertThat(FrameDataControlFrameType.EndServiceNACK.getValue(),
                is((byte) 0x06));
    }

    public void testFrameDataValuesHasEndSessionNACKValue() throws Exception {
        assertTrue("FrameData list should have EndServiceNACK value", Arrays.asList(FrameDataControlFrameType.values()).contains(FrameDataControlFrameType.EndServiceNACK));
    }
}
