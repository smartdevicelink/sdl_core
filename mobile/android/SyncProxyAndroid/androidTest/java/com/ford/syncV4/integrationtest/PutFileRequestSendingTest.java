package com.ford.syncV4.integrationtest;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.SyncProxyBase;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.test.TestConfig;

import org.hamcrest.CoreMatchers;
import org.json.JSONException;
import org.json.JSONObject;
import org.mockito.ArgumentCaptor;

import java.lang.reflect.Field;
import java.nio.charset.Charset;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.core.Is.is;
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
    private IProxyListenerALMTesting proxyListenerMock;
    private WiProProtocol protocolMock;
    private SyncConnection connectionMock;
    private SyncProxyALM proxy;
    private int maxDataSize;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);

        marshaller = new JsonRPCMarshaller();

        // we have to use the special IProxyListenerALMTesting here to allow to
        // send messages at any time
        proxyListenerMock = mock(IProxyListenerALMTesting.class);
        protocolMock = mock(WiProProtocol.class);
        connectionMock = createNewSyncConnectionMock();

        proxy = new SyncProxyALM(proxyListenerMock, null, "a", null, null,
                false, null, null, null, null, null, null, false, false, 2,
                null, connectionMock, new TestConfig());

        WiProProtocol protocol =
                new WiProProtocol(mock(IProtocolListener.class));
        protocol.setProtocolVersion(PROTOCOL_VERSION);
        maxDataSize = WiProProtocol.MAX_DATA_SIZE;
    }

    public void testBigPutFileRequestShouldNotBeSentInPartsAndProxyListenerShouldBeCalledOnce()
            throws Exception {
        final int extraDataSize = 10;
        final int dataSize = maxDataSize + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);
        final int correlationID = 0;

        PutFile msg = RPCRequestFactory.buildPutFile();
        msg.setBulkData(data);
        msg.setCorrelationID(correlationID);
        proxy.sendRPCRequest(msg);

        Thread.sleep(WAIT_TIMEOUT);

        // we expect the whole PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor0 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock, times(1)).sendMessage(pmCaptor0.capture());

        final ProtocolMessage pm0 = pmCaptor0.getValue();
        checkNoOffsetAndLengthInJSON(pm0.getData());
        assertThat(pm0.getBulkData(), is(data));


        SyncConnection connectionMock2 = createNewSyncConnectionMock();
        setSyncConnection(proxy, connectionMock2);

        // emulate incoming PutFile response
        PutFileResponse response = new PutFileResponse();
        response.setResultCode(Result.SUCCESS);
        response.setCorrelationID(correlationID);

        ProtocolMessage incomingPM0 =
                createResponseProtocolMessage(response, correlationID,
                        PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPM0);

        Thread.sleep(WAIT_TIMEOUT);

        // the listener should be called now
        ArgumentCaptor<PutFileResponse> responseCaptor =
                ArgumentCaptor.forClass(PutFileResponse.class);
        verify(proxyListenerMock, times(1)).onPutFileResponse(
                responseCaptor.capture());
        final PutFileResponse putFileResponse = responseCaptor.getValue();
        assertThat(putFileResponse.getResultCode(), is(Result.SUCCESS));
        assertThat(putFileResponse.getCorrelationID(), is(correlationID));

        // we expect no other PutFile to be sent
        verify(connectionMock2, never()).sendMessage(
                any(ProtocolMessage.class));
    }

    public void testProxyListenerShouldBeCalledOnceOnBigPutFileErrorResponse()
            throws Exception {
        final int extraDataSize = 10;
        final int dataSize = maxDataSize + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);
        final int correlationID = 0;

        PutFile msg = RPCRequestFactory.buildPutFile();
        msg.setBulkData(data);
        msg.setCorrelationID(correlationID);
        proxy.sendRPCRequest(msg);

        Thread.sleep(WAIT_TIMEOUT);

        // we expect the whole PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor0 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock, times(1)).sendMessage(pmCaptor0.capture());

        final ProtocolMessage pm0 = pmCaptor0.getValue();
        checkNoOffsetAndLengthInJSON(pm0.getData());
        assertThat(pm0.getBulkData(), is(data));


        SyncConnection connectionMock2 = createNewSyncConnectionMock();
        setSyncConnection(proxy, connectionMock2);

        // emulate incoming PutFile response
        PutFileResponse response = new PutFileResponse();
        final Result resultCode = Result.INVALID_DATA;
        response.setResultCode(resultCode);
        response.setCorrelationID(correlationID);

        ProtocolMessage incomingPM0 =
                createResponseProtocolMessage(response, correlationID,
                        PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPM0);

        Thread.sleep(WAIT_TIMEOUT);

        // the listener should be called now
        ArgumentCaptor<PutFileResponse> responseCaptor =
                ArgumentCaptor.forClass(PutFileResponse.class);
        verify(proxyListenerMock, times(1)).onPutFileResponse(
                responseCaptor.capture());
        final PutFileResponse putFileResponse = responseCaptor.getValue();
        assertThat(putFileResponse.getResultCode(), is(resultCode));
        assertThat(putFileResponse.getCorrelationID(), is(correlationID));

        // we expect no other PutFile to be sent
        verify(connectionMock2, never()).sendMessage(
                any(ProtocolMessage.class));
    }

    private SyncConnection createNewSyncConnectionMock() {
        SyncConnection connectionMock2 = mock(SyncConnection.class);
        when(connectionMock2.getIsConnected()).thenReturn(true);
        when(connectionMock2.getWiProProtocol()).thenReturn(protocolMock);
        return connectionMock2;
    }

    private ProtocolMessage createResponseProtocolMessage(RPCResponse response,
                                                          int correlationID,
                                                          int functionID) {
        ProtocolMessage incomingPM0 = new ProtocolMessage();
        incomingPM0.setVersion(PROTOCOL_VERSION);
        byte[] msgBytes = marshaller.marshall(response, PROTOCOL_VERSION);
        incomingPM0.setData(msgBytes);
        incomingPM0.setJsonSize(msgBytes.length);
        incomingPM0.setMessageType(MessageType.RPC);
        incomingPM0.setSessionType(ServiceType.RPC);
        incomingPM0.setFunctionID(functionID);
        incomingPM0.setRPCType(ProtocolMessage.RPCTYPE_RESPONSE);
        incomingPM0.setCorrID(correlationID);
        return incomingPM0;
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
                SyncProxyBase.class.getDeclaredField("mSyncConnection");
        syncConnection.setAccessible(true);
        syncConnection.set(proxy, connection);
    }

    private void checkNoOffsetAndLengthInJSON(byte[] data)
            throws JSONException {
        assertThat("JSON data must not be null", data,
                CoreMatchers.notNullValue());

        JSONObject jsonObject =
                new JSONObject(new String(data, Charset.defaultCharset()));
        assertThat(jsonObject.has(OFFSET), is(false));
        assertThat(jsonObject.has(LENGTH), is(false));
    }
}
