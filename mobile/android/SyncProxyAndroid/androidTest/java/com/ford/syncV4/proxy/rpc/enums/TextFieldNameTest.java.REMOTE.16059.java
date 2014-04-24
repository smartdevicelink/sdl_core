package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for TextFieldName enum.
 * <p/>
 * Created by enikolsky on 2014-02-06.
 */
public class TextFieldNameTest extends TestCase {

    public void testAllValuesFromSpecificationShouldBeDefined() {
        assertThat(TextFieldName.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        TextFieldName.mainField1, TextFieldName.mainField2,
                        TextFieldName.mainField3, TextFieldName.mainField4,
                        TextFieldName.statusBar, TextFieldName.mediaClock,
                        TextFieldName.mediaTrack, TextFieldName.alertText1,
                        TextFieldName.alertText2, TextFieldName.alertText3,
                        TextFieldName.scrollableMessageBody, TextFieldName.initialInteractionText,
                        TextFieldName.navigationText1, TextFieldName.navigationText2,
                        TextFieldName.ETA, TextFieldName.totalDistance,
                        TextFieldName.audioPassThruDisplayText1,
                        TextFieldName.audioPassThruDisplayText2,
                        TextFieldName.sliderHeader, TextFieldName.sliderFooter,
                        TextFieldName.menuName, TextFieldName.secondaryText,
                        TextFieldName.tertiaryText, TextFieldName.menuTitle)
        );
    }

    public void testValueForStringMainField1ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mainField1"), is(TextFieldName.mainField1));
    }

    public void testValueForStringMainField2ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mainField2"), is(TextFieldName.mainField2));
    }

    public void testValueForStringMainField3ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mainField3"), is(TextFieldName.mainField3));
    }

    public void testValueForStringMainField4ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mainField4"), is(TextFieldName.mainField4));
    }

    public void testValueForStringStatusBarShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("statusBar"), is(TextFieldName.statusBar));
    }

    public void testValueForStringMediaClockShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mediaClock"), is(TextFieldName.mediaClock));
    }

    public void testValueForStringMediaTrackShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mediaTrack"), is(TextFieldName.mediaTrack));
    }

    public void testValueForStringAlertText1ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("alertText1"), is(TextFieldName.alertText1));
    }

    public void testValueForStringAlertText2ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("alertText2"), is(TextFieldName.alertText2));
    }

    public void testValueForStringAlertText3ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("alertText3"), is(TextFieldName.alertText3));
    }

    public void testValueForStringScrollableMessageBodyShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("scrollableMessageBody"),
                is(TextFieldName.scrollableMessageBody));
    }

    public void testValueForStringInitialInteractionTextShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("initialInteractionText"),
                is(TextFieldName.initialInteractionText));
    }

    public void testValueForStringNavigationText1ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("navigationText1"),
                is(TextFieldName.navigationText1));
    }

    public void testValueForStringNavigationText2ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("navigationText2"),
                is(TextFieldName.navigationText2));
    }

    public void testValueForStringETAShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("ETA"), is(TextFieldName.ETA));
    }

    public void testValueForStringTotalDistanceShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("totalDistance"), is(TextFieldName.totalDistance));
    }

    public void testValueForStringAudioPassThroughDisplayText1ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("audioPassThruDisplayText1"),
                is(TextFieldName.audioPassThruDisplayText1));
    }

    public void testValueForStringAudioPassThroughDisplayText2ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("audioPassThruDisplayText2"),
                is(TextFieldName.audioPassThruDisplayText2));
    }

    public void testValueForStringSliderHeaderShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("sliderHeader"), is(TextFieldName.sliderHeader));
    }

    public void testValueForStringSliderFooterShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("sliderFooter"), is(TextFieldName.sliderFooter));
    }

    public void testValueForStringMenuNameShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("menuName"), is(TextFieldName.menuName));
    }

    public void testValueForStringSecondaryTextShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("secondaryText"), is(TextFieldName.secondaryText));
    }

    public void testValueForStringMenuTitleTextShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("menuTitle"), is(TextFieldName.menuTitle));
    }

    public void testValueForStringTertiaryTextShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("tertiaryText"), is(TextFieldName.tertiaryText));
    }

    public void testMainField1ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mainField1.toString(), is("mainField1"));
    }

    public void testMainField2ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mainField2.toString(), is("mainField2"));
    }

    public void testMainField3ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mainField3.toString(), is("mainField3"));
    }

    public void testMainField4ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mainField4.toString(), is("mainField4"));
    }

    public void testStatusBarToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.statusBar.toString(), is("statusBar"));
    }

    public void testMediaClockToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mediaClock.toString(), is("mediaClock"));
    }

    public void testMediaTrackToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mediaTrack.toString(), is("mediaTrack"));
    }

    public void testAlertText1ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.alertText1.toString(), is("alertText1"));
    }

    public void testAlertText2ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.alertText2.toString(), is("alertText2"));
    }

    public void testAlertText3ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.alertText3.toString(), is("alertText3"));
    }

    public void testScrollableMessageBodyToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.scrollableMessageBody.toString(),
                is("scrollableMessageBody"));
    }

    public void testInitialInteractionTextToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.initialInteractionText.toString(),
                is("initialInteractionText"));
    }

    public void testNavigationText1ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.navigationText1.toString(), is("navigationText1"));
    }

    public void testNavigationText2ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.navigationText2.toString(), is("navigationText2"));
    }

    public void testETAToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.ETA.toString(), is("ETA"));
    }

    public void testTotalDistanceToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.totalDistance.toString(), is("totalDistance"));
    }

    public void testAudioPassThroughDisplayText1ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.audioPassThruDisplayText1.toString(),
                is("audioPassThruDisplayText1"));
    }

    public void testAudioPassThroughDisplayText2ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.audioPassThruDisplayText2.toString(),
                is("audioPassThruDisplayText2"));
    }

    public void testSliderHeaderToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.sliderHeader.toString(), is("sliderHeader"));
    }

    public void testSliderFooterToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.sliderFooter.toString(), is("sliderFooter"));
    }

    public void testMenuNameToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.menuName.toString(), is("menuName"));
    }

    public void testSecondaryTextToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.secondaryText.toString(), is("secondaryText"));
    }

    public void testMenuTitleToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.menuTitle.toString(), is("menuTitle"));
    }

    public void testTertiaryTextToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.tertiaryText.toString(), is("tertiaryText"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            TextFieldName.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}