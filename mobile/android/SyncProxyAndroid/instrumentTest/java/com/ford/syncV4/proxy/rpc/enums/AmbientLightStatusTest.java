package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for AmbientLightStatus enum.
 *
 * Created by enikolsky on 2014-02-07.
 */
public class AmbientLightStatusTest extends TestCase {
    public void testAllValuesFromSpecificationShouldBeDefined() {
        assertThat(AmbientLightStatus.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        AmbientLightStatus.NIGHT, AmbientLightStatus.TWILIGHT_1,
                        AmbientLightStatus.TWILIGHT_2,
                        AmbientLightStatus.TWILIGHT_3,
                        AmbientLightStatus.TWILIGHT_4, AmbientLightStatus.DAY,
                        AmbientLightStatus.UNKNOWN,
                        AmbientLightStatus.INVALID));
    }

    public void testValueForStringNIGHTShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.valueForString("NIGHT"),
                is(AmbientLightStatus.NIGHT));
    }

    public void testValueForStringTWILIGHT_1ShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.valueForString("TWILIGHT_1"),
                is(AmbientLightStatus.TWILIGHT_1));
    }

    public void testValueForStringTWILIGHT_2ShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.valueForString("TWILIGHT_2"),
                is(AmbientLightStatus.TWILIGHT_2));
    }

    public void testValueForStringTWILIGHT_3ShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.valueForString("TWILIGHT_3"),
                is(AmbientLightStatus.TWILIGHT_3));
    }

    public void testValueForStringTWILIGHT_4ShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.valueForString("TWILIGHT_4"),
                is(AmbientLightStatus.TWILIGHT_4));
    }

    public void testValueForStringDAYShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.valueForString("DAY"),
                is(AmbientLightStatus.DAY));
    }

    public void testValueForStringUNKNOWNShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.valueForString("UNKNOWN"),
                is(AmbientLightStatus.UNKNOWN));
    }

    public void testValueForStringINVALIDShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.valueForString("INVALID"),
                is(AmbientLightStatus.INVALID));
    }

    public void testNIGHTToStringShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.NIGHT.toString(), is("NIGHT"));
    }

    public void testTWILIGHT_1ToStringShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.TWILIGHT_1.toString(), is("TWILIGHT_1"));
    }

    public void testTWILIGHT_2ToStringShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.TWILIGHT_2.toString(), is("TWILIGHT_2"));
    }

    public void testTWILIGHT_3ToStringShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.TWILIGHT_3.toString(), is("TWILIGHT_3"));
    }

    public void testTWILIGHT_4ToStringShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.TWILIGHT_4.toString(), is("TWILIGHT_4"));
    }

    public void testDAYToStringShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.DAY.toString(), is("DAY"));
    }

    public void testUNKNOWNToStringShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.UNKNOWN.toString(), is("UNKNOWN"));
    }

    public void testINVALIDToStringShouldReturnCorrectValue() {
        assertThat(AmbientLightStatus.INVALID.toString(), is("INVALID"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            AmbientLightStatus.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}
