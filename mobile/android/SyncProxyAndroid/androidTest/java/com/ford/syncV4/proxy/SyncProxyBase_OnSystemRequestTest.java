package com.ford.syncV4.proxy;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.proxy.converter.IRPCRequestConverterFactory;
import com.ford.syncV4.proxy.converter.SystemPutFileRPCRequestConverter;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.rpc.OnSystemRequest;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.systemrequest.IOnSystemRequestHandler;
import com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy;
import com.ford.syncV4.session.SessionTest;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.test.TestConfig;
import com.ford.syncV4.transport.BTTransportConfig;
import com.ford.syncV4.transport.usb.USBTransportConfig;

import org.json.JSONException;
import org.json.JSONObject;
import org.mockito.ArgumentCaptor;

import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.List;
import java.util.Vector;

import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.CoreMatchers.sameInstance;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.core.Is.is;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyListOf;
import static org.mockito.Matchers.anyString;
import static org.mockito.Matchers.eq;
import static org.mockito.Matchers.isNull;
import static org.mockito.Matchers.notNull;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * Tests how the SyncProxyALM handles OnSystemRequest notifications.
 *
 * Created by enikolsky on 2014-02-03.
 */
public class SyncProxyBase_OnSystemRequestTest extends InstrumentationTestCase {

