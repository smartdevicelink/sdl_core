package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for ImageFieldName enum.
 *
 * Created by enikolsky on 2014-02-07.
 */
public class ImageFieldNameTest extends TestCase {

    public void testAllValuesFromSpecificationShouldBeDefined() {
        assertThat(ImageFieldName.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        ImageFieldName.softButtonImage,
                        ImageFieldName.choiceImage,
                        ImageFieldName.choiceSecondaryImage,
                        ImageFieldName.vrHelpItem, ImageFieldName.turnIcon,
                        ImageFieldName.menuIcon, ImageFieldName.cmdIcon,
                        ImageFieldName.appIcon, ImageFieldName.graphic,
                        ImageFieldName.showConstantTBTIcon,
                        ImageFieldName.showConstantTBTNextTurnIcon, ImageFieldName.locationImage));
    }

    public void testValueForStringsoftButtonImageShouldReturnCorrectValue() {
        assertThat(ImageFieldName.valueForString("softButtonImage"),
                is(ImageFieldName.softButtonImage));
    }

    public void testValueForStringchoiceImageShouldReturnCorrectValue() {
        assertThat(ImageFieldName.valueForString("choiceImage"),
                is(ImageFieldName.choiceImage));
    }

    public void testValueForStringchoiceSecondaryImageShouldReturnCorrectValue() {
        assertThat(ImageFieldName.valueForString("choiceSecondaryImage"),
                is(ImageFieldName.choiceSecondaryImage));
    }

    public void testValueForStringvrHelpItemShouldReturnCorrectValue() {
        assertThat(ImageFieldName.valueForString("vrHelpItem"),
                is(ImageFieldName.vrHelpItem));
    }

    public void testValueForStringturnIconShouldReturnCorrectValue() {
        assertThat(ImageFieldName.valueForString("turnIcon"),
                is(ImageFieldName.turnIcon));
    }

    public void testValueForStringmenuIconShouldReturnCorrectValue() {
        assertThat(ImageFieldName.valueForString("menuIcon"),
                is(ImageFieldName.menuIcon));
    }

    public void testValueForStringcmdIconShouldReturnCorrectValue() {
        assertThat(ImageFieldName.valueForString("cmdIcon"),
                is(ImageFieldName.cmdIcon));
    }

    public void testValueForStringappIconShouldReturnCorrectValue() {
        assertThat(ImageFieldName.valueForString("appIcon"),
                is(ImageFieldName.appIcon));
    }

    public void testValueForStringgraphicShouldReturnCorrectValue() {
        assertThat(ImageFieldName.valueForString("graphic"),
                is(ImageFieldName.graphic));
    }

    public void testValueForStringshowConstantTBTIconShouldReturnCorrectValue() {
        assertThat(ImageFieldName.valueForString("showConstantTBTIcon"),
                is(ImageFieldName.showConstantTBTIcon));
    }

    public void testValueForStringshowConstantTBTNextTurnIconShouldReturnCorrectValue() {
        assertThat(ImageFieldName.valueForString("showConstantTBTNextTurnIcon"),
                is(ImageFieldName.showConstantTBTNextTurnIcon));
    }

    public void testsoftButtonImageToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.softButtonImage.toString(),
                is("softButtonImage"));
    }

    public void testchoiceImageToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.choiceImage.toString(), is("choiceImage"));
    }

    public void testchoiceSecondaryImageToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.choiceSecondaryImage.toString(),
                is("choiceSecondaryImage"));
    }

    public void testvrHelpItemToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.vrHelpItem.toString(), is("vrHelpItem"));
    }

    public void testturnIconToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.turnIcon.toString(), is("turnIcon"));
    }

    public void testmenuIconToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.menuIcon.toString(), is("menuIcon"));
    }

    public void testcmdIconToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.cmdIcon.toString(), is("cmdIcon"));
    }

    public void testappIconToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.appIcon.toString(), is("appIcon"));
    }

    public void testgraphicToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.graphic.toString(), is("graphic"));
    }

    public void testshowConstantTBTIconToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.showConstantTBTIcon.toString(),
                is("showConstantTBTIcon"));
    }

    public void testshowConstantTBTNextTurnIconToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.showConstantTBTNextTurnIcon.toString(),
                is("showConstantTBTNextTurnIcon"));
    }

    public void testshowConstantLocationImageToStringShouldReturnCorrectValue() {
        assertThat(ImageFieldName.locationImage.toString(),
                is("locationImage"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            ImageFieldName.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}
