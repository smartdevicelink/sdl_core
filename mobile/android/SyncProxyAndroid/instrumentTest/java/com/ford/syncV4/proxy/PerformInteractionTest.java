package com.ford.syncV4.proxy;

import com.ford.syncV4.proxy.rpc.PerformInteraction;
import com.ford.syncV4.proxy.rpc.enums.LayoutMode;

import junit.framework.TestCase;

/**
 * Created by enikolsky on 2013-10-02.
 */
public class PerformInteractionTest extends TestCase {
    private static final String INTERACTION_LAYOUT = "interactionLayout";

    public void testInteractionLayoutAPI() {
        PerformInteraction msg = new PerformInteraction();
        assertNotNull(msg);

        final LayoutMode layoutMode = LayoutMode.ICON_ONLY;
        msg.setInteractionLayout(layoutMode);

        assertEquals(layoutMode, msg.getInteractionLayout());
    }

    public void testRemoveInteractionLayout() {
        PerformInteraction msg = new PerformInteraction();

        final LayoutMode layoutMode = LayoutMode.KEYBOARD;
        msg.setInteractionLayout(layoutMode);

        msg.setInteractionLayout(null);
        assertNull(msg.getInteractionLayout());
    }

    public void testSetInteractionLayout() {
        PerformInteraction msg = new PerformInteraction();
        final LayoutMode layoutMode = LayoutMode.ICON_WITH_SEARCH;
        msg.setInteractionLayout(layoutMode);
        assertEquals(layoutMode, msg.parameters.get(INTERACTION_LAYOUT));
    }

    public void testGetInteractionLayout() {
        PerformInteraction msg = new PerformInteraction();
        final LayoutMode layoutMode = LayoutMode.LIST_WITH_SEARCH;
        msg.parameters.put(INTERACTION_LAYOUT, layoutMode);
        assertEquals(layoutMode, msg.getInteractionLayout());
    }

    public void testGetInteractionLayoutFromString() {
        PerformInteraction msg = new PerformInteraction();
        msg.parameters.put(INTERACTION_LAYOUT, "KEYBOARD");
        assertEquals(LayoutMode.KEYBOARD, msg.getInteractionLayout());
    }

    public void testGetInteractionLayoutIncorrect() {
        PerformInteraction msg = new PerformInteraction();
        msg.parameters.put(INTERACTION_LAYOUT, 42);
        assertNull(msg.getInteractionLayout());
    }
}