    private static final int PUT_FILE_FUNCTION_ID = 32;
    private static final int ON_SYSTEM_REQUEST_FUNCTION_ID = 32781;
    private static final int WAIT_TIMEOUT = 20;
    private WiProProtocol protocolMock;
    private SyncConnection connectionMock;
    private SyncProxyALM proxy;
    private IOnSystemRequestHandler handlerMock;
    private IJsonRPCMarshaller marshaller;
    private int maxDataSize;
    private IProxyListenerALMTesting proxyListenerMock;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);

        proxyListenerMock = mock(IProxyListenerALMTesting.class);
        protocolMock = mock(WiProProtocol.class);
        connectionMock = createNewSyncConnectionMock();

        // Set correct version of the Protocol when creates RPC requests at SyncProxyBase
        when(connectionMock.getProtocolVersion()).thenReturn(ProtocolConstants.PROTOCOL_VERSION_MAX);
        when(connectionMock.getIsConnected()).thenReturn(true);

        proxy = new SyncProxyALM(proxyListenerMock, null, "a", null, null,
                false, null, null, null, null, SessionTest.APP_ID, null, false, false,
                ProtocolConstants.PROTOCOL_VERSION_TWO,
                new USBTransportConfig(getInstrumentation().getTargetContext()), connectionMock,
                new TestConfig());
        marshaller = proxy.getJsonRPCMarshaller();

        handlerMock = mock(IOnSystemRequestHandler.class);

        final SystemPutFileRPCRequestConverter converter =
                new SystemPutFileRPCRequestConverter();
        maxDataSize = 32;
        converter.setMaxDataSize(maxDataSize);
        IRPCRequestConverterFactory factoryMock =
                mock(IRPCRequestConverterFactory.class);
        when(factoryMock.getConverterForRequest(
                notNull(RPCRequest.class))).thenReturn(converter);
        proxy.setRpcRequestConverterFactory(factoryMock);
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
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, times(1)).onFilesDownloadRequest(eq(SessionTest.APP_ID_DEFAULT),
                notNull(ISystemRequestProxy.class), eq(urls), eq(fileType));
    }

    public void testOnSystemRequestWithRequestTypeHTTPWithoutUrlShouldNotCallHandler()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final FileType fileType = FileType.GRAPHIC_PNG;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.HTTP);
        onSysRq.setFileType(fileType);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, never()).onFilesDownloadRequest(eq(SessionTest.APP_ID_DEFAULT),
                any(ISystemRequestProxy.class), anyListOf(String.class),
                any(FileType.class));
    }

    public void testOnSystemRequestWithRequestTypeHTTPWithoutFileTypeShouldCallHandler()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final List<String> urls = Arrays.asList("http://example.com/");

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.HTTP);
        onSysRq.setUrl(new Vector<String>(urls));

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, times(1)).onFilesDownloadRequest(eq(SessionTest.APP_ID_DEFAULT),
                notNull(ISystemRequestProxy.class), eq(urls),
                isNull(FileType.class));
    }

    public void testOnSystemRequestWithRequestTypeHTTPShouldCallProxyListener()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final List<String> urls = Arrays.asList("http://example.com/");
        final FileType fileType = FileType.GRAPHIC_PNG;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.HTTP);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setFileType(fileType);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);
        verify(proxyListenerMock, times(1)).onOnSystemRequest(any(String.class), any(OnSystemRequest.class));
    }

    public void testOnSystemRequestWithRequestTypeHTTPWithoutParametersShouldNotCallProxyListener()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.HTTP);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);
        verify(proxyListenerMock, times(1)).onOnSystemRequest(any(String.class) ,any(OnSystemRequest.class));
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
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);
    }

    public void testOnSystemRequestWithRequestTypeFileResumeShouldCallHandler()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final String filename = "dummy";
        final List<String> urls = Arrays.asList(filename);
        final FileType fileType = FileType.GRAPHIC_PNG;
        final int offset = 4000;
        final int length = 8800;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.FILE_RESUME);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setFileType(fileType);
        onSysRq.setOffset(offset);
        onSysRq.setLength(length);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, times(1)).onFileResumeRequest(eq(SessionTest.APP_ID_DEFAULT),
                notNull(ISystemRequestProxy.class), eq(filename), eq(offset),
                eq(length), eq(fileType));
    }

    public void testOnSystemRequestWithRequestTypeFileResumeWithoutFilenameShouldNotCallHandler()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final FileType fileType = FileType.GRAPHIC_PNG;
        final int offset = 4000;
        final int length = 8800;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.FILE_RESUME);
        onSysRq.setFileType(fileType);
        onSysRq.setOffset(offset);
        onSysRq.setLength(length);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, never()).onFileResumeRequest(eq(SessionTest.APP_ID_DEFAULT),
                any(ISystemRequestProxy.class), anyString(), anyInt(), anyInt(),
                any(FileType.class));
    }

    public void testOnSystemRequestWithRequestTypeFileResumeWithoutOffsetShouldNotCallHandler()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final String filename = "dummy";
        final List<String> urls = Arrays.asList(filename);
        final FileType fileType = FileType.GRAPHIC_PNG;
        final int length = 8800;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.FILE_RESUME);
        onSysRq.setFileType(fileType);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setLength(length);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, never()).onFileResumeRequest(eq(SessionTest.APP_ID_DEFAULT),
                any(ISystemRequestProxy.class), anyString(), anyInt(), anyInt(),
                any(FileType.class));
    }

    public void testOnSystemRequestWithRequestTypeFileResumeWithoutLengthShouldNotCallHandler()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final String filename = "dummy";
        final List<String> urls = Arrays.asList(filename);
        final FileType fileType = FileType.GRAPHIC_PNG;
        final int offset = 8800;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.FILE_RESUME);
        onSysRq.setFileType(fileType);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setOffset(offset);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, never()).onFileResumeRequest(eq(SessionTest.APP_ID_DEFAULT),
                any(ISystemRequestProxy.class), anyString(), anyInt(), anyInt(),
                any(FileType.class));
    }

    public void testOnSystemRequestWithRequestTypeFileResumeWithoutFileTypeShouldCallHandler()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final String filename = "dummy";
        final List<String> urls = Arrays.asList(filename);
        final int offset = 4000;
        final int length = 8800;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.FILE_RESUME);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setOffset(offset);
        onSysRq.setLength(length);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, times(1)).onFileResumeRequest(eq(SessionTest.APP_ID_DEFAULT),
                notNull(ISystemRequestProxy.class), eq(filename), eq(offset),
                eq(length), isNull(FileType.class));
    }

    public void testOnSystemRequestWithRequestTypeFileResumeShouldCallProxyListener()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final String filename = "dummy";
        final List<String> urls = Arrays.asList(filename);
        final FileType fileType = FileType.GRAPHIC_PNG;
        final int offset = 4000;
        final int length = 8800;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.FILE_RESUME);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setFileType(fileType);
        onSysRq.setOffset(offset);
        onSysRq.setLength(length);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);
        verify(proxyListenerMock, times(1)).onOnSystemRequest(any(String.class),any(OnSystemRequest.class));
    }

    public void testOnSystemRequestWithRequestTypeFileResumeWithoutParametersShouldCallProxyListener()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.FILE_RESUME);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);
        verify(proxyListenerMock, times(1)).onOnSystemRequest(any(String.class),any(OnSystemRequest.class));
    }

    public void testOnSystemRequestWithRequestTypeFileResumeShouldNotCrashWhenHandlerNotSet()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(null);

        final String filename = "dummy";
        final List<String> urls = Arrays.asList(filename);
        final FileType fileType = FileType.GRAPHIC_PNG;
        final int offset = 4000;
        final int length = 8800;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.FILE_RESUME);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setFileType(fileType);
        onSysRq.setOffset(offset);
        onSysRq.setLength(length);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);
    }

    public void testOnSystemRequestWithRequestTypeFileResumeMultipleFilenamesShouldCallHandlerOnce()
            throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final String filename0 = "whatchamacallit";
        final String filename1 = "thingamajig";
        final String filename2 = "doohickey";
        final List<String> urls = Arrays.asList(filename0, filename1, filename2);
        final FileType fileType = FileType.GRAPHIC_PNG;
        final int offset = 4000;
        final int length = 8800;

        OnSystemRequest onSysRq = new OnSystemRequest();
        onSysRq.setRequestType(RequestType.FILE_RESUME);
        onSysRq.setUrl(new Vector<String>(urls));
        onSysRq.setFileType(fileType);
        onSysRq.setOffset(offset);
        onSysRq.setLength(length);

        ProtocolMessage pm = createNotificationProtocolMessage(onSysRq,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        emulateIncomingMessage(proxy, pm);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, times(1)).onFileResumeRequest(eq(SessionTest.APP_ID_DEFAULT),
                notNull(ISystemRequestProxy.class), eq(filename0), eq(offset),
                eq(length), eq(fileType));
    }

    public void testPutSystemFileShouldSendCorrectFirstProtocolMessage()
            throws InterruptedException, JSONException, SyncException {

        // fake data for PutFile
        final int extraDataSize = 10;
        final int dataSize = maxDataSize + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        final String filename = "file";
        final FileType fileType = FileType.GRAPHIC_JPEG;

        proxy.putSystemFile(SessionTest.APP_ID_DEFAULT, filename, data, fileType);

        Thread.sleep(WAIT_TIMEOUT);

        // expect the first part of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor = ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock, times(1)).sendMessage(pmCaptor.capture());

        final ProtocolMessage pm = pmCaptor.getValue();
        assertThat(pm.getFunctionID(), is(PUT_FILE_FUNCTION_ID));
        checkSystemPutFileJSON(pm.getData(), 0, maxDataSize, filename, fileType);
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm.getBulkData(), is(data0));
    }

    public void testPutSystemFileWithOffsetShouldSendCorrectFirstProtocolMessage()
            throws InterruptedException, JSONException, SyncException {
        // fake data for PutFile
        final int extraDataSize = 10;
        final int dataSize = maxDataSize + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        final String filename = "file";
        final FileType fileType = FileType.GRAPHIC_JPEG;
        final int offset = 4000;
        proxy.putSystemFile(SessionTest.APP_ID_DEFAULT, filename, data, offset, fileType);

        Thread.sleep(WAIT_TIMEOUT);

        // expect the first part of PutFile to be sent
        ArgumentCaptor<ProtocolMessage> pmCaptor = ArgumentCaptor.forClass(ProtocolMessage.class);
        verify(connectionMock, times(1)).sendMessage(pmCaptor.capture());

        final ProtocolMessage pm = pmCaptor.getValue();
        assertThat(pm.getFunctionID(), is(PUT_FILE_FUNCTION_ID));
        checkSystemPutFileJSON(pm.getData(), offset, maxDataSize, filename, fileType);
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm.getBulkData(), is(data0));
    }

    public void testOnSystemRequestHTTPandBINARYTriggerProcessPolicy() throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final String filename = "dummy";
        final List<String> urls = Arrays.asList(filename);
        final FileType fileType = FileType.BINARY;
        final RequestType requestType = RequestType.HTTP;
        final byte[] data = new byte[2048];

        OnSystemRequest onSystemRequest = new OnSystemRequest();
        onSystemRequest.setRequestType(requestType);
        onSystemRequest.setUrl(new Vector<String>(urls));
        onSystemRequest.setFileType(fileType);
        onSystemRequest.setBulkData(data);

        ProtocolMessage protocolMessage = createNotificationProtocolMessage(onSystemRequest,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        protocolMessage.setBulkData(data);
        emulateIncomingMessage(proxy, protocolMessage);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, times(1)).onPolicyTableSnapshotRequest(eq(SessionTest.APP_ID_DEFAULT),
                notNull(ISystemRequestProxy.class), eq(data), eq(fileType), eq(requestType));
    }

    public void testOnSystemRequestPROPRIETARYandJSONTriggerProcessPolicy() throws InterruptedException {
        proxy.setOnSystemRequestHandler(handlerMock);

        final FileType fileType = FileType.JSON;
        final RequestType requestType = RequestType.PROPRIETARY;
        final byte[] data = new byte[2048];

        OnSystemRequest onSystemRequest = new OnSystemRequest();
        onSystemRequest.setRequestType(requestType);
        onSystemRequest.setFileType(fileType);
        onSystemRequest.setBulkData(data);

        ProtocolMessage protocolMessage = createNotificationProtocolMessage(onSystemRequest,
                ON_SYSTEM_REQUEST_FUNCTION_ID);
        protocolMessage.setBulkData(data);
        emulateIncomingMessage(proxy, protocolMessage);

        Thread.sleep(WAIT_TIMEOUT);

        verify(handlerMock, times(1)).onPolicyTableSnapshotRequest(eq(SessionTest.APP_ID_DEFAULT),
                notNull(ISystemRequestProxy.class), eq(data), eq(fileType), eq(requestType));
    }

    // TODO other request types
    // TODO check on main thread

    private SyncConnection createNewSyncConnectionMock() {
        SyncConnection connectionMock = mock(SyncConnection.class);
        when(connectionMock.getIsConnected()).thenReturn(true);
        when(connectionMock.getWiProProtocol()).thenReturn(protocolMock);
        return connectionMock;
    }

    private ProtocolMessage createNotificationProtocolMessage(RPCNotification notification,
                                                              int functionID) {
        ProtocolMessage protocolMessage = new ProtocolMessage();
        protocolMessage.setVersion(ProtocolConstants.PROTOCOL_VERSION_TWO);
        byte[] msgBytes = marshaller.marshall(notification, ProtocolConstants.PROTOCOL_VERSION_TWO);
        protocolMessage.setData(msgBytes);
        protocolMessage.setJsonSize(msgBytes.length);
        protocolMessage.setMessageType(MessageType.RPC);
        protocolMessage.setServiceType(ServiceType.RPC);
        protocolMessage.setFunctionID(functionID);
        protocolMessage.setRPCType(ProtocolMessage.RPCTYPE_NOTIFICATION);
        return protocolMessage;
    }

    private void emulateIncomingMessage(SyncProxyALM proxy, ProtocolMessage protocolMessage) {
        // synchronous:
        // proxy.dispatchIncomingMessage(protocolMessage);
        // asynchronous, more correct:
        proxy.getInterfaceBroker().onProtocolMessageReceived(protocolMessage);
    }

    private void checkSystemPutFileJSON(byte[] data, int offset, int length,
                                        String filename, FileType fileType) throws JSONException {
        assertThat("JSON data must not be null", data, notNullValue());

        JSONObject jsonObject = new JSONObject(new String(data, Charset.defaultCharset()));
        assertThat("offset doesn't match", jsonObject.getInt(Names.offset), is(offset));
        assertThat("length doesn't match", jsonObject.getInt(Names.length), is(length));
        assertThat("filename must be set", jsonObject.getString(Names.syncFileName),
                is(filename));
        assertThat("systemFile must be true", jsonObject.getBoolean(Names.systemFile), is(true));
        assertThat("fileType must be set", jsonObject.getString(Names.fileType),
                is(fileType.toString()));
    }
}