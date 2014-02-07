package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for WarningLightStatus enum.
 *
 * Created by enikolsky on 2014-02-07.
 */
public class WarningLightStatusTest extends TestCase {
    public void testAllValuesFromSpecificationShouldBeDefined() {
        assertThat(WarningLightStatus.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        WarningLightStatus.OFF, WarningLightStatus.ON,
                        WarningLightStatus.FLASH, WarningLightStatus.NOT_USED));
    }

    public void testValueForStringOFFShouldReturnCorrectValue() {
        assertThat(WarningLightStatus.valueForString("WLS_OFF"),
                is(WarningLightStatus.OFF));
    }

    public void testValueForStringONShouldReturnCorrectValue() {
        assertThat(WarningLightStatus.valueForString("WLS_ON"),
                is(WarningLightStatus.ON));
    }

    public void testValueForStringFLASHShouldReturnCorrectValue() {
        assertThat(WarningLightStatus.valueForString("WLS_FLASH"),
                is(WarningLightStatus.FLASH));
    }

    public void testValueForStringNOT_USEDShouldReturnCorrectValue() {
        assertThat(WarningLightStatus.valueForString("WLS_NOT_USED"),
                is(WarningLightStatus.NOT_USED));
    }

    public void testOFFToStringShouldReturnCorrectValue() {
        assertThat(WarningLightStatus.OFF.toString(), is("WLS_OFF"));
    }

    public void testONToStringShouldReturnCorrectValue() {
        assertThat(WarningLightStatus.ON.toString(), is("WLS_ON"));
    }

    public void testFLASHToStringShouldReturnCorrectValue() {
        assertThat(WarningLightStatus.FLASH.toString(), is("WLS_FLASH"));
    }

    public void testNOT_USEDToStringShouldReturnCorrectValue() {
        assertThat(WarningLightStatus.NOT_USED.toString(), is("WLS_NOT_USED"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            WarningLightStatus.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}
