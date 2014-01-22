package com.ford.syncV4.integrationtest;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.SessionType;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.SyncProxyBase;
import com.ford.syncV4.proxy.converter.PutFileRPCRequestConverter;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.syncConnection.SyncConnection;

import org.json.JSONException;
import org.json.JSONObject;
import org.mockito.ArgumentCaptor;

import java.lang.reflect.Field;
import java.nio.charset.Charset;
import java.util.Arrays;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.core.Is.is;
import static org.hamcrest.core.IsNull.notNullValue;
import static org.mockito.Matchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * This is a integration test for how the PutFile request is broken down into
 * protocol frames to be sent.
 *
 * Created by enikolsky on 2014-01-20.
 */
public class PutFileRequestSendingTest extends InstrumentationTestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final int PUTFILE_FUNCTIONID = 32;
    private static final String OFFSET = "offset";
    private static final String LENGTH = "length";
    private static final int WAIT_TIMEOUT = 20;
    private IJsonRPCMarshaller marshaller;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);

        marshaller = new JsonRPCMarshaller();
    }

    public void testBigPutFileRequestShouldBeSentInPartsAndProxyListenerShouldBeCalledOnce()
            throws Exception {
        // we have to use the special IProxyListenerALMTesting here to allow to
        // send messages at any time
        IProxyListenerALMTesting proxyListenerMock =
                mock(IProxyListenerALMTesting.class);
        WiProProtocol protocolMock = mock(WiProProtocol.class);
        SyncConnection connectionMock = mock(SyncConnection.class);
        when(connectionMock.getIsConnected()).thenReturn(true);
        when(connectionMock.getWiProProtocol()).thenReturn(protocolMock);

        SyncProxyALM proxy =
                new SyncProxyALM(proxyListenerMock, null, "a", null, null,
                        false, null, null, null, null, null, null, false, false,
                        2, null, connectionMock);
        final int maxDataSize =
                ((PutFileRPCRequestConverter) proxy.getRpcRequestConverterFactory()
                                                   .getConverterForFunctionName(
                                                           "PutFile")).getMaxDataSize();

        final int extraDataSize = 10;
        final int dataSize = maxDataSize + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);
        final int correlationID = 0;

        PutFile msg = new PutFile();
        msg.setBulkData(data);
        msg.setCorrelationID(correlationID);
        proxy.sendRPCRequest(msg);

        Thread.sleep(WAIT_TIMEOUT);

        // we expect only the first frame of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor0 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock, times(1)).sendMessage(pmCaptor0.capture());

        final ProtocolMessage pm0 = pmCaptor0.getValue();
        checkOffsetAndLengthInJSON(pm0.getData(), 0, maxDataSize);
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm0.getBulkData(), is(data0));


        SyncConnection connectionMock2 = mock(SyncConnection.class);
        when(connectionMock2.getIsConnected()).thenReturn(true);
        when(connectionMock2.getWiProProtocol()).thenReturn(protocolMock);
        setSyncConnection(proxy, connectionMock2);

        // emulate incoming PutFile response
        PutFileResponse response = new PutFileResponse();
        response.setResultCode(Result.SUCCESS);
        response.setCorrelationID(correlationID);

        ProtocolMessage incomingPM0 = new ProtocolMessage();
        incomingPM0.setVersion(PROTOCOL_VERSION);
        byte[] msgBytes = marshaller.marshall(response, PROTOCOL_VERSION);
        incomingPM0.setData(msgBytes);
        incomingPM0.setJsonSize(msgBytes.length);
        incomingPM0.setMessageType(MessageType.RPC);
        incomingPM0.setSessionType(SessionType.RPC);
        incomingPM0.setFunctionID(PUTFILE_FUNCTIONID);
        incomingPM0.setRPCType(ProtocolMessage.RPCTYPE_RESPONSE);
        incomingPM0.setCorrID(correlationID);
        emulateIncomingMessage(proxy, incomingPM0);

        Thread.sleep(WAIT_TIMEOUT);

        // the listener should not be called yet
        verify(proxyListenerMock, never()).onPutFileResponse(
                any(PutFileResponse.class));

        // we expect the second frame to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor1 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock2, times(1)).sendMessage(pmCaptor1.capture());

        final ProtocolMessage pm1 = pmCaptor1.getValue();
        checkOffsetAndLengthInJSON(pm1.getData(), maxDataSize, extraDataSize);
        final byte[] data1 = Arrays.copyOfRange(data, maxDataSize,
                maxDataSize + extraDataSize);
        assertThat(pm1.getBulkData(), is(data1));

        ProtocolMessage incomingPM1 = new ProtocolMessage(incomingPM0);
        emulateIncomingMessage(proxy, incomingPM1);

        Thread.sleep(WAIT_TIMEOUT);

        // the listener should be called now
        ArgumentCaptor<PutFileResponse> responseCaptor =
                ArgumentCaptor.forClass(PutFileResponse.class);
        verify(proxyListenerMock, times(1)).onPutFileResponse(
                responseCaptor.capture());
        final PutFileResponse putFileResponse = responseCaptor.getValue();
        assertThat(putFileResponse.getResultCode(), is(Result.SUCCESS));
        assertThat(putFileResponse.getCorrelationID(), is(correlationID));
    }

    private void emulateIncomingMessage(SyncProxyALM proxy, ProtocolMessage pm)
            throws NoSuchFieldException, IllegalAccessException {
        final Field interfaceBroker =
                SyncProxyBase.class.getDeclaredField("_interfaceBroker");
        interfaceBroker.setAccessible(true);
        SyncProxyBase.SyncInterfaceBroker broker =
                (SyncProxyBase.SyncInterfaceBroker) interfaceBroker.get(proxy);
        broker.onProtocolMessageReceived(pm);
    }

    private void setSyncConnection(SyncProxyALM proxy,
                                   SyncConnection connection)
            throws NoSuchFieldException, IllegalAccessException {
        final Field syncConnection =
                SyncProxyBase.class.getDeclaredField("_syncConnection");
        syncConnection.setAccessible(true);
        syncConnection.set(proxy, connection);
    }

    // TODO what if correlation id is different?
    // TODO is the request saved after reconnect?
    // TODO what if corr id is correct, but response is not PutFile?
    // TODO what if the result code != OK?

    private void checkOffsetAndLengthInJSON(byte[] data, int offset, int length)
            throws JSONException {
        assertThat("JSON data must not be null", data, notNullValue());

        JSONObject jsonObject =
                new JSONObject(new String(data, Charset.defaultCharset()));
        assertThat("offset doesn't match", jsonObject.getInt(OFFSET),
                is(offset));
        assertThat("length doesn't match", jsonObject.getInt(LENGTH),
                is(length));
    }
}
