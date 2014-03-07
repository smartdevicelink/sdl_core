package com.ford.syncV4.protocol.enums;

import junit.framework.TestCase;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.core.Is.is;

/**
 * Tests for FunctionID class, like correct IDs for function names.
 *
 * Created by enikolsky on 2014-01-23.
 */
public class FunctionIDTest extends TestCase {
    private static final String SYSTEM_REQUEST = "SystemRequest";
    private static final String ON_SYSTEM_REQUEST = "OnSystemRequest";
    private static final String DIAGNOSTIC_MESSAGE = "DiagnosticMessage";
    private static final int SYSTEM_REQUEST_ID = 38;
    private static final int ON_SYSTEM_REQUEST_ID = 32781;
    private static final int DIAGNOSTIC_MESSAGE_ID = 37;

    public void testSystemRequestIDShouldBe38() {
        assertThat(FunctionID.getFunctionID(SYSTEM_REQUEST),
                is(SYSTEM_REQUEST_ID));
    }

    public void testID38ShouldBeSystemRequest() {
        assertThat(FunctionID.getFunctionName(SYSTEM_REQUEST_ID),
                is(SYSTEM_REQUEST));
    }

    public void testOnSystemRequestIDShouldBe32781() {
        assertThat(FunctionID.getFunctionID(ON_SYSTEM_REQUEST),
                is(ON_SYSTEM_REQUEST_ID));
    }

    public void testID32781ShouldBeOnSystemRequest() {
        assertThat(FunctionID.getFunctionName(ON_SYSTEM_REQUEST_ID),
                is(ON_SYSTEM_REQUEST));
    }

    public void testDiagnosticMessageIDShouldBe37() {
        assertThat(FunctionID.getFunctionID(DIAGNOSTIC_MESSAGE),
                is(DIAGNOSTIC_MESSAGE_ID));
    }

    public void testID37ShouldBeDiagnosticMessageID() {
        assertThat(FunctionID.getFunctionName(DIAGNOSTIC_MESSAGE_ID),
                is(DIAGNOSTIC_MESSAGE));
    }
}
