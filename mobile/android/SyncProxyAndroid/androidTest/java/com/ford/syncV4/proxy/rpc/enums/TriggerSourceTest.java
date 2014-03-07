package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for TriggerSource enum.
 *
 * Created by enikolsky on 2014-01-31.
 */
public class TriggerSourceTest extends TestCase {
    private static final String VR = "VR";
    private static final String MENU = "MENU";
    private static final String KEYBOARD = "KEYBOARD";

    public void testValueForStringMenuShouldReturnMenuItem() {
        assertThat(TriggerSource.valueForString(MENU),
                is(TriggerSource.TS_MENU));
    }

    public void testValueForStringVrShouldReturnVrItem() {
        assertThat(TriggerSource.valueForString(VR), is(TriggerSource.TS_VR));
    }

    public void testValueForStringKeyboardShouldReturnKeyboardItem() {
        assertThat(TriggerSource.valueForString(KEYBOARD),
                is(TriggerSource.TS_KEYBOARD));
    }

    public void testValueForStringUnknownShouldReturnNull() {
        assertThat(TriggerSource.valueForString("Unknown"), nullValue());
    }

    public void testMenuItemShouldHaveMenuName() {
        assertThat(TriggerSource.TS_MENU.toString(), is(MENU));
    }

    public void testVrItemShouldHaveVrName() {
        assertThat(TriggerSource.TS_VR.toString(), is(VR));
    }

    public void testKeyboardItemShouldHaveKeyboardName() {
        assertThat(TriggerSource.TS_KEYBOARD.toString(), is(KEYBOARD));
    }
}
