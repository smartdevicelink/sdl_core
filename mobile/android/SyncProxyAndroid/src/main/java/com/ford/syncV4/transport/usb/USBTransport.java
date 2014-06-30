package com.ford.syncV4.transport.usb;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbAccessory;
import android.hardware.usb.UsbManager;
import android.os.ParcelFileDescriptor;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.transport.SiphonServer;
import com.ford.syncV4.transport.SyncTransport;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.util.logger.Logger;

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Class that implements USB transport.
 * <p/>
 * A note about USB Accessory protocol. If the device is already in the USB
 * accessory mode, any side (computer or Android) can open connection even if
 * the other side is not connected. Conversely, if one side simply disconnects,
 * the other side will NOT be notified and unblocked from reading data until
 * some data is sent again or the USB is physically disconnected.
 */
public class USBTransport extends SyncTransport {
    /**
     * Broadcast action: sent when a USB accessory is attached.
     * <p/>
     * UsbManager.EXTRA_ACCESSORY extra contains UsbAccessory object that has
     * been attached.
     */
    public static final String ACTION_USB_ACCESSORY_ATTACHED =
            "com.ford.syncV4.USB_ACCESSORY_ATTACHED";

    private static final String CLASS_NAME = USBTransport.class.getSimpleName();

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
    private final static String ACCESSORY_MANUFACTURER = "Ford";
    /**
     * Model name of the accessory we want to connect to. Must be the same as
     * in accessory_filter.xml to work properly.
     */
    private final static String ACCESSORY_MODEL = "HMI";
    /**
     * Version of the accessory we want to connect to. Must be the same as in
     * accessory_filter.xml to work properly.
     */
    private final static String ACCESSORY_VERSION = "1.0";

    /**
     * Broadcast receiver that receives different USB-related intents: USB
     * accessory connected, disconnected, and permission granted.
     */
    private final BroadcastReceiver mUSBReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            Logger.d(CLASS_NAME + " USBReceiver Action: " + action);

