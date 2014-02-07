package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for VehicleDataNotificationStatus enum.
 *
 * Created by enikolsky on 2014-02-07.
 */
public class VehicleDataNotificationStatusTest extends TestCase {
    public void testAllValuesFromSpecificationShouldBeDefined() {
        assertThat(VehicleDataNotificationStatus.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(

                        VehicleDataNotificationStatus.NOT_SUPPORTED,
                        VehicleDataNotificationStatus.NORMAL,
                        VehicleDataNotificationStatus.ACTIVE,
                        VehicleDataNotificationStatus.NOT_USED));
    }

    public void testValueForStringNOT_SUPPORTEDShouldReturnCorrectValue() {
        assertThat(VehicleDataNotificationStatus.valueForString(
                "VDNS_NOT_SUPPORTED"),
                is(VehicleDataNotificationStatus.NOT_SUPPORTED));
    }

    public void testValueForStringNORMALShouldReturnCorrectValue() {
        assertThat(VehicleDataNotificationStatus.valueForString("VDNS_NORMAL"),
                is(VehicleDataNotificationStatus.NORMAL));
    }

    public void testValueForStringACTIVEShouldReturnCorrectValue() {
        assertThat(VehicleDataNotificationStatus.valueForString("VDNS_ACTIVE"),
                is(VehicleDataNotificationStatus.ACTIVE));
    }

    public void testValueForStringNOT_USEDShouldReturnCorrectValue() {
        assertThat(
                VehicleDataNotificationStatus.valueForString("VDNS_NOT_USED"),
                is(VehicleDataNotificationStatus.NOT_USED));
    }

    public void testNOT_SUPPORTEDToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataNotificationStatus.NOT_SUPPORTED.toString(),
                is("VDNS_NOT_SUPPORTED"));
    }

    public void testNORMALToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataNotificationStatus.NORMAL.toString(),
                is("VDNS_NORMAL"));
    }

    public void testACTIVEToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataNotificationStatus.ACTIVE.toString(),
                is("VDNS_ACTIVE"));
    }

    public void testNOT_USEDToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataNotificationStatus.NOT_USED.toString(),
                is("VDNS_NOT_USED"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            VehicleDataNotificationStatus.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}
