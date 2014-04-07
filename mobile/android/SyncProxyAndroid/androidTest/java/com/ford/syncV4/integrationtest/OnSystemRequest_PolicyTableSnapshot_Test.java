package com.ford.syncV4.integrationtest;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.RPCNotification;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.SyncProxyBase;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.proxy.converter.IRPCRequestConverterFactory;
import com.ford.syncV4.proxy.converter.SystemPutFileRPCRequestConverter;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.rpc.OnSystemRequest;
import com.ford.syncV4.proxy.rpc.PutFileResponse;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandler;
import com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.test.TestConfig;

import org.hamcrest.core.IsNull;
import org.json.JSONException;
import org.json.JSONObject;
import org.mockito.ArgumentCaptor;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.lang.reflect.Field;
import java.nio.charset.Charset;
import java.util.Arrays;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.core.Is.is;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Matchers.notNull;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;

/**
 * This is an integration test of the mobile side reaction on Policy Table Snapshot
 * APPLINK-5540
 *
 * Created by Yuriy Chernyshov on 2014-02-11.
 */
public class OnSystemRequest_PolicyTableSnapshot_Test extends InstrumentationTestCase {

    private static final String TAG = "PolicyTableSnapshot_Test";

    private static final byte PROTOCOL_VERSION = ProtocolConstants.PROTOCOL_VERSION_MAX;
    private static final int PUTFILE_FUNCTIONID = 32;
    private static final int ONSYSTEMREQUEST_FUNCTIONID = 32781;
    private static final String OFFSET = "offset";
    private static final String LENGTH = "length";
    private static final String SYNC_FILENAME = "syncFileName";
    private static final String SYSTEM_FILE = "systemFile";
    private static final String FILE_TYPE = "fileType";
    private static final int WAIT_TIMEOUT = 200;
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

        // Set correct version of the Protocol when creates RPC requests at SyncProxyBase
        when(connectionMock.getProtocolVersion()).thenReturn(ProtocolConstants.PROTOCOL_VERSION_MAX);

        proxy = new SyncProxyALM(proxyListenerMock, null, "a", null, null,
                false, null, null, null, null, null, null, false, false, 2,
                null, connectionMock, new TestConfig()){
            @Override
            public void setupSecureProxy() {
                // we don't need secure proxy here.
            }
        };

