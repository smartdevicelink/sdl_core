package com.ford.syncV4.proxy;

import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.ShowConstantTBT;

import junit.framework.TestCase;

/**
 * Created by enikolsky on 2013-10-02.
 */
public class ShowConstantTBTTest extends TestCase {
    private static final String TIME_TO_DESTINATION = "timeToDestination";
    private static final String NEXT_TURN_ICON = "nextTurnIcon";

    public void testTimeToDestinationAPI() {
        ShowConstantTBT msg = new ShowConstantTBT();
        assertNotNull(msg);

        final String text = "123456";
        msg.setTimeToDestination(text);

        assertEquals(text, msg.getTimeToDestination());
    }

    public void testRemoveTimeToDestination() {
        ShowConstantTBT msg = new ShowConstantTBT();

        final String text = "098765";
        msg.setTimeToDestination(text);

        msg.setTimeToDestination(null);
        assertNull(msg.getTimeToDestination());
    }

    public void testSetTimeToDestination() {
        ShowConstantTBT msg = new ShowConstantTBT();
        final String text = "abcdef";
        msg.setTimeToDestination(text);
        assertEquals(text, msg.parameters.get(TIME_TO_DESTINATION));
    }

    public void testGetTimeToDestination() {
        ShowConstantTBT msg = new ShowConstantTBT();
        final String text = "ETA";
        msg.parameters.put(TIME_TO_DESTINATION, text);
        assertEquals(text, msg.getTimeToDestination());
    }

    /*
    // Throws java.lang.ClassCastException: java.lang.Integer cannot be cast to java.lang.String
    public void testGetTimeToDestinationIncorrect() {
        ShowConstantTBT msg = new ShowConstantTBT();
        msg.parameters.put(TIME_TO_DESTINATION, 42);
        assertNull(msg.getTimeToDestination());
    }
    */

    public void testNextTurnIconAPI() {
        ShowConstantTBT msg = new ShowConstantTBT();
        assertNotNull(msg);

        final Image image = new Image();
        msg.setNextTurnIcon(image);

        assertEquals(image, msg.getNextTurnIcon());
    }

    public void testRemoveNextTurnIcon() {
        ShowConstantTBT msg = new ShowConstantTBT();

        final Image image = new Image();
        msg.setNextTurnIcon(image);

        msg.setNextTurnIcon(null);
        assertNull(msg.getNextTurnIcon());
    }

    public void testSetNextTurnIcon() {
        ShowConstantTBT msg = new ShowConstantTBT();
        final Image image = new Image();
        msg.setNextTurnIcon(image);
        assertEquals(image, msg.parameters.get(NEXT_TURN_ICON));
    }

    public void testGetNextTurnIcon() {
        ShowConstantTBT msg = new ShowConstantTBT();
        final Image image = new Image();
        msg.parameters.put(NEXT_TURN_ICON, image);
        assertEquals(image, msg.getNextTurnIcon());
    }

    /*
    // Throws java.lang.ClassCastException: java.lang.Integer cannot be cast to java.lang.String
    public void testGetNextTurnIconIncorrect() {
        ShowConstantTBT msg = new ShowConstantTBT();
        msg.parameters.put(NEXT_TURN_ICON, 42);
        assertNull(msg.getNextTurnIcon());
    }
    */
}
