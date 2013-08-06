package com.ford.syncV4.transport.usb;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbManager;
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
     * Broadcast action: sent when a USB accessory is attached.
     */
    public static final String ACTION_USB_ACCESSORY_ATTACHED =
            "com.ford.syncV4.USB_ACCESSORY_ATTACHED";
    /**
     * String tag for logging.
     */
    private static final String TAG = USBTransport.class.getSimpleName();
    /**
     * Broadcast action: sent when the user has granted access to the USB
     * accessory.
     */
    private static final String ACTION_USB_PERMISSION =
            "com.ford.syncV4.USB_PERMISSION";
    /**
     * Broadcast receiver that receives different USB-related intents: USB
     * accessory connected, disconnected, and permission granted.
     */
    private final BroadcastReceiver mUSBReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            logI("USBReceiver Action: " + action);
        }
    };
    /**
     * USB config object.
     */
    private USBTransportConfig mConfig = null;

    /**
     * Constructs the USBTransport instance.
     *
     * @param usbTransportConfig Config object for the USB transport
     * @param transportListener  Listener that gets notified on different
     *                           transport events
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
        logI("sendBytesOverTransport() " + Arrays.toString(msgBytes) +
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
        logI("openConnection()");

        logD("Registering receiver");
        IntentFilter filter = new IntentFilter();
        filter.addAction(ACTION_USB_ACCESSORY_ATTACHED);
        filter.addAction(UsbManager.ACTION_USB_ACCESSORY_DETACHED);
        filter.addAction(ACTION_USB_PERMISSION);
        getContext().registerReceiver(mUSBReceiver, filter);
    }

    /**
     * Closes the USB connection if open.
     */
    @Override
    public void disconnect() {
        logI("disconnect()");

        logD("Unregistering receiver");
        getContext().unregisterReceiver(mUSBReceiver);
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
     * Logs the string with INFO level.
     *
     * @param s string to log
     */
    private void logI(String s) {
        Log.i(TAG, s);
    }

    /**
     * Logs the string with DEBUG level.
     *
     * @param s string to log
     */
    private void logD(String s) {
        Log.d(TAG, s);
    }

    /**
     * Returns Context to communicate with the OS.
     *
     * @return current context to be used by the USB transport
     */
    private Context getContext() {
        return mConfig.getContext();
    }
}
