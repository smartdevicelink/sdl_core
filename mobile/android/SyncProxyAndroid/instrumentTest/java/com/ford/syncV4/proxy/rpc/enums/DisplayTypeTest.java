package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for DisplayType enum.
 *
 * Created by enikolsky on 2014-02-06.
 */
public class DisplayTypeTest extends TestCase {
    public void testAllValuesShouldBeDefined() {
        assertThat(DisplayType.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        DisplayType.CID, DisplayType.TYPE2, DisplayType.TYPE5,
                        DisplayType.NGN, DisplayType.GEN2_8_DMA,
                        DisplayType.GEN2_6_DMA, DisplayType.MFD3,
                        DisplayType.MFD4, DisplayType.MFD5,
                        DisplayType.GEN3_8_INCH));
    }

    public void testValueForStringCIDShouldReturnCorrectValue() {
        assertThat(DisplayType.valueForString("CID"), is(DisplayType.CID));
    }

    public void testValueForStringTYPE2ShouldReturnCorrectValue() {
        assertThat(DisplayType.valueForString("TYPE2"), is(DisplayType.TYPE2));
    }

    public void testValueForStringTYPE5ShouldReturnCorrectValue() {
        assertThat(DisplayType.valueForString("TYPE5"), is(DisplayType.TYPE5));
    }

    public void testValueForStringNGNShouldReturnCorrectValue() {
        assertThat(DisplayType.valueForString("NGN"), is(DisplayType.NGN));
    }

    public void testValueForStringGEN2_8_DMAShouldReturnCorrectValue() {
        assertThat(DisplayType.valueForString("GEN2_8_DMA"),
                is(DisplayType.GEN2_8_DMA));
    }

    public void testValueForStringGEN2_6_DMAShouldReturnCorrectValue() {
        assertThat(DisplayType.valueForString("GEN2_6_DMA"),
                is(DisplayType.GEN2_6_DMA));
    }

    public void testValueForStringMFD3ShouldReturnCorrectValue() {
        assertThat(DisplayType.valueForString("MFD3"), is(DisplayType.MFD3));
    }

    public void testValueForStringMFD4ShouldReturnCorrectValue() {
        assertThat(DisplayType.valueForString("MFD4"), is(DisplayType.MFD4));
    }

    public void testValueForStringMFD5ShouldReturnCorrectValue() {
        assertThat(DisplayType.valueForString("MFD5"), is(DisplayType.MFD5));
    }

    public void testValueForStringGEN3_8minusINCHShouldReturnCorrectValue() {
        assertThat(DisplayType.valueForString("GEN3_8-INCH"),
                is(DisplayType.GEN3_8_INCH));
    }

    public void testCIDToStringShouldReturnCorrectValue() {
        assertThat(DisplayType.CID.toString(), is("CID"));
    }

    public void testTYPE2ToStringShouldReturnCorrectValue() {
        assertThat(DisplayType.TYPE2.toString(), is("TYPE2"));
    }

    public void testTYPE5ToStringShouldReturnCorrectValue() {
        assertThat(DisplayType.TYPE5.toString(), is("TYPE5"));
    }

    public void testNGNToStringShouldReturnCorrectValue() {
        assertThat(DisplayType.NGN.toString(), is("NGN"));
    }

    public void testGEN2_8_DMAToStringShouldReturnCorrectValue() {
        assertThat(DisplayType.GEN2_8_DMA.toString(), is("GEN2_8_DMA"));
    }

    public void testGEN2_6_DMAToStringShouldReturnCorrectValue() {
        assertThat(DisplayType.GEN2_6_DMA.toString(), is("GEN2_6_DMA"));
    }

    public void testMFD3ToStringShouldReturnCorrectValue() {
        assertThat(DisplayType.MFD3.toString(), is("MFD3"));
    }

    public void testMFD4ToStringShouldReturnCorrectValue() {
        assertThat(DisplayType.MFD4.toString(), is("MFD4"));
    }

    public void testMFD5ToStringShouldReturnCorrectValue() {
        assertThat(DisplayType.MFD5.toString(), is("MFD5"));
    }

    public void testGEN3_8_INCHToStringShouldReturnCorrectValue() {
        assertThat(DisplayType.GEN3_8_INCH.toString(), is("GEN3_8-INCH"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            DisplayType.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}
