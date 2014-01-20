package com.ford.syncV4.integrationtest;

import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALMTesting;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.syncConnection.ISyncConnectionListener;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.transport.SyncTransport;

import junit.framework.TestCase;

import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.io.ByteArrayOutputStream;
import java.util.Random;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.core.Is.is;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * This is a integration test for how the PutFile request is broken down into
 * protocol frames to be sent.
 *
 * Created by enikolsky on 2014-01-20.
 */
public class PutFileRequestSendingTest extends TestCase {
    public void testBigPutFileRequestShouldBeSplitIntoFirstAndConsecutiveFrames()
            throws Exception {
        final byte[] startSessionACKBytes =
                new byte[]{ 0x20, 0x07, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x01 };

        final ByteArrayOutputStream baos = new ByteArrayOutputStream();

        // we have to use the special IProxyListenerALMTesting here to allow to
        // send messages at any time
        IProxyListenerALMTesting proxyListenerMock =
                mock(IProxyListenerALMTesting.class);
        SyncTransport transportMock = mock(SyncTransport.class);
        when(transportMock.getIsConnected()).thenReturn(true);
        when(transportMock.sendBytes(any(byte[].class), anyInt(),
                anyInt())).then(new Answer<Object>() {
            @Override
            public Object answer(InvocationOnMock invocation) throws Throwable {
                final Object[] arguments = invocation.getArguments();
                final byte[] data = (byte[]) arguments[0];
                final Integer offset = (Integer) arguments[1];
                final Integer len = (Integer) arguments[2];
                baos.write(data, offset, len);
                return true;
            }
        });
        SyncConnection connection =
                new SyncConnection(mock(ISyncConnectionListener.class), null,
                        transportMock);
        connection.onTransportBytesReceived(startSessionACKBytes,
                startSessionACKBytes.length);

        SyncProxyALM proxy =
                new SyncProxyALM(proxyListenerMock, null, "a", null, null,
                        false, null, null, null, null, null, null, false, false,
                        2, null, connection);

        final int maxDataSize = WiProProtocol.MAX_DATA_SIZE;
        final int dataSize = maxDataSize + 10;
        final byte[] data = new byte[dataSize];
        // to simplify matching if bytes mess up
        new Random().nextBytes(data);
        final int correlationID = 0;

        PutFile msg = new PutFile();
        msg.setBulkData(data);
        msg.setCorrelationID(correlationID);
        proxy.sendRPCRequest(msg);

        Thread.sleep(100);

        final byte[] actualBytes = baos.toByteArray();

        final int jsonSize = msg.serializeJSON((byte) 2).toString().length();

        final byte[] firstFrame_0 =
                new byte[]{ 0x22, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
                        0x00, 0x00, 0x00, 0x01 };
        int offset = 0;
        assertBuffersEqual("Frame 1's header is incorrect", firstFrame_0,
                actualBytes, offset);
        offset += firstFrame_0.length;

        byte[] buf =
                intToBytes(dataSize + jsonSize + WiProProtocol.HEADER_SIZE);
        assertBuffersEqual("Frame 1's data0 is incorrect", buf, actualBytes,
                offset);
        offset += buf.length;

        buf = intToBytes(2);
        assertBuffersEqual("Frame 1's data1 is incorrect", buf, actualBytes,
                offset);
        offset += buf.length;


        buf = new byte[]{ 0x23, 0x0F, 0x01, 0x00 };
        assertBuffersEqual("Consecutive Frame 2's part1 is incorrect", buf,
                actualBytes, offset);
        offset += buf.length;

        buf = intToBytes(maxDataSize);
        assertBuffersEqual("Consecutive Frame 2's dataSize is incorrect", buf,
                actualBytes, offset);
        offset += buf.length;

        buf = new byte[]{ 0x00, 0x00, 0x00, 0x01 };
        assertBuffersEqual("Consecutive Frame 2's messageID is incorrect", buf,
                actualBytes, offset);
        offset += buf.length;

        final byte[] rpcHeaderBytes =
                new byte[]{ 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x02, '{', '}' };
        assertBuffersEqual("Consecutive Frame 2's RPC header is incorrect",
                rpcHeaderBytes, actualBytes, offset);
        offset += rpcHeaderBytes.length;

        buf = new byte[maxDataSize - rpcHeaderBytes.length];
        System.arraycopy(data, 0, buf, 0, maxDataSize - rpcHeaderBytes.length);
        assertBuffersEqual("Consecutive Frame 2's data is incorrect", buf,
                actualBytes, offset);
        offset += buf.length;
        final int dataLeft = dataSize - buf.length;


        buf = new byte[]{ 0x23, 0x0F, 0x00, 0x00 };
        assertBuffersEqual("Consecutive Frame 3's part1 is incorrect", buf,
                actualBytes, offset);
        offset += buf.length;

        buf = intToBytes(dataLeft);
        assertBuffersEqual("Consecutive Frame 3's dataSize is incorrect", buf,
                actualBytes, offset);
        offset += buf.length;

        buf = new byte[]{ 0x00, 0x00, 0x00, 0x01 };
        assertBuffersEqual("Consecutive Frame 3's messageID is incorrect", buf,
                actualBytes, offset);
        offset += buf.length;

//        assertBuffersEqual("Consecutive Frame 3's RPC header is incorrect",
//                rpcHeaderBytes, actualBytes, offset);
//        offset += rpcHeaderBytes.length;

        buf = new byte[dataLeft];
        System.arraycopy(data, dataSize - dataLeft, buf, 0, dataLeft);
        assertBuffersEqual("Consecutive Frame 3's data is incorrect", buf,
                actualBytes, offset);
    }

    private void assertBuffersEqual(String reason, byte[] expected,
                                    byte[] actual, int offset) {
        byte[] buf = new byte[expected.length];
        System.arraycopy(actual, offset, buf, 0, expected.length);
        assertThat(reason, buf, is(expected));
    }

    private byte[] intToBytes(int i) {
        byte[] res = new byte[4];
        res[0] = (byte) ((i >>> 24) & 0x000000FF);
        res[1] = (byte) ((i >>> 16) & 0x000000FF);
        res[2] = (byte) ((i >>> 8) & 0x000000FF);
        res[3] = (byte) (i & 0x000000FF);
        return res;
    }
}
