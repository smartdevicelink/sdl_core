package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for TextFieldName enum.
 *
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
                        TextFieldName.scrollableMessageBody,
                        TextFieldName.initialInteractionText,
                        TextFieldName.navigationText1,
                        TextFieldName.navigationText2, TextFieldName.ETA,
                        TextFieldName.totalDistance,
                        TextFieldName.audioPassThruDisplayText1,
                        TextFieldName.audioPassThruDisplayText2,
                        TextFieldName.sliderHeader, TextFieldName.sliderFooter,
                        TextFieldName.menuName, TextFieldName.secondaryText,
                        TextFieldName.tertiaryText));
    }

    public void testValueForStringmainField1ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mainField1"),
                is(TextFieldName.mainField1));
    }

    public void testValueForStringmainField2ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mainField2"),
                is(TextFieldName.mainField2));
    }

    public void testValueForStringmainField3ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mainField3"),
                is(TextFieldName.mainField3));
    }

    public void testValueForStringmainField4ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mainField4"),
                is(TextFieldName.mainField4));
    }

    public void testValueForStringstatusBarShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("statusBar"),
                is(TextFieldName.statusBar));
    }

    public void testValueForStringmediaClockShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mediaClock"),
                is(TextFieldName.mediaClock));
    }

    public void testValueForStringmediaTrackShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("mediaTrack"),
                is(TextFieldName.mediaTrack));
    }

    public void testValueForStringalertText1ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("alertText1"),
                is(TextFieldName.alertText1));
    }

    public void testValueForStringalertText2ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("alertText2"),
                is(TextFieldName.alertText2));
    }

    public void testValueForStringalertText3ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("alertText3"),
                is(TextFieldName.alertText3));
    }

    public void testValueForStringscrollableMessageBodyShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("scrollableMessageBody"),
                is(TextFieldName.scrollableMessageBody));
    }

    public void testValueForStringinitialInteractionTextShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("initialInteractionText"),
                is(TextFieldName.initialInteractionText));
    }

    public void testValueForStringnavigationText1ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("navigationText1"),
                is(TextFieldName.navigationText1));
    }

    public void testValueForStringnavigationText2ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("navigationText2"),
                is(TextFieldName.navigationText2));
    }

    public void testValueForStringETAShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("ETA"), is(TextFieldName.ETA));
    }

    public void testValueForStringtotalDistanceShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("totalDistance"),
                is(TextFieldName.totalDistance));
    }

    public void testValueForStringaudioPassThruDisplayText1ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("audioPassThruDisplayText1"),
                is(TextFieldName.audioPassThruDisplayText1));
    }

    public void testValueForStringaudioPassThruDisplayText2ShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("audioPassThruDisplayText2"),
                is(TextFieldName.audioPassThruDisplayText2));
    }

    public void testValueForStringsliderHeaderShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("sliderHeader"),
                is(TextFieldName.sliderHeader));
    }

    public void testValueForStringsliderFooterShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("sliderFooter"),
                is(TextFieldName.sliderFooter));
    }

    public void testValueForStringmenuNameShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("menuName"),
                is(TextFieldName.menuName));
    }

    public void testValueForStringsecondaryTextShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("secondaryText"),
                is(TextFieldName.secondaryText));
    }

    public void testValueForStringtertiaryTextShouldReturnCorrectValue() {
        assertThat(TextFieldName.valueForString("tertiaryText"),
                is(TextFieldName.tertiaryText));
    }

    public void testmainField1ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mainField1.toString(), is("mainField1"));
    }

    public void testmainField2ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mainField2.toString(), is("mainField2"));
    }

    public void testmainField3ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mainField3.toString(), is("mainField3"));
    }

    public void testmainField4ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mainField4.toString(), is("mainField4"));
    }

    public void teststatusBarToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.statusBar.toString(), is("statusBar"));
    }

    public void testmediaClockToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mediaClock.toString(), is("mediaClock"));
    }

    public void testmediaTrackToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.mediaTrack.toString(), is("mediaTrack"));
    }

    public void testalertText1ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.alertText1.toString(), is("alertText1"));
    }

    public void testalertText2ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.alertText2.toString(), is("alertText2"));
    }

    public void testalertText3ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.alertText3.toString(), is("alertText3"));
    }

    public void testscrollableMessageBodyToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.scrollableMessageBody.toString(),
                is("scrollableMessageBody"));
    }

    public void testinitialInteractionTextToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.initialInteractionText.toString(),
                is("initialInteractionText"));
    }

    public void testnavigationText1ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.navigationText1.toString(),
                is("navigationText1"));
    }

    public void testnavigationText2ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.navigationText2.toString(),
                is("navigationText2"));
    }

    public void testETAToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.ETA.toString(), is("ETA"));
    }

    public void testtotalDistanceToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.totalDistance.toString(), is("totalDistance"));
    }

    public void testaudioPassThruDisplayText1ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.audioPassThruDisplayText1.toString(),
                is("audioPassThruDisplayText1"));
    }

    public void testaudioPassThruDisplayText2ToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.audioPassThruDisplayText2.toString(),
                is("audioPassThruDisplayText2"));
    }

    public void testsliderHeaderToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.sliderHeader.toString(), is("sliderHeader"));
    }

    public void testsliderFooterToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.sliderFooter.toString(), is("sliderFooter"));
    }

    public void testmenuNameToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.menuName.toString(), is("menuName"));
    }

    public void testsecondaryTextToStringShouldReturnCorrectValue() {
        assertThat(TextFieldName.secondaryText.toString(), is("secondaryText"));
    }

    public void testtertiaryTextToStringShouldReturnCorrectValue() {
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
