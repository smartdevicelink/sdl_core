package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for ButtonName enum.
 *
 * Created by enikolsky on 2014-01-31.
 */
public class ButtonNameTest extends TestCase {
    public void testAllValuesShouldBeDefined() {
        assertThat(ButtonName.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        ButtonName.OK, ButtonName.SEEKLEFT,
                        ButtonName.SEEKRIGHT, ButtonName.TUNEUP,
                        ButtonName.TUNEDOWN, ButtonName.PRESET_0,
                        ButtonName.PRESET_1, ButtonName.PRESET_2,
                        ButtonName.PRESET_3, ButtonName.PRESET_4,
                        ButtonName.PRESET_5, ButtonName.PRESET_6,
                        ButtonName.PRESET_7, ButtonName.PRESET_8,
                        ButtonName.PRESET_9, ButtonName.CUSTOM_BUTTON,
                        ButtonName.SEARCH));
    }

    public void testValueForStringOKShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("OK"), is(ButtonName.OK));
    }

    public void testValueForStringSeekLeftShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("SEEKLEFT"), is(ButtonName.SEEKLEFT));
    }

    public void testValueForStringSeekRightShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("SEEKRIGHT"), is(ButtonName.SEEKRIGHT));
    }

    public void testValueForStringTuneUpShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("TUNEUP"), is(ButtonName.TUNEUP));
    }

    public void testValueForStringTuneDownShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("TUNEDOWN"), is(ButtonName.TUNEDOWN));
    }

    public void testValueForStringPreset0ShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("PRESET_0"), is(ButtonName.PRESET_0));
    }

    public void testValueForStringPreset1ShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("PRESET_1"), is(ButtonName.PRESET_1));
    }

    public void testValueForStringPreset2ShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("PRESET_2"), is(ButtonName.PRESET_2));
    }

    public void testValueForStringPreset3ShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("PRESET_3"), is(ButtonName.PRESET_3));
    }

    public void testValueForStringPreset4ShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("PRESET_4"), is(ButtonName.PRESET_4));
    }

    public void testValueForStringPreset5ShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("PRESET_5"), is(ButtonName.PRESET_5));
    }

    public void testValueForStringPreset6ShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("PRESET_6"), is(ButtonName.PRESET_6));
    }

    public void testValueForStringPreset7ShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("PRESET_7"), is(ButtonName.PRESET_7));
    }

    public void testValueForStringPreset8ShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("PRESET_8"), is(ButtonName.PRESET_8));
    }

    public void testValueForStringPreset9ShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("PRESET_9"), is(ButtonName.PRESET_9));
    }

    public void testValueForStringCustomButtonShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("CUSTOM_BUTTON"),
                is(ButtonName.CUSTOM_BUTTON));
    }

    public void testValueForStringSearchShouldReturnCorrectValue() {
        assertThat(ButtonName.valueOf("SEARCH"), is(ButtonName.SEARCH));
    }

    public void testValueForStringUnknownShouldReturnNull() {
        assertThat(ButtonName.valueForString("Unknown"), nullValue());
    }

    public void testIndexForPresetButton0ShouldReturn0() {
        assertThat(ButtonName.indexForPresetButton(ButtonName.PRESET_0), is(0));
    }

    public void testIndexForPresetButton1ShouldReturn1() {
        assertThat(ButtonName.indexForPresetButton(ButtonName.PRESET_1), is(1));
    }

    public void testIndexForPresetButton9ShouldReturn9() {
        assertThat(ButtonName.indexForPresetButton(ButtonName.PRESET_9), is(9));
    }

    public void testIndexForPresetButtonCustomButtonShouldReturnNull() {
        assertThat(ButtonName.indexForPresetButton(ButtonName.CUSTOM_BUTTON),
                nullValue());
    }
}