        final SystemPutFileRPCRequestConverter converter = new SystemPutFileRPCRequestConverter();
        maxDataSize = 64;
        converter.setMaxDataSize(maxDataSize);
        IRPCRequestConverterFactory factoryMock = mock(IRPCRequestConverterFactory.class);
        when(factoryMock.getConverterForRequest(notNull(RPCRequest.class))).thenReturn(converter);
        proxy.setRpcRequestConverterFactory(factoryMock);
    }

    public void testOnSystemRequestWithPTS() throws Exception {
        final FileType fileType = FileType.JSON;
        final int extraDataSize = 10;
        final int dataSize = (maxDataSize * 2) + extraDataSize;
        final byte[] dataSnapshot = TestCommon.getRandomBytes(dataSize);

        final String filename = "PolicyTableUpdate";

        IOnSystemRequestHandler handlerMock = mock(IOnSystemRequestHandler.class);
        doAnswer(new Answer() {
            @Override
            public Object answer(InvocationOnMock invocationOnMock) throws Throwable {
                assertNotNull(invocationOnMock.getArguments());
                assertEquals(2, invocationOnMock.getArguments().length);
                byte[] data = (byte[]) invocationOnMock.getArguments()[1];
                assertNotNull(data);
                assertEquals(dataSize, data.length);

                final ISystemRequestProxy proxy =
                        (ISystemRequestProxy) invocationOnMock.getArguments()[0];
                proxy.putPolicyTableUpdateFile(filename, data);
                return null;
            }
        }).when(handlerMock)
          .onPolicyTableSnapshotRequest(notNull(ISystemRequestProxy.class), eq(dataSnapshot));
        proxy.setOnSystemRequestHandler(handlerMock);

        // emulate incoming OnSystemRequest notification with HTTP
        OnSystemRequest onSystemRequest = new OnSystemRequest();
        onSystemRequest.setRequestType(RequestType.HTTP);
        onSystemRequest.setBulkData(dataSnapshot);
        onSystemRequest.setFileType(fileType);

        ProtocolMessage incomingOnSysRequest = createNotificationProtocolMessage(onSystemRequest,
                        ONSYSTEMREQUEST_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingOnSysRequest);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // expect PutFile to be sent
        ArgumentCaptor<ProtocolMessage> argumentCaptor = ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock, times(1)).sendMessage(argumentCaptor.capture());

        // set another connection mock to be able to verify the second time below
        //final SyncConnection connectionMock2 = createNewSyncConnectionMock();
        //setSyncConnection(proxy, connectionMock2);

        final ProtocolMessage protocolMessage = argumentCaptor.getValue();
        assertThat(protocolMessage.getFunctionID(), is(PUTFILE_FUNCTIONID));
        checkPutFileJSON(protocolMessage.getData(), fileType);
        final byte[] data = Arrays.copyOfRange(dataSnapshot, 0, maxDataSize);
        assertThat(protocolMessage.getBulkData(), is(data));
        final int putFileRequestCorrID = protocolMessage.getCorrID();

        // the listener should not be called for OnSystemRequest
        verifyZeroInteractions(proxyListenerMock);

        // emulate incoming PutFile response
        PutFileResponse putFileResponse = new PutFileResponse();
        putFileResponse.setResultCode(Result.SUCCESS);
        putFileResponse.setCorrelationID(putFileRequestCorrID);

        ProtocolMessage incomingPutFileResponse = createResponseProtocolMessage(putFileResponse,
                        putFileRequestCorrID, PUTFILE_FUNCTIONID);
        emulateIncomingMessage(proxy, incomingPutFileResponse);

        // wait for processing
        Thread.sleep(WAIT_TIMEOUT);

        // the listener should not be called for PutFile or OnSystemRequest
        verify(proxyListenerMock, never()).onPutFileResponse(any(PutFileResponse.class));
        verify(proxyListenerMock, never()).onOnSystemRequest(any(OnSystemRequest.class));
    }

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

    private ProtocolMessage createNotificationProtocolMessage(RPCNotification notification,
                                                              int functionID) {
        ProtocolMessage incomingPM0 = new ProtocolMessage();
        incomingPM0.setVersion(PROTOCOL_VERSION);
        byte[] msgBytes = marshaller.marshall(notification, PROTOCOL_VERSION);
        incomingPM0.setJsonSize(msgBytes.length);
        incomingPM0.setData(msgBytes);
        incomingPM0.setBulkData(notification.getBulkData());
        incomingPM0.setMessageType(MessageType.RPC);
        incomingPM0.setSessionType(ServiceType.RPC);
        incomingPM0.setFunctionID(functionID);
        incomingPM0.setRPCType(ProtocolMessage.RPCTYPE_NOTIFICATION);
        return incomingPM0;
    }

    private void emulateIncomingMessage(SyncProxyALM proxy, ProtocolMessage protocolMessage)
            throws NoSuchFieldException, IllegalAccessException {
        final Field interfaceBroker = SyncProxyBase.class.getDeclaredField("_interfaceBroker");
        interfaceBroker.setAccessible(true);
        SyncProxyBase.SyncInterfaceBroker broker =
                (SyncProxyBase.SyncInterfaceBroker) interfaceBroker.get(proxy);
        broker.onProtocolMessageReceived(protocolMessage);
    }

    private void setSyncConnection(SyncProxyALM proxy, SyncConnection connection)
            throws NoSuchFieldException, IllegalAccessException {
        final Field syncConnection = SyncProxyBase.class.getDeclaredField("mSyncConnection");
        syncConnection.setAccessible(true);
        syncConnection.set(proxy, connection);
    }

    private void checkPutFileJSON(byte[] data, FileType fileType)
            throws JSONException {
        assertThat("JSON data must not be null", data, IsNull.notNullValue());
        JSONObject jsonObject = new JSONObject(new String(data, Charset.defaultCharset()));
        assertThat("fileType must be set", jsonObject.getString(FILE_TYPE), is(fileType.toString()));
    }
}