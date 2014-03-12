package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for ECallConfirmationStatus enum.
 *
 * Created by enikolsky on 2014-02-11.
 */
public class ECallConfirmationStatusTest extends TestCase {
    public void testAllValuesFromSpecificationShouldBeDefined() {
        assertThat(ECallConfirmationStatus.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        ECallConfirmationStatus.NORMAL,
                        ECallConfirmationStatus.CALL_IN_PROGRESS,
                        ECallConfirmationStatus.CALL_CANCELLED,
                        ECallConfirmationStatus.CALL_COMPLETED,
                        ECallConfirmationStatus.CALL_UNSUCCESSFUL,
                        ECallConfirmationStatus.ECALL_CONFIGURED_OFF,
                        ECallConfirmationStatus.CALL_COMPLETE_DTMF_TIMEOUT));
    }


    public void testValueForStringNORMALShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.valueForString("NORMAL"),
                is(ECallConfirmationStatus.NORMAL));
    }

    public void testValueForStringCALL_IN_PROGRESSShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.valueForString("CALL_IN_PROGRESS"),
                is(ECallConfirmationStatus.CALL_IN_PROGRESS));
    }

    public void testValueForStringCALL_CANCELLEDShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.valueForString("CALL_CANCELLED"),
                is(ECallConfirmationStatus.CALL_CANCELLED));
    }

    public void testValueForStringCALL_COMPLETEDShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.valueForString("CALL_COMPLETED"),
                is(ECallConfirmationStatus.CALL_COMPLETED));
    }

    public void testValueForStringCALL_UNSUCCESSFULShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.valueForString("CALL_UNSUCCESSFUL"),
                is(ECallConfirmationStatus.CALL_UNSUCCESSFUL));
    }

    public void testValueForStringECALL_CONFIGURED_OFFShouldReturnCorrectValue() {
        assertThat(
                ECallConfirmationStatus.valueForString("ECALL_CONFIGURED_OFF"),
                is(ECallConfirmationStatus.ECALL_CONFIGURED_OFF));
    }

    public void testValueForStringCALL_COMPLETE_DTMF_TIMEOUTShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.valueForString(
                "CALL_COMPLETE_DTMF_TIMEOUT"),
                is(ECallConfirmationStatus.CALL_COMPLETE_DTMF_TIMEOUT));
    }


    public void testNORMALToStringShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.NORMAL.toString(), is("NORMAL"));
    }

    public void testCALL_IN_PROGRESSToStringShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.CALL_IN_PROGRESS.toString(),
                is("CALL_IN_PROGRESS"));
    }

    public void testCALL_CANCELLEDToStringShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.CALL_CANCELLED.toString(),
                is("CALL_CANCELLED"));
    }

    public void testCALL_COMPLETEDToStringShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.CALL_COMPLETED.toString(),
                is("CALL_COMPLETED"));
    }

    public void testCALL_UNSUCCESSFULToStringShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.CALL_UNSUCCESSFUL.toString(),
                is("CALL_UNSUCCESSFUL"));
    }

    public void testECALL_CONFIGURED_OFFToStringShouldReturnCorrectValue() {
        assertThat(ECallConfirmationStatus.ECALL_CONFIGURED_OFF.toString(),
                is("ECALL_CONFIGURED_OFF"));
    }

    public void testCALL_COMPLETE_DTMF_TIMEOUTToStringShouldReturnCorrectValue() {
        assertThat(
                ECallConfirmationStatus.CALL_COMPLETE_DTMF_TIMEOUT.toString(),
                is("CALL_COMPLETE_DTMF_TIMEOUT"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            ECallConfirmationStatus.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}
