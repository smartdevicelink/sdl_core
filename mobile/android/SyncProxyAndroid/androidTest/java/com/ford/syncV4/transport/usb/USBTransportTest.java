package com.ford.syncV4.transport.usb;

import android.test.AndroidTestCase;

import com.ford.syncV4.transport.ITransportListener;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Created by Andrew Batutin on 1/8/14.
 */
public class USBTransportTest extends AndroidTestCase {

    public void testStopReadingCausesReaderThreadToStop() throws Exception {

        final ITransportListener transportListener = new ITransportListener() {
            @Override
            public void onTransportBytesReceived(byte[] receivedBytes,
                                                 int receivedBytesLength) {
            }

            @Override
            public void onTransportConnected() {
            }

            @Override
            public void onTransportDisconnected(String info) {
            }

            @Override
            public void onTransportError(String info, Exception e) {
            }

            @Override
            public void onServerSocketInit(int serverSocketPort) {

            }
        };

        USBTransport usbTransport = new USBTransport(null, transportListener);
        Thread thread = mock(Thread.class);
        when(thread.isInterrupted()).thenReturn(true);
        usbTransport.setReaderThread(thread);
        usbTransport.setState(USBTransport.State.CONNECTED);
        usbTransport.stopReading();
        boolean isInterrupted = usbTransport.getReaderThread().isInterrupted();
        assertEquals("thread should be interrupted", true, isInterrupted);
    }
}