            UsbAccessory accessory =
                    intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
            if (accessory != null) {
                if (ACTION_USB_ACCESSORY_ATTACHED.equals(action)) {
                    Logger.i(CLASS_NAME + " Accessory " + accessory + " attached");
                    if (isAccessorySupported(accessory)) {
                        connectToAccessory(accessory);
                    } else {
                        Logger.w(CLASS_NAME + " Attached accessory is not supported!");
                    }
                } else if (UsbManager.ACTION_USB_ACCESSORY_DETACHED
                        .equals(action)) {
                    Logger.i(CLASS_NAME + " Accessory " + accessory + " detached");
                    final String msg = "USB accessory has been detached";
                    disconnect(msg, new SyncException(msg,
                            SyncExceptionCause.SYNC_USB_DETACHED));
                } else if (ACTION_USB_PERMISSION.equals(action)) {
                    boolean permissionGranted = intent.getBooleanExtra(
                            UsbManager.EXTRA_PERMISSION_GRANTED, false);
                    if (permissionGranted) {
                        Logger.i(CLASS_NAME + " Permission granted for accessory " + accessory);
                        openAccessory(accessory);
                    } else {
                        final String msg =
                                " Permission denied for accessory " + accessory;
                        Logger.w(CLASS_NAME + msg);
                        disconnect(msg, new SyncException(msg,
                                SyncExceptionCause.SYNC_USB_PERMISSION_DENIED));
                    }
                }
            } else {
                Logger.w(CLASS_NAME + " Accessory is null");
            }
        }
    };
    /**
     * USB config object.
     */
    private USBTransportConfig mConfig = null;
    /**
     * Current state of transport.
     * <p/>
     * Use setter and getter to access it.
     */
    private State mState = State.IDLE;
    /**
     * Current accessory the transport is working with if any.
     */
    private UsbAccessory mAccessory = null;
    /**
     * FileDescriptor that owns the input and output streams. We have to keep
     * it, otherwise it will be garbage collected and the streams will become
     * invalid.
     */
    private ParcelFileDescriptor mParcelFD = null;
    /**
     * Data input stream to read data from USB accessory.
     */
    private InputStream mInputStream = null;
    /**
     * Data output stream to write data to USB accessory.
     */
    private OutputStream mOutputStream = null;

    public void setReaderThread(Thread mReaderThread) {
        this.mReaderThread = mReaderThread;
    }

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

    public Thread getReaderThread() {
        return mReaderThread;
    }

    /**
     * Returns the current state of transport.
     *
     * @return Current state of transport
     */
    public synchronized State getState() {
        return mState;
    }

    /**
     * Changes current state of transport.
     *
     * @param state New state
     */
    public synchronized void setState(State state) {
        Logger.d(CLASS_NAME + " Changing state " + mState + " to " + state);
        mState = state;
        if (mState == State.CONNECTED) {
            handleTransportConnected();
        }
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
        boolean result = false;
        final State state = getState();
        if (state != State.CONNECTED) {
            Logger.w(CLASS_NAME + " Can't send bytes from " + state + " state");
            return result;
        }
        if (mOutputStream != null) {
            try {
                mOutputStream.write(msgBytes, offset, length);
                result = true;

                Logger.d(CLASS_NAME + ": " + msgBytes.length + " bytes sent");
            } catch (IOException e) {
                final String msg = " Failed to send bytes over USB";
                Logger.w(CLASS_NAME + msg, e);
                handleTransportError(msg, e);
            }
        } else {
            final String msg = " Can't send bytes when output stream is null";
            Logger.w(CLASS_NAME + msg);
            handleTransportError(msg, null);
        }

        return result;
    }

    /**
     * Opens a USB connection if not open yet.
     *
     * @throws SyncException
     */
    @Override
    public void openConnection() throws SyncException {
        final State state = getState();
        if (state != State.IDLE) {
            Logger.w(CLASS_NAME + " openConnection() called from state " + state + "; doing nothing");
            return;
        }

        Logger.i(CLASS_NAME + " openConnection()");
        setState(State.LISTENING);

        Logger.d(CLASS_NAME + " Registering receiver");
        try {
            IntentFilter filter = new IntentFilter();
            filter.addAction(ACTION_USB_ACCESSORY_ATTACHED);
            filter.addAction(UsbManager.ACTION_USB_ACCESSORY_DETACHED);
            filter.addAction(ACTION_USB_PERMISSION);
            getContext().registerReceiver(mUSBReceiver, filter);

            initializeAccessory();
        } catch (Exception e) {
            String msg = " Couldn't start opening connection";
            Logger.e(CLASS_NAME + msg, e);
            throw new SyncException(msg, e, SyncExceptionCause.SYNC_CONNECTION_FAILED);
        }
    }

    /**
     * Closes the USB connection if open.
     */
    @Override
    public void disconnect() {
        disconnect(null, null);
    }

    /**
     * Asks the reader thread to stop while it's possible. If it's blocked on
     * read(), there is no way to stop it except for physical USB disconnect.
     */
    @Override
    public void stopReading() {
        Logger.i(CLASS_NAME + " Stop reading requested, doing nothing");
        stopUSBReading();
    }

    private void stopUSBReading() {
        final State state = getState();
        if (state != State.CONNECTED) {
            Logger.w(CLASS_NAME + " Stopping reading called from state " + state + "; doing nothing");
            return;
        }
        Logger.i(CLASS_NAME + " Stopping reading");
        stopReaderThread();
    }

    /**
     * Actually asks the reader thread to interrupt.
     */
    private void stopReaderThread() {
        if (mReaderThread != null) {
            Logger.i(CLASS_NAME + " Interrupting USB reader");
            mReaderThread.interrupt();
        } else {
            Logger.d(CLASS_NAME + " USB reader is null");
        }
    }

    /**
     * Closes the USB connection from inside the transport with some extra info.
     *
     * @param msg Disconnect reason message, if any
     * @param ex  Disconnect exception, if any
     */
    private void disconnect(String msg, Exception ex) {
        final State state = getState();
        if (state != State.LISTENING && state != State.CONNECTED) {
            Logger.w(CLASS_NAME + " Disconnect called from state " + state + "; doing nothing");
            return;
        }

        Logger.i(CLASS_NAME + " Disconnect from state " + getState() + "; message: " + msg + "; exception: " + ex);
        setState(State.IDLE);

        stopReaderThread();

        if (mAccessory != null) {
            if (mOutputStream != null) {
                try {
                    mOutputStream.close();
                } catch (IOException e) {
                    Logger.w(CLASS_NAME + " Can't close output stream", e);
                }
                mOutputStream = null;
            }
            if (mInputStream != null) {
                try {
                    mInputStream.close();
                } catch (IOException e) {
                    Logger.w(CLASS_NAME + " Can't close input stream", e);
                }
                mInputStream = null;
            }
            if (mParcelFD != null) {
                try {
                    mParcelFD.close();
                } catch (IOException e) {
                    Logger.w(CLASS_NAME + " Can't close file descriptor", e);
                }
                mParcelFD = null;
            }

            mAccessory = null;
        }

        Logger.d(CLASS_NAME + " Unregistering receiver");
        try {
            getContext().unregisterReceiver(mUSBReceiver);
        } catch (IllegalArgumentException e) {
            Logger.w(CLASS_NAME + " Receiver was already unregistered", e);
        }

        String disconnectMsg = (msg == null ? "" : msg);
        if (ex != null) {
            disconnectMsg += ", " + ex.toString();
        }

        if (ex == null) {
            // This disconnect was not caused by an error, notify the
            // proxy that the transport has been disconnected.
            Logger.i(CLASS_NAME + " Disconnect is correct. Handling it");
            handleTransportDisconnected(disconnectMsg);
        } else {
            // This disconnect was caused by an error, notify the proxy
            // that there was a transport error.
            Logger.i(CLASS_NAME + " Disconnect is incorrect. Handling it as error");
            handleTransportError(disconnectMsg, ex);
        }
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
        Logger.i(CLASS_NAME + " Looking for connected accessories");
        UsbManager usbManager = getUsbManager();
        UsbAccessory[] accessories = usbManager.getAccessoryList();
        if (accessories != null) {
            Logger.d(CLASS_NAME + " Found total " + accessories.length + " accessories");
            for (UsbAccessory accessory : accessories) {
                if (isAccessorySupported(accessory)) {
                    connectToAccessory(accessory);
                    break;
                }
            }
        } else {
            Logger.i(CLASS_NAME + " No connected accessories found");
        }
    }

    /**
     * Checks if the specified connected USB accessory is what we expect.
     *
     * @param accessory Accessory to check
     * @return true if the accessory is right
     */
    private boolean isAccessorySupported(UsbAccessory accessory) {
        boolean manufacturerMatches =
                ACCESSORY_MANUFACTURER.equals(accessory.getManufacturer());
        boolean modelMatches = ACCESSORY_MODEL.equals(accessory.getModel());
        boolean versionMatches =
                ACCESSORY_VERSION.equals(accessory.getVersion());
        return manufacturerMatches && modelMatches && versionMatches;
    }

    /**
     * Attempts to connect to the specified accessory.
     * <p/>
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
                    Logger.i(CLASS_NAME + " Already have permission to use " + accessory);
                    openAccessory(accessory);
                } else {
                    Logger.i(CLASS_NAME + " Requesting permission to use " + accessory);

                    PendingIntent permissionIntent = PendingIntent
                            .getBroadcast(getContext(), 0,
                                    new Intent(ACTION_USB_PERMISSION), 0);
                    usbManager.requestPermission(accessory, permissionIntent);
                }

                break;

            default:
                Logger.w(CLASS_NAME + " connectToAccessory() called from state " + state +
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
     * <p/>
     * When this function is called, the permission to use it must have already
     * been granted.
     *
     * @param accessory Accessory to open connection to
     */
    private void openAccessory(UsbAccessory accessory) {
        final State state = getState();
        if (state != State.LISTENING) {
            Logger.w(CLASS_NAME + " openAccessory() called from state " + state + "; doing nothing");
            return;
        }
        Logger.i(CLASS_NAME + " Opening accessory " + accessory);
        mAccessory = accessory;

        startReaderThread();

        // Initialize the SiphonServer
        //SiphonServer.init();
    }

    private void startReaderThread() {
        mReaderThread = new Thread(new USBTransportReader());
        mReaderThread.setDaemon(true);
        mReaderThread.setName(USBTransportReader.class.getSimpleName());
        mReaderThread.start();
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
    public enum State {
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
         * USB accessory attached; permission granted; data IO in progress.
         */
        CONNECTED
    }

    /**
     * Internal task that connects to and reads data from a USB accessory.
     * <p/>
     * Since the class has to have access to the parent class' variables,
     * synchronization must be taken in consideration! For now, all access
     * to variables of USBTransport must be surrounded with
     * synchronized (USBTransport.this) { … }
     */
    private class USBTransportReader implements Runnable {
        /**
         * String tag for logging inside the task.
         */
        private final String TAG = USBTransportReader.class.getSimpleName();

        /**
         * Checks if the thread has been interrupted.
         *
         * @return true if the thread has been interrupted
         */
        private boolean isInterrupted() {
            return Thread.interrupted();
        }

        /**
         * Entry function that is called when the task is started. It attempts
         * to connect to the accessory, then starts a read loop until
         * interrupted.
         */
        @Override
        public void run() {
            Logger.d(CLASS_NAME + " USB reader started!");

            if (isInterrupted()) {
                Logger.i(CLASS_NAME + " Thread is interrupted, not connecting");
                return;
            }

            final State state = getState();
            if (state != State.LISTENING) {
                Logger.w(CLASS_NAME + " State is:" + state + ", will not try to connect");
            }

            if (connect()) {
                readFromTransport();
            }

            Logger.d(CLASS_NAME + " USB reader finished!");
        }

        /**
         * Attempts to open connection to USB accessory.
         *
         * @return true if connected successfully
         */
        private boolean connect() {
            try {
                mParcelFD = getUsbManager().openAccessory(mAccessory);
            } catch (SecurityException e) {
                final String msg = " Have no permission to open the accessory";
                Logger.e(CLASS_NAME + msg, e);
                disconnect(msg, e);
                return false;
            }
            if (mParcelFD == null) {
                if (isInterrupted()) {
                    Logger.w(CLASS_NAME + " Can't open accessory, and thread is interrupted");
                } else {
                    Logger.w(CLASS_NAME + " Can't open accessory, disconnecting!");
                    String msg = "Failed to open USB accessory";
                    disconnect(msg, new SyncException(msg,
                            SyncExceptionCause.SYNC_CONNECTION_FAILED));
                }
                return false;
            }
            FileDescriptor fd = mParcelFD.getFileDescriptor();
            mInputStream = new FileInputStream(fd);
            mOutputStream = new FileOutputStream(fd);

            Logger.i(CLASS_NAME + " Accessory opened!");

            setState(State.CONNECTED);

            return true;
        }

        /**
         * Continuously reads data from the transport's input stream, blocking
         * when no data is available.
         */
        private void readFromTransport() {
            final int READ_BUFFER_SIZE = 4096;
            byte[] buffer = new byte[READ_BUFFER_SIZE];
            int bytesRead;

            // read loop
            while (!isInterrupted()) {
                try {
                    bytesRead = mInputStream.read(buffer);
                    if (bytesRead == -1) {
                        if (isInterrupted()) {
                            Logger.i(CLASS_NAME + " EOF reached, and thread is interrupted");
                        } else {
                            Logger.i(CLASS_NAME + " EOF reached, disconnecting!");
                            disconnect("EOF reached", null);
                        }
                        return;
                    }
                } catch (IOException e) {
                    if (isInterrupted()) {
                        Logger.w(CLASS_NAME + " Can't read data, and thread is interrupted", e);
                    } else {
                        Logger.w(CLASS_NAME + " Can't read data, disconnecting!", e);
                        disconnect("Can't read data from USB", e);
                    }
                    return;
                }

                Logger.d(CLASS_NAME + " Read " + bytesRead + " bytes");

                if (isInterrupted()) {
                    Logger.w(CLASS_NAME + " Read some data, but thread is interrupted");
                    return;
                }

                if (bytesRead > 0) {
                    handleReceivedBytes(buffer, bytesRead);
                }
            }

            Logger.d(CLASS_NAME + " Quit Read loop");
        }
    }
}