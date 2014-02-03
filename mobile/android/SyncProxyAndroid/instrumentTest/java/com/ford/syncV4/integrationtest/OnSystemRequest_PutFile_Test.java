package com.ford.syncV4.integrationtest;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.IProtocolListener;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.SyncProxyBase;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.rpc.OnSystemRequest;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.syncConnection.SyncConnection;

import org.json.JSONException;
import org.json.JSONObject;
import org.mockito.ArgumentCaptor;

import java.lang.reflect.Field;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.Vector;

import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.core.Is.is;
import static org.mockito.Matchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;

/**
 * This is an integration test for how the OnSystemRequest notification and the
 * PutFile request interact to provide system updates to SDL.
 *
 * Created by enikolsky on 2014-02-03.
 */
public class OnSystemRequest_PutFile_Test extends InstrumentationTestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final int PUTFILE_FUNCTIONID = 32;
    private static final int ONSYSTEMREQUEST_FUNCTIONID = 32781;
    private static final String OFFSET = "offset";
    private static final String LENGTH = "length";
    private static final String SYNC_FILENAME = "syncFileName";
    private static final String SYSTEM_FILE = "systemFile";
    private static final String FILE_TYPE = "fileType";
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
                null, connectionMock);

        WiProProtocol protocol =
                new WiProProtocol(mock(IProtocolListener.class));
        protocol.setVersion(PROTOCOL_VERSION);
        maxDataSize = WiProProtocol.MAX_DATA_SIZE;
    }

    public void testOnSystemRequestRequestTypeHTTPShouldSendPartialPutFile()
            throws Exception {
        // fake data for PutFile
        final int extraDataSize = 10;
        final int dataSize = maxDataSize + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        // emulate incoming OnSystemRequest notification with HTTP
        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.HTTP);
        onSysRq.setUrl(
                new Vector<String>(Arrays.asList("http://example.com/")));
        onSysRq.setFileType(FileType.GRAPHIC_PNG);

        ProtocolMessage incomingOnSysRqPM0 =
                createNotificationProtocolMessage(onSysRq,
                        ONSYSTEMREQUEST_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingOnSysRqPM0);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // expect the first part of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor0 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock, times(1)).sendMessage(pmCaptor0.capture());

        final ProtocolMessage pm0 = pmCaptor0.getValue();
        assertThat(pm0.getFunctionID(), is(PUTFILE_FUNCTIONID));
        checkSystemPutFileJSON(pm0.getData(), 0, maxDataSize);
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm0.getBulkData(), is(data0));
        final int putFileRequestCorrID = pm0.getCorrID();

        // the listener should not be called for OnSystemRequest
        verifyZeroInteractions(proxyListenerMock);

        // emulate incoming PutFile response for first part
        PutFileResponse putFileResponse1 = new PutFileResponse();
        putFileResponse1.setResultCode(Result.SUCCESS);
        putFileResponse1.setCorrelationID(putFileRequestCorrID);

        ProtocolMessage incomingPutFileResponsePM1 =
                createResponseProtocolMessage(putFileResponse1,
                        putFileRequestCorrID, PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPutFileResponsePM1);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // expect the second part of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor1 =
                ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock, times(1)).sendMessage(pmCaptor1.capture());

        final ProtocolMessage pm1 = pmCaptor1.getValue();
        assertThat(pm1.getFunctionID(), is(PUTFILE_FUNCTIONID));
        checkSystemPutFileJSON(pm1.getData(), maxDataSize, extraDataSize);
        final byte[] data1 = Arrays.copyOfRange(data, maxDataSize, dataSize);
        assertThat(pm1.getBulkData(), is(data1));
        assertThat(pm1.getCorrID(), is(putFileRequestCorrID));

        // the listener should not be called for PutFile
        verifyZeroInteractions(proxyListenerMock);

        // emulate incoming PutFile response for second part
        PutFileResponse putFileResponse2 = new PutFileResponse();
        putFileResponse2.setResultCode(Result.SUCCESS);
        putFileResponse2.setCorrelationID(putFileRequestCorrID);

        ProtocolMessage incomingPutFileResponsePM2 =
                createResponseProtocolMessage(putFileResponse2,
                        putFileRequestCorrID, PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPutFileResponsePM2);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // the listener should not be called for PutFile
        verifyZeroInteractions(proxyListenerMock);

        // phew, done
    }

    // TODO what if PutFile response is error?
    // TODO check the rest is not sent after reconnect

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

    private ProtocolMessage createNotificationProtocolMessage(
            RPCNotification notification, int functionID) {
        ProtocolMessage incomingPM0 = new ProtocolMessage();
        incomingPM0.setVersion(PROTOCOL_VERSION);
        byte[] msgBytes = marshaller.marshall(notification, PROTOCOL_VERSION);
        incomingPM0.setData(msgBytes);
        incomingPM0.setJsonSize(msgBytes.length);
        incomingPM0.setMessageType(MessageType.RPC);
        incomingPM0.setSessionType(ServiceType.RPC);
        incomingPM0.setFunctionID(functionID);
        incomingPM0.setRPCType(ProtocolMessage.RPCTYPE_NOTIFICATION);
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
                SyncProxyBase.class.getDeclaredField("_syncConnection");
        syncConnection.setAccessible(true);
        syncConnection.set(proxy, connection);
    }

    private void checkSystemPutFileJSON(byte[] data, int offset, int length)
            throws JSONException {
        assertThat("JSON data must not be null", data, notNullValue());

        JSONObject jsonObject =
                new JSONObject(new String(data, Charset.defaultCharset()));
        assertThat("offset doesn't match", jsonObject.getInt(OFFSET),
                is(offset));
        assertThat("length doesn't match", jsonObject.getInt(LENGTH),
                is(length));
        assertThat("filename must be set", jsonObject.getString(SYNC_FILENAME),
                notNullValue());
        assertThat("systemFile must be true",
                jsonObject.getBoolean(SYSTEM_FILE), is(true));
        assertThat("fileType must be set", jsonObject.getJSONObject(FILE_TYPE),
                notNullValue());
    }
}
