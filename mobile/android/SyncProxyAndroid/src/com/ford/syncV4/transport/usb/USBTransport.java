package com.ford.syncV4.transport.usb;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbAccessory;
import android.hardware.usb.UsbManager;
import android.os.ParcelFileDescriptor;
import android.util.Log;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.transport.SiphonServer;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TransportType;

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Class that implements USB transport.
 *
 * A note about USB Accessory protocol. If the device is already in the USB
 * accessory mode, any side (computer or Android) can open connection even if
 * the other side is not connected. The transport thus can't notify of
 * successful connection at that point yet. To workaround that, the transport
 * sends an initial request (one byte currently) and waits for a reply (one
 * byte as well). Then it notifies of a successful connection. It's not part of
 * the official SDL protocol specification at the moment.
 *
 * TODO: use SyncTrace and/or DebugTool loggers
 */
public class USBTransport extends SyncTransport {
    /**
     * Broadcast action: sent when a USB accessory is attached.
     *
     * UsbManager.EXTRA_ACCESSORY extra contains UsbAccessory object that has
     * been attached.
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
     * Manufacturer name of the accessory we want to connect to. Must be the
     * same as in accessory_filter.xml to work properly.
     */
    private final static String ACCESSORY_MANUFACTURER = "Nexus-Computing GmbH";
    /**
     * Broadcast receiver that receives different USB-related intents: USB
     * accessory connected, disconnected, and permission granted.
     */
    private final BroadcastReceiver mUSBReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            logD("USBReceiver Action: " + action);

