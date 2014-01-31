package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for PrerecordedSpeech enum.
 *
 * Created by enikolsky on 2014-01-31.
 */
public class PrerecordedSpeechTest extends TestCase {
    public void testAllValuesShouldBeDefined() {
        assertThat(PrerecordedSpeech.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        PrerecordedSpeech.HELP_JINGLE,
                        PrerecordedSpeech.INITIAL_JINGLE,
                        PrerecordedSpeech.LISTEN_JINGLE,
                        PrerecordedSpeech.POSITIVE_JINGLE,
                        PrerecordedSpeech.NEGATIVE_JINGLE));
    }

    public void testValueForStringHelpJingleShouldReturnCorrectValue() {
        assertThat(PrerecordedSpeech.valueOf("HELP_JINGLE"),
                is(PrerecordedSpeech.HELP_JINGLE));
    }

    public void testValueForStringInitialJingleShouldReturnCorrectValue() {
        assertThat(PrerecordedSpeech.valueOf("INITIAL_JINGLE"),
                is(PrerecordedSpeech.INITIAL_JINGLE));
    }

    public void testValueForStringListenJingleShouldReturnCorrectValue() {
        assertThat(PrerecordedSpeech.valueOf("LISTEN_JINGLE"),
                is(PrerecordedSpeech.LISTEN_JINGLE));
    }

    public void testValueForStringPositiveJingleShouldReturnCorrectValue() {
        assertThat(PrerecordedSpeech.valueOf("POSITIVE_JINGLE"),
                is(PrerecordedSpeech.POSITIVE_JINGLE));
    }

    public void testValueForStringNegativeJingleShouldReturnCorrectValue() {
        assertThat(PrerecordedSpeech.valueOf("NEGATIVE_JINGLE"),
                is(PrerecordedSpeech.NEGATIVE_JINGLE));
    }
}
