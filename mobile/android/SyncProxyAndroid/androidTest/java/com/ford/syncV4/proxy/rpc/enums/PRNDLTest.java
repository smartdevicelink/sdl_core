package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for PRNDL enum.
 *
 * Created by enikolsky on 2014-02-06.
 */
public class PRNDLTest extends TestCase {
    public void testAllValuesFromSpecificationShouldBeDefined() {
        assertThat(PRNDL.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        PRNDL.PARK, PRNDL.REVERSE, PRNDL.NEUTRAL, PRNDL.DRIVE,
                        PRNDL.SPORT, PRNDL.LOWGEAR, PRNDL.FIRST, PRNDL.SECOND,
                        PRNDL.THIRD, PRNDL.FOURTH, PRNDL.FIFTH, PRNDL.SIXTH,
                        PRNDL.SEVENTH, PRNDL.EIGHTH, PRNDL.UNKNOWN,
                        PRNDL.FAULT));
    }

    public void testValueForStringPARKShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("PARK"), is(PRNDL.PARK));
    }

    public void testValueForStringREVERSEShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("REVERSE"), is(PRNDL.REVERSE));
    }

    public void testValueForStringNEUTRALShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("NEUTRAL"), is(PRNDL.NEUTRAL));
    }

    public void testValueForStringDRIVEShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("DRIVE"), is(PRNDL.DRIVE));
    }

    public void testValueForStringSPORTShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("SPORT"), is(PRNDL.SPORT));
    }

    public void testValueForStringLOWGEARShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("LOWGEAR"), is(PRNDL.LOWGEAR));
    }

    public void testValueForStringFIRSTShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("FIRST"), is(PRNDL.FIRST));
    }

    public void testValueForStringSECONDShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("SECOND"), is(PRNDL.SECOND));
    }

    public void testValueForStringTHIRDShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("THIRD"), is(PRNDL.THIRD));
    }

    public void testValueForStringFOURTHShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("FOURTH"), is(PRNDL.FOURTH));
    }

    public void testValueForStringFIFTHShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("FIFTH"), is(PRNDL.FIFTH));
    }

    public void testValueForStringSIXTHShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("SIXTH"), is(PRNDL.SIXTH));
    }

    public void testValueForStringSEVENTHShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("SEVENTH"), is(PRNDL.SEVENTH));
    }

    public void testValueForStringEIGHTHShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("EIGHTH"), is(PRNDL.EIGHTH));
    }

    public void testValueForStringUNKNOWNShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("UNKNOWN"), is(PRNDL.UNKNOWN));
    }

    public void testValueForStringFAULTShouldReturnCorrectValue() {
        assertThat(PRNDL.valueForString("FAULT"), is(PRNDL.FAULT));
    }

    public void testPARKToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.PARK.toString(), is("PARK"));
    }

    public void testREVERSEToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.REVERSE.toString(), is("REVERSE"));
    }

    public void testNEUTRALToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.NEUTRAL.toString(), is("NEUTRAL"));
    }

    public void testDRIVEToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.DRIVE.toString(), is("DRIVE"));
    }

    public void testSPORTToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.SPORT.toString(), is("SPORT"));
    }

    public void testLOWGEARToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.LOWGEAR.toString(), is("LOWGEAR"));
    }

    public void testFIRSTToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.FIRST.toString(), is("FIRST"));
    }

    public void testSECONDToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.SECOND.toString(), is("SECOND"));
    }

    public void testTHIRDToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.THIRD.toString(), is("THIRD"));
    }

    public void testFOURTHToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.FOURTH.toString(), is("FOURTH"));
    }

    public void testFIFTHToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.FIFTH.toString(), is("FIFTH"));
    }

    public void testSIXTHToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.SIXTH.toString(), is("SIXTH"));
    }

    public void testSEVENTHToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.SEVENTH.toString(), is("SEVENTH"));
    }

    public void testEIGHTHToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.EIGHTH.toString(), is("EIGHTH"));
    }

    public void testUNKNOWNToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.UNKNOWN.toString(), is("UNKNOWN"));
    }

    public void testFAULTToStringShouldReturnCorrectValue() {
        assertThat(PRNDL.FAULT.toString(), is("FAULT"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            PRNDL.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}
