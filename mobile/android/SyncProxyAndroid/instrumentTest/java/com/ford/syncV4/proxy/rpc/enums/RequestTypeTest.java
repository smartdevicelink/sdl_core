package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for RequestType enum.
 *
 * Created by enikolsky on 2014-02-10.
 */
public class RequestTypeTest extends TestCase {
    public void testAllValuesFromSpecificationShouldBeDefined() {
        assertThat(RequestType.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        RequestType.HTTP, RequestType.FILE_RESUME,
                        RequestType.AUTH_REQUEST, RequestType.AUTH_CHALLENGE,
                        RequestType.AUTH_ACK));
    }

    public void testValueForStringHTTPShouldReturnCorrectValue() {
        assertThat(RequestType.valueForString("HTTP"), is(RequestType.HTTP));
    }

    public void testValueForStringFILE_RESUMEShouldReturnCorrectValue() {
        assertThat(RequestType.valueForString("FILE_RESUME"),
                is(RequestType.FILE_RESUME));
    }

    public void testValueForStringAUTH_REQUESTShouldReturnCorrectValue() {
        assertThat(RequestType.valueForString("AUTH_REQUEST"),
                is(RequestType.AUTH_REQUEST));
    }

    public void testValueForStringAUTH_CHALLENGEShouldReturnCorrectValue() {
        assertThat(RequestType.valueForString("AUTH_CHALLENGE"),
                is(RequestType.AUTH_CHALLENGE));
    }

    public void testValueForStringAUTH_ACKShouldReturnCorrectValue() {
        assertThat(RequestType.valueForString("AUTH_ACK"),
                is(RequestType.AUTH_ACK));
    }

    public void testHTTPToStringShouldReturnCorrectValue() {
        assertThat(RequestType.HTTP.toString(), is("HTTP"));
    }

    public void testFILE_RESUMEToStringShouldReturnCorrectValue() {
        assertThat(RequestType.FILE_RESUME.toString(), is("FILE_RESUME"));
    }

    public void testAUTH_REQUESTToStringShouldReturnCorrectValue() {
        assertThat(RequestType.AUTH_REQUEST.toString(), is("AUTH_REQUEST"));
    }

    public void testAUTH_CHALLENGEToStringShouldReturnCorrectValue() {
        assertThat(RequestType.AUTH_CHALLENGE.toString(), is("AUTH_CHALLENGE"));
    }

    public void testAUTH_ACKToStringShouldReturnCorrectValue() {
        assertThat(RequestType.AUTH_ACK.toString(), is("AUTH_ACK"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            RequestType.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}
