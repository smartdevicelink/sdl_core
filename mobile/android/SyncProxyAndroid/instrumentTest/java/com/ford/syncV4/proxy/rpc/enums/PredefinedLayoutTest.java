package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for PredefinedLayout enum.
 *
 * Created by enikolsky on 2014-02-17.
 */
public class PredefinedLayoutTest extends TestCase {
    public void testAllValuesFromSpecificationShouldBeDefined() {
        assertThat(PredefinedLayout.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        PredefinedLayout.DEFAULT, PredefinedLayout.MEDIA,
                        PredefinedLayout.NON_MEDIA,
                        PredefinedLayout.ONSCREEN_PRESETS,
                        PredefinedLayout.NAV_FULLSCREEN_MAP,
                        PredefinedLayout.NAV_LIST,
                        PredefinedLayout.NAV_KEYBOARD,
                        PredefinedLayout.GRAPHIC_WITH_TEXT,
                        PredefinedLayout.TEXT_WITH_GRAPHIC,
                        PredefinedLayout.TILES_ONLY,
                        PredefinedLayout.TEXTBUTTONS_ONLY,
                        PredefinedLayout.GRAPHIC_WITH_TILES,
                        PredefinedLayout.TILES_WITH_GRAPHIC,
                        PredefinedLayout.GRAPHIC_WITH_TEXT_AND_SOFTBUTTONS,
                        PredefinedLayout.TEXT_AND_SOFTBUTTONS_WITH_GRAPHIC,
                        PredefinedLayout.GRAPHIC_WITH_TEXTBUTTONS,
                        PredefinedLayout.TEXTBUTTONS_WITH_GRAPHIC,
                        PredefinedLayout.LARGE_GRAPHIC_WITH_SOFTBUTTONS,
                        PredefinedLayout.DOUBLE_GRAPHIC_WITH_SOFTBUTTONS,
                        PredefinedLayout.LARGE_GRAPHIC_ONLY));
    }

    public void testValueForStringDEFAULTShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("DEFAULT"),
                is(PredefinedLayout.DEFAULT));
    }

    public void testValueForStringMEDIAShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("MEDIA"),
                is(PredefinedLayout.MEDIA));
    }

    public void testValueForStringNONminusMEDIAShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("NON-MEDIA"),
                is(PredefinedLayout.NON_MEDIA));
    }

    public void testValueForStringONSCREEN_PRESETSShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("ONSCREEN_PRESETS"),
                is(PredefinedLayout.ONSCREEN_PRESETS));
    }

    public void testValueForStringNAV_FULLSCREEN_MAPShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("NAV_FULLSCREEN_MAP"),
                is(PredefinedLayout.NAV_FULLSCREEN_MAP));
    }

    public void testValueForStringNAV_LISTShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("NAV_LIST"),
                is(PredefinedLayout.NAV_LIST));
    }

    public void testValueForStringNAV_KEYBOARDShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("NAV_KEYBOARD"),
                is(PredefinedLayout.NAV_KEYBOARD));
    }

    public void testValueForStringGRAPHIC_WITH_TEXTShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("GRAPHIC_WITH_TEXT"),
                is(PredefinedLayout.GRAPHIC_WITH_TEXT));
    }

    public void testValueForStringTEXT_WITH_GRAPHICShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("TEXT_WITH_GRAPHIC"),
                is(PredefinedLayout.TEXT_WITH_GRAPHIC));
    }

    public void testValueForStringTILES_ONLYShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("TILES_ONLY"),
                is(PredefinedLayout.TILES_ONLY));
    }

    public void testValueForStringTEXTBUTTONS_ONLYShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("TEXTBUTTONS_ONLY"),
                is(PredefinedLayout.TEXTBUTTONS_ONLY));
    }

    public void testValueForStringGRAPHIC_WITH_TILESShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("GRAPHIC_WITH_TILES"),
                is(PredefinedLayout.GRAPHIC_WITH_TILES));
    }

    public void testValueForStringTILES_WITH_GRAPHICShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("TILES_WITH_GRAPHIC"),
                is(PredefinedLayout.TILES_WITH_GRAPHIC));
    }

    public void testValueForStringGRAPHIC_WITH_TEXT_AND_SOFTBUTTONSShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString(
                "GRAPHIC_WITH_TEXT_AND_SOFTBUTTONS"),
                is(PredefinedLayout.GRAPHIC_WITH_TEXT_AND_SOFTBUTTONS));
    }

    public void testValueForStringTEXT_AND_SOFTBUTTONS_WITH_GRAPHICShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString(
                "TEXT_AND_SOFTBUTTONS_WITH_GRAPHIC"),
                is(PredefinedLayout.TEXT_AND_SOFTBUTTONS_WITH_GRAPHIC));
    }

    public void testValueForStringGRAPHIC_WITH_TEXTBUTTONSShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("GRAPHIC_WITH_TEXTBUTTONS"),
                is(PredefinedLayout.GRAPHIC_WITH_TEXTBUTTONS));
    }

    public void testValueForStringTEXTBUTTONS_WITH_GRAPHICShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("TEXTBUTTONS_WITH_GRAPHIC"),
                is(PredefinedLayout.TEXTBUTTONS_WITH_GRAPHIC));
    }

    public void testValueForStringLARGE_GRAPHIC_WITH_SOFTBUTTONSShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString(
                "LARGE_GRAPHIC_WITH_SOFTBUTTONS"),
                is(PredefinedLayout.LARGE_GRAPHIC_WITH_SOFTBUTTONS));
    }

    public void testValueForStringDOUBLE_GRAPHIC_WITH_SOFTBUTTONSShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString(
                "DOUBLE_GRAPHIC_WITH_SOFTBUTTONS"),
                is(PredefinedLayout.DOUBLE_GRAPHIC_WITH_SOFTBUTTONS));
    }

    public void testValueForStringLARGE_GRAPHIC_ONLYShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.valueForString("LARGE_GRAPHIC_ONLY"),
                is(PredefinedLayout.LARGE_GRAPHIC_ONLY));
    }


    public void testDEFAULTToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.DEFAULT.toString(), is("DEFAULT"));
    }

    public void testMEDIAToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.MEDIA.toString(), is("MEDIA"));
    }

    public void testNONminusMEDIAToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.NON_MEDIA.toString(), is("NON-MEDIA"));
    }

    public void testONSCREEN_PRESETSToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.ONSCREEN_PRESETS.toString(),
                is("ONSCREEN_PRESETS"));
    }

    public void testNAV_FULLSCREEN_MAPToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.NAV_FULLSCREEN_MAP.toString(),
                is("NAV_FULLSCREEN_MAP"));
    }

    public void testNAV_LISTToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.NAV_LIST.toString(), is("NAV_LIST"));
    }

    public void testNAV_KEYBOARDToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.NAV_KEYBOARD.toString(),
                is("NAV_KEYBOARD"));
    }

    public void testGRAPHIC_WITH_TEXTToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.GRAPHIC_WITH_TEXT.toString(),
                is("GRAPHIC_WITH_TEXT"));
    }

    public void testTEXT_WITH_GRAPHICToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.TEXT_WITH_GRAPHIC.toString(),
                is("TEXT_WITH_GRAPHIC"));
    }

    public void testTILES_ONLYToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.TILES_ONLY.toString(), is("TILES_ONLY"));
    }

    public void testTEXTBUTTONS_ONLYToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.TEXTBUTTONS_ONLY.toString(),
                is("TEXTBUTTONS_ONLY"));
    }

    public void testGRAPHIC_WITH_TILESToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.GRAPHIC_WITH_TILES.toString(),
                is("GRAPHIC_WITH_TILES"));
    }

    public void testTILES_WITH_GRAPHICToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.TILES_WITH_GRAPHIC.toString(),
                is("TILES_WITH_GRAPHIC"));
    }

    public void testGRAPHIC_WITH_TEXT_AND_SOFTBUTTONSToStringShouldReturnCorrectValue() {
        assertThat(
                PredefinedLayout.GRAPHIC_WITH_TEXT_AND_SOFTBUTTONS.toString(),
                is("GRAPHIC_WITH_TEXT_AND_SOFTBUTTONS"));
    }

    public void testTEXT_AND_SOFTBUTTONS_WITH_GRAPHICToStringShouldReturnCorrectValue() {
        assertThat(
                PredefinedLayout.TEXT_AND_SOFTBUTTONS_WITH_GRAPHIC.toString(),
                is("TEXT_AND_SOFTBUTTONS_WITH_GRAPHIC"));
    }

    public void testGRAPHIC_WITH_TEXTBUTTONSToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.GRAPHIC_WITH_TEXTBUTTONS.toString(),
                is("GRAPHIC_WITH_TEXTBUTTONS"));
    }

    public void testTEXTBUTTONS_WITH_GRAPHICToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.TEXTBUTTONS_WITH_GRAPHIC.toString(),
                is("TEXTBUTTONS_WITH_GRAPHIC"));
    }

    public void testLARGE_GRAPHIC_WITH_SOFTBUTTONSToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.LARGE_GRAPHIC_WITH_SOFTBUTTONS.toString(),
                is("LARGE_GRAPHIC_WITH_SOFTBUTTONS"));
    }

    public void testDOUBLE_GRAPHIC_WITH_SOFTBUTTONSToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.DOUBLE_GRAPHIC_WITH_SOFTBUTTONS.toString(),
                is("DOUBLE_GRAPHIC_WITH_SOFTBUTTONS"));
    }

    public void testLARGE_GRAPHIC_ONLYToStringShouldReturnCorrectValue() {
        assertThat(PredefinedLayout.LARGE_GRAPHIC_ONLY.toString(),
                is("LARGE_GRAPHIC_ONLY"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            PredefinedLayout.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}