            UsbAccessory accessory =
                    intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
            if (accessory != null) {
                if (ACTION_USB_ACCESSORY_ATTACHED.equals(action)) {
                    logI("Accessory " + accessory + " attached");
                    connectToAccessory(accessory);
                } else if (UsbManager.ACTION_USB_ACCESSORY_DETACHED
                        .equals(action)) {
                    logI("Accessory " + accessory + " detached");
                } else if (ACTION_USB_PERMISSION.equals(action)) {
                    logI("Permission granted for accessory " + accessory);
                    openAccessory(accessory);
                }
            } else {
                logW("Accessory is null");
            }
        }
    };
    /**
     * USB config object.
     */
    private USBTransportConfig mConfig = null;
    /**
     * Current state of transport.
     *
     * Use setter and getter to access it.
     */
    private State mState = State.IDLE;
    /**
     * Current accessory the transport is working with if any.
     */
    private UsbAccessory mAccessory = null;
    /**
     * Data input stream to read data from USB accessory.
     */
    private InputStream mInputStream = null;
    /**
     * Data output stream to write data to USB accessory.
     */
    private OutputStream mOutputStream = null;
    /**
     * Thread that connects and reads data from USB accessory.
     *
     * @see USBTransportReader
     */
    private Thread mReaderThread = null;

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
     * Returns the current state of transport.
     *
     * @return Current state of transport
     */
    public State getState() {
        return this.mState;
    }

    /**
     * Changes current state of transport.
     *
     * @param state New state
     */
    private void setState(State state) {
        logD("Changing state " + this.mState + " to " + state);
        this.mState = state;
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
        logD("SendBytes: array size " + msgBytes.length + ", offset " + offset +
                ", length " + length);

        boolean result = false;
        final State state = getState();
        switch (state) {
            case CONNECTED:
                synchronized (this) {
                    if (mOutputStream != null) {
                        try {
                            mOutputStream.write(msgBytes, offset, length);
                            result = true;
                            logI("Bytes successfully sent");
                        } catch (IOException e) {
                            logW("Failed to send bytes", e);
                        }
                    } else {
                        logW("Can't send bytes when output stream is null");
                    }
                }
                break;

            default:
                logW("Can't send bytes from " + state + " state");
                break;
        }

        return result;
    }

    /**
     * Opens a USB connection if not open yet.
     *
     * TODO: throw SyncException on different errors
     *
     * @throws SyncException
     */
    @Override
    public void openConnection() throws SyncException {
        final State state = getState();
        switch (state) {
            case IDLE:
                synchronized (this) {
                    logI("openConnection()");
                    setState(State.LISTENING);
                }

                logD("Registering receiver");
                IntentFilter filter = new IntentFilter();
                filter.addAction(ACTION_USB_ACCESSORY_ATTACHED);
                filter.addAction(UsbManager.ACTION_USB_ACCESSORY_DETACHED);
                filter.addAction(ACTION_USB_PERMISSION);
                getContext().registerReceiver(mUSBReceiver, filter);

                initializeAccessory();

                break;

            default:
                logW("openConnection() called from state " + state +
                        "; doing nothing");
                break;
        }
    }

    /**
     * Closes the USB connection if open.
     */
    @Override
    public void disconnect() {
        synchronized (this) {
            logI("Disconnect from state " + getState());
            setState(State.IDLE);

            if (mReaderThread != null) {
                logI("Interrupting USB reader");
                mReaderThread.interrupt();
                // don't join() now
                mReaderThread = null;
            } else {
                logD("USB reader is null");
            }

            if (mAccessory != null) {
                if (mOutputStream != null) {
                    try {
                        mOutputStream.close();
                    } catch (IOException e) {
                        logW("Can't close output stream", e);
                        mOutputStream = null;
                    }
                }
                if (mInputStream != null) {
                    try {
                        mInputStream.close();
                    } catch (IOException e) {
                        logW("Can't close input stream", e);
                        mInputStream = null;
                    }
                }

                mAccessory = null;
            }
        }

        logD("Unregistering receiver");
        getContext().unregisterReceiver(mUSBReceiver);

        // TODO: use proper message
        handleTransportDisconnected("");
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
     * Looks for an already connected compatible accessory and connect to it.
     */
    private void initializeAccessory() {
        logI("Looking for connected accessories");
        UsbManager usbManager = getUsbManager();
        UsbAccessory[] accessories = usbManager.getAccessoryList();
        if (accessories != null) {
            logD("Found total " + accessories.length + " accessories");
            for (UsbAccessory accessory : accessories) {
                if (ACCESSORY_MANUFACTURER
                        .equals(accessory.getManufacturer())) {
                    connectToAccessory(accessory);
                    break;
                }
            }
        } else {
            logI("No connected accessories found");
        }
    }

    /**
     * Attempts to connect to the specified accessory.
     *
     * If the permission is already granted, opens the accessory. Otherwise,
     * requests permission to use it.
     *
     * @param accessory Accessory to connect to
     */
    private void connectToAccessory(UsbAccessory accessory) {
        final State state = getState();
        switch (state) {
            case LISTENING:
                UsbManager usbManager = getUsbManager();
                if (usbManager.hasPermission(accessory)) {
                    logI("Already have permission to use " + accessory);
                    openAccessory(accessory);
                } else {
                    logI("Requesting permission to use " + accessory);
                    PendingIntent permissionIntent = PendingIntent
                            .getBroadcast(getContext(), 0,
                                    new Intent(ACTION_USB_PERMISSION), 0);
                    usbManager.requestPermission(accessory, permissionIntent);
                }

                break;

            default:
                logW("connectToAccessory() called from state " + state +
                        "; doing nothing");
        }
    }

    /**
     * Returns the UsbManager to use with accessories.
     *
     * @return System UsbManager
     */
    private UsbManager getUsbManager() {
        return (UsbManager) getContext().getSystemService(Context.USB_SERVICE);
    }

    /**
     * Opens a connection to the accessory.
     *
     * When this function is called, the permission to use it must have already
     * been granted.
     *
     * @param accessory Accessory to open connection to
     */
    private void openAccessory(UsbAccessory accessory) {
        final State state = getState();
        switch (state) {
            case LISTENING:
                synchronized (this) {
                    logI("Opening accessory " + accessory);
                    setState(State.CONNECTING);
                    mAccessory = accessory;

                    mReaderThread = new Thread(new USBTransportReader());
                    mReaderThread.setDaemon(true);
                    mReaderThread
                            .setName(USBTransportReader.class.getSimpleName());
                    mReaderThread.start();

                    // Initialize the SiphonServer
                    SiphonServer.init();
                }

                break;

            default:
                logW("openAccessory() called from state " + state +
                        "; doing nothing");
        }
    }

    /**
     * Logs the string with WARN level.
     *
     * @param s string to log
     */
    private void logW(String s) {
        Log.w(TAG, s);
    }

    /**
     * Logs the string and the throwable with WARN level.
     *
     * @param s  string to log
     * @param tr throwable to log
     */
    private void logW(String s, Throwable tr) {
        Log.w(TAG, s, tr);
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

    /**
     * Possible states of the USB transport.
     */
    private enum State {
        /**
         * Transport initialized; no connections.
         */
        IDLE,

        /**
         * USB accessory not attached; SyncProxy wants connection as soon as
         * accessory is attached.
         */
        LISTENING,

        /**
         * USB accessory attached; permission granted; initial request sent;
         * waiting for reply to make sure the USB accessory is live there.
         */
        CONNECTING,

        /**
         * Reply received; data IO in progress.
         */
        CONNECTED
    }

    /**
     * Internal task that connects to and reads data from a USB accessory.
     *
     * Since the class has to have access to the parent class' variables,
     * synchronization must be taken in consideration! For now, all access
     * to variables of USBTransport must be surrounded with
     * synchronized (USBTransport.this) { … }
     */
    private class USBTransportReader implements Runnable {
        public static final char INITIAL_BYTE = '?';
        /**
         * String tag for logging inside the task.
         */
        private final String TAG = USBTransportReader.class.getSimpleName();

        /**
         * Entry function that is called when the task is started. It attempts
         * to connect to the accessory, sends an initial request (to make sure
         * there is someone on the other side), waits for a reply, and starts
         * a read loop until interrupted.
         *
         * TODO: add isInterrupted checks
         */
        @Override
        public void run() {
            Log.d(TAG, "USB reader started!");

            FileDescriptor fd;
            synchronized (USBTransport.this) {
                final ParcelFileDescriptor parcelFD =
                        getUsbManager().openAccessory(mAccessory);
                if (parcelFD == null) {
                    Log.w(TAG, "Can't open accessory, disconnecting!");
                    disconnect();
                    return;
                }
                fd = parcelFD.getFileDescriptor();
                mInputStream = new FileInputStream(fd);
                mOutputStream = new FileOutputStream(fd);
            }
            Log.i(TAG, "Accessory opened!");

            // sending initial request "Is there anybody out there?"
            try {
                mOutputStream.write(new byte[]{ INITIAL_BYTE });
                Log.d(TAG, "Initial request sent, waiting for a reply");
            } catch (IOException e) {
                Log.w(TAG, "Can't send initial request, disconnecting!", e);
                disconnect();
                return;
            }

            // waiting for a one-byte reply
            byte reply;
            try {
                int intReply = mInputStream.read();
                if (intReply == -1) {
                    Log.w(TAG, "Can't read initial response, EOF reached" +
                            ", disconnecting!");
                    disconnect();
                    return;
                }
                reply = (byte) intReply;
            } catch (IOException e) {
                Log.w(TAG, "Can't read initial response, disconnecting!", e);
                disconnect();
                return;
            }

            Log.d(TAG, "Received reply: " + reply);
            synchronized (USBTransport.this) {
                setState(State.CONNECTED);
                handleTransportConnected();
            }

            final int READ_BUFFER_SIZE = 4096;
            byte[] buffer = new byte[READ_BUFFER_SIZE];
            int bytesRead = 0;

            // read loop
            while (true) {
                try {
                    bytesRead = mInputStream.read(buffer);
                    if (bytesRead == -1) {
                        Log.i(TAG, "EOF reached, disconnecting!");
                        disconnect();
                        return;
                    }
                } catch (IOException e) {
                    Log.w(TAG, "Can't read data, disconnecting!", e);
                    disconnect();
                    return;
                }

                Log.d(TAG, "Read " + bytesRead + " bytes");
                if (bytesRead > 0) {
                    synchronized (USBTransport.this) {
                        handleReceivedBytes(buffer, bytesRead);
                    }
                }
            }
        }
    }
}
