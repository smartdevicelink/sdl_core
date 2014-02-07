package com.ford.syncV4.proxy;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.OnSystemRequest;
import com.ford.syncV4.proxy.rpc.SystemRequestResponse;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.rpc.enums.Result;

import junit.framework.TestCase;

import org.json.JSONException;
import org.mockito.ArgumentCaptor;

import java.util.Hashtable;
import java.util.Vector;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.core.Is.is;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * Tests for SyncProxyALM class calling proxy listener for different responses.
 *
 * Created by enikolsky on 2014-01-24.
 */
public class SyncProxyALMProxyListenerTest extends InstrumentationTestCase {
    private static final int CALLBACK_WAIT_TIMEOUT = 100;
    private IProxyListenerALM proxyListenerMock;
    private SyncProxyALM proxy;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);

        proxyListenerMock = mock(IProxyListenerALM.class);
        proxy = TestCommon.getSyncProxyALMNoTransport(proxyListenerMock);
        proxy._wiproVersion = 2;
    }

    public void testOnSystemRequestResponseShouldBeCalledOnResponse()
            throws SyncException, JSONException {
        // send SystemRequestResponse
        Hashtable<String, Object> params = new Hashtable<String, Object>();
        params.put(Names.success, true);
        params.put(Names.resultCode, Result.SUCCESS);
        proxy.dispatchIncomingMessage(
                TestCommon.createProtocolMessage(Names.SystemRequest, params,
                        ProtocolMessage.RPCTYPE_RESPONSE, 1));

        ArgumentCaptor<SystemRequestResponse> responseCaptor =
                ArgumentCaptor.forClass(SystemRequestResponse.class);
        verify(proxyListenerMock,
                timeout(CALLBACK_WAIT_TIMEOUT)).onSystemRequestResponse(
                responseCaptor.capture());
        assertThat(responseCaptor.getValue().getSuccess(), is(true));
        assertThat(responseCaptor.getValue().getResultCode(),
                is(Result.SUCCESS));
    }

    public void testOnOnSystemRequestShouldBeCalledOnNotification()
            throws SyncException, JSONException {
        final RequestType requestType = RequestType.AUTH_ACK;
        final Vector<String> urls = new Vector<String>();
        urls.add("https://x.y/");
        final FileType fileType = FileType.GRAPHIC_PNG;
        final int offset = 10;
        final int length = 1000;

        // send OnSystemRequest
        Hashtable<String, Object> params = new Hashtable<String, Object>();
        params.put(Names.requestType, requestType);
        params.put(Names.url, urls);
        params.put(Names.fileType, fileType);
        params.put(Names.offset, offset);
        params.put(Names.length, length);
        proxy.dispatchIncomingMessage(
                TestCommon.createProtocolMessage(Names.OnSystemRequest, params,
                        ProtocolMessage.RPCTYPE_NOTIFICATION, 1));

        ArgumentCaptor<OnSystemRequest> notificationCaptor =
                ArgumentCaptor.forClass(OnSystemRequest.class);
        verify(proxyListenerMock,
                timeout(CALLBACK_WAIT_TIMEOUT)).onOnSystemRequest(
                notificationCaptor.capture());
        final OnSystemRequest notification = notificationCaptor.getValue();
        assertThat(notification.getRequestType(), is(requestType));
        assertThat(notification.getUrl(), is(urls));
        assertThat(notification.getFileType(), is(fileType));
        assertThat(notification.getOffset(), is(offset));
        assertThat(notification.getLength(), is(length));
    }
}
