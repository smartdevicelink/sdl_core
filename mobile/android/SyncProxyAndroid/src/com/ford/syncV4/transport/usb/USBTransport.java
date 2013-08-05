package com.ford.syncV4.transport.usb;

import android.util.Log;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TransportType;

import java.util.Arrays;

/**
 * Class that implements USB transport.
 */
public class USBTransport extends SyncTransport {
    /**
     * String tag for logging.
     */
    private static final String TAG = USBTransport.class.getSimpleName();
    /**
     * USB config object.
     */
    private USBTransportConfig mConfig = null;

    /**
     * Constructs the USBTransport instance.
     *
     * @param usbTransportConfig Config object for the USB transport
     * @param transportListener  Listener that gets notified on different transport events
     */
    public USBTransport(USBTransportConfig usbTransportConfig,
                        ITransportListener transportListener) {
        super(transportListener);
        this.mConfig = usbTransportConfig;
    }

    /**
     * Sends the array of bytes over USB.
     *
     * @param msgBytes Array of bytes to send
     * @param offset   Offset in the array to start from
     * @param length   Number of bytes to send
     * @return true if the bytes are sent successfully
     */
    @Override
    protected boolean sendBytesOverTransport(byte[] msgBytes, int offset,
                                             int length) {
        logD("sendBytesOverTransport() " + Arrays.toString(msgBytes) +
                ", offset " + offset + ", length " + length);
        return false;
    }

    /**
     * Opens a USB connection if not open yet.
     *
     * @throws SyncException
     */
    @Override
    public void openConnection() throws SyncException {
        logD("openConnection()");
    }

    /**
     * Closes the USB connection if open.
     */
    @Override
    public void disconnect() {
        logD("disconnect()");
    }

    /**
     * Returns the type of the transport.
     *
     * @return TransportType.USB
     * @see com.ford.syncV4.transport.TransportType
     */
    @Override
    public TransportType getTransportType() {
        return TransportType.USB;
    }

    /**
     * Logs the string with DEBUG level.
     *
     * @param s string to log
     */
    private void logD(String s) {
        Log.d(TAG, s);
    }
}
