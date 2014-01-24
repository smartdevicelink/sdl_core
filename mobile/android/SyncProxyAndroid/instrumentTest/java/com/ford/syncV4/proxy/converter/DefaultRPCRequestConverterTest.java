package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;

import junit.framework.TestCase;

import java.util.List;

import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.greaterThan;
import static org.hamcrest.core.Is.is;

/**
 * Tests for DefaultRPCRequestConverter class.
 *
 * Created by enikolsky on 2014-01-21.
 */
public class DefaultRPCRequestConverterTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;

    public void testGetProtocolMessagesForShowShouldReturnCorrectProtocolMessage()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        Show msg = new Show();
        msg.setAlignment(TextAlignment.CENTERED);
        msg.setCorrelationID(correlationID);

        DefaultRPCRequestConverter converter = new DefaultRPCRequestConverter();
        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID,
                        new JsonRPCMarshaller(), PROTOCOL_VERSION);

        assertThat(protocolMessages.size(), is(1));

        ProtocolMessage pm = protocolMessages.get(0);
        assertThat(pm.getData(), notNullValue());
        assertThat(pm.getData().length, greaterThan(0));
        assertThat(pm.getSessionID(), is(sessionID));
        assertThat(pm.getMessageType(), is(MessageType.RPC));
        assertThat(pm.getFunctionID(), is(13));
        assertThat(pm.getCorrID(), is(correlationID));
        assertThat(pm.getBulkData(), nullValue());
    }

    public void testGetProtocolMessagesShouldThrowNullPointerExceptionWhenRequestIsNull() {
        try {
            DefaultRPCRequestConverter converter =
                    new DefaultRPCRequestConverter();
            converter.getProtocolMessages(null, (byte) 0,
                    new JsonRPCMarshaller(), PROTOCOL_VERSION);
            fail("Should have thrown NullPointerException");
        } catch (NullPointerException e) {
            // success
        }
    }

    public void testGetProtocolMessagesShouldThrowNullPointerExceptionWhenMarshallerIsNull() {
        try {
            Show msg = new Show();

            DefaultRPCRequestConverter converter =
                    new DefaultRPCRequestConverter();
            converter.getProtocolMessages(msg, (byte) 0, null,
                    PROTOCOL_VERSION);
            fail("Should have thrown NullPointerException");
        } catch (NullPointerException e) {
            // success
        }
    }
}
