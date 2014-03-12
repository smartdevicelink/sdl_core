package com.ford.syncV4.proxy;

import com.ford.syncV4.proxy.rpc.PerformInteractionResponse;

import junit.framework.TestCase;

/**
 * Created by enikolsky on 2013-10-02.
 */
public class PerformInteractionResponseTest extends TestCase {
    private static final String MANUAL_TEXT_ENTRY = "manualTextEntry";

    public void testManualTextEntryAPI() {
        PerformInteractionResponse msg = new PerformInteractionResponse();
        assertNotNull(msg);

        final String text = "0xdeadbeef";
        msg.setManualTextEntry(text);

        assertEquals(text, msg.getManualTextEntry());
    }

    public void testRemoveManualTextEntry() {
        PerformInteractionResponse msg = new PerformInteractionResponse();
        msg.setManualTextEntry("abcdef");
        msg.setManualTextEntry(null);
        assertNull(msg.getManualTextEntry());
    }

    public void testSetManualTextEntry() {
        PerformInteractionResponse msg = new PerformInteractionResponse();
        final String text = "012345";
        msg.setManualTextEntry(text);
        assertEquals(text, msg.parameters.get(MANUAL_TEXT_ENTRY));
    }

    public void testGetManualTextEntry() {
        PerformInteractionResponse msg = new PerformInteractionResponse();
        final String text = "098765";
        msg.parameters.put(MANUAL_TEXT_ENTRY, text);
        assertEquals(text, msg.getManualTextEntry());
    }

    /*
    // Throws java.lang.ClassCastException: java.lang.Integer cannot be cast to java.lang.String
    public void testGetManualTextEntryIncorrect() {
        PerformInteractionResponse msg = new PerformInteractionResponse();
        msg.parameters.put(MANUAL_TEXT_ENTRY, 42);
        assertNull(msg.getManualTextEntry());
    }
    */
}
