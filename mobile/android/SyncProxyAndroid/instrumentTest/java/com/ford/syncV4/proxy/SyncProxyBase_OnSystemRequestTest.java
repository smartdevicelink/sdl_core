package com.ford.syncV4.proxy;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.rpc.OnSystemRequest;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandler;
import com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy;
import com.ford.syncV4.syncConnection.SyncConnection;

import java.util.Arrays;
import java.util.List;
import java.util.Vector;

import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.CoreMatchers.sameInstance;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.mockito.Matchers.eq;
import static org.mockito.Matchers.notNull;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;

/**
 * Tests how the SyncProxyALM handles OnSystemRequest notifications.
 *
 * Created by enikolsky on 2014-02-03.
 */
public class SyncProxyBase_OnSystemRequestTest extends InstrumentationTestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final int ONSYSTEMREQUEST_FUNCTIONID = 32781;
    private static final int WAIT_TIMEOUT = 20;
    private IProxyListenerALMTesting proxyListenerMock;
    private WiProProtocol protocolMock;
    private SyncConnection connectionMock;
    private SyncProxyALM proxy;
    private IOnSystemRequestHandler handlerMock;
    private IJsonRPCMarshaller marshaller;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);

        proxyListenerMock = mock(IProxyListenerALMTesting.class);
        protocolMock = mock(WiProProtocol.class);
        connectionMock = createNewSyncConnectionMock();

        proxy = new SyncProxyALM(proxyListenerMock, null, "a", null, null,
                false, null, null, null, null, null, null, false, false, 2,
                null, connectionMock);
        marshaller = proxy.getJsonRPCMarshaller();

        handlerMock = mock(IOnSystemRequestHandler.class);
    }

    public void testDefaultSystemRequestHandlerShouldBeNull() {
        assertThat(proxy.getOnSystemRequestHandler(), nullValue());
    }

    public void testGetOnSystemRequestHandlerShouldReturnSetValue() {
        proxy.setOnSystemRequestHandler(handlerMock);
        assertThat(proxy.getOnSystemRequestHandler(),
                sameInstance(handlerMock));
    }

    public void testOnSystemRequestWithRequestTypeHTTPShouldCallHandler()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final List<String> urls = Arrays.asList("http://example.com/");
        final FileType fileType = FileType.GRAPHIC_PNG;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.HTTP);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setFileType(fileType);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ONSYSTEMREQUEST_FUNCTIONID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, times(1)).onFilesDownloadRequest(
                notNull(ISystemRequestProxy.class), eq(urls), eq(fileType));
    }

    public void testOnSystemRequestWithRequestTypeHTTPShouldNotCallProxyListener()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final List<String> urls = Arrays.asList("http://example.com/");
        final FileType fileType = FileType.GRAPHIC_PNG;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.HTTP);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setFileType(fileType);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ONSYSTEMREQUEST_FUNCTIONID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);

        verifyZeroInteractions(proxyListenerMock);
    }

    public void testOnSystemRequestWithRequestTypeHTTPShouldNotCrashWhenHandlerNotSet()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(null);

        final List<String> urls = Arrays.asList("http://example.com/");
        final FileType fileType = FileType.GRAPHIC_PNG;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.HTTP);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setFileType(fileType);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ONSYSTEMREQUEST_FUNCTIONID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);
    }

    // TODO other request types
    // TODO check on main thread

    private SyncConnection createNewSyncConnectionMock() {
        SyncConnection connectionMock = mock(SyncConnection.class);
        when(connectionMock.getIsConnected()).thenReturn(true);
        when(connectionMock.getWiProProtocol()).thenReturn(protocolMock);
        return connectionMock;
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

    private void emulateIncomingMessage(SyncProxyALM proxy,
                                        ProtocolMessage pm) {
        // synchronous:
//        proxy.dispatchIncomingMessage(pm);
        // asynchronous, more correct:
        proxy.getInterfaceBroker().onProtocolMessageReceived(pm);
    }
}
