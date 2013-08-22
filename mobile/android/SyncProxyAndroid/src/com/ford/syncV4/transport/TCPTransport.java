package com.ford.syncV4.transport;

import android.util.Log;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.Timer;
import java.util.TimerTask;

/**
 * General comments:
 *
 * 1) Transport layer can be reorganized to properly incapsulate thread-related code according to Android OS guidelines
 * 2) Currently there are different cases when transport error information sent to listener components
 *      a) when there are some errors during writing data to transport
 *      b) when there are errors during connection establishing/reading data
 *    But information about transport disconnection is sent only if disconnection was successful. So we have following
 *    sequences:
 *    a) handleTransportConnected -> read without errors -> handleTransportDisconnected
 *    b) handleTransportConnected -> handleTransportError(write with errors) -> handleTransportDisconnected
 *    c) handleTransportConnected -> handleTransportError(read with errors) -> handleTransportError(socket closed)
 *
 *    They can be changed to be more natural and easy to use.
 *
 * 3) Public api is inconsistent. During single call of some api method (for example "openConnection") some of the
 *    following result can appears:
 *      a) SyncException thrown
 *      b) onTransportError callback called on listeners
 *
 * 4) Handling of connection must be more stable
 * 5) General solution in case of reconnecting must be implemented
 * 6) It must be common and same solution for handling information about physical device adapters (BT, WiFi etc.)
 */

/**
 * Class that implements TCP transport
 */
public class TCPTransport extends SyncTransport {

    /**
     * Size of the read buffer.
     */
    private static final int READ_BUFFER_SIZE = 4096;

    /**
     * Delay between reconnect attempts
     */
    private static final int RECONNECT_DELAY = 5000;

    /**
     * Count of the reconnect retries
     */
    private static final int RECONNECT_RETRY_COUNT = 30;

    /**
     * Instance of TCP transport configuration
     */
    private TCPTransportConfig mConfig = null;

    /**
     * Instance of the client socket
     */
    private Socket mSocket = null;

    /**
     * Instance of the input stream. Used to read data from ApplinkCore
     */
    private InputStream mInputStream = null;

    /**
     * Instance of the output stream. Used to send data to ApplinkCore
     */
    private OutputStream mOutputStream = null;

    /**
     * Instance of the separate thread, that does actual work, related to connecting/reading/writing data
     */
    private TCPTransportThread mThread = null;

    /**
     * Initial internal state of the component. Used like a simple lightweight FSM replacement while component
     * must behave differently in response to it's public function calls depending of it's current state
     */
    private TCPTransportState mCurrentState = TCPTransportState.IDLE;

    /**
     * Constructs TCP transport component instance
     *
     * @param tcpTransportConfig Instance of the TCP transport configuration
     * @param transportListener Listener that will be notified on different TCP transport activities
     */
    public TCPTransport(TCPTransportConfig tcpTransportConfig, ITransportListener transportListener) {
        super(transportListener);
        this.mConfig = tcpTransportConfig;
    }

    /**
     * Performs actual work of sending array of bytes over the transport
     * @param msgBytes Bytes to send
     * @param offset Offset in the bytes array to send data from
     * @param length Number of bytes to send
     * @return True if data was sent successfully, False otherwise
     */
    @Override
    protected boolean sendBytesOverTransport(byte[] msgBytes, int offset, int length) {
        TCPTransportState currentState = getCurrentState();
        logInfo(String.format("TCPTransport: sendBytesOverTransport requested. Size: %d, Offset: %d, Length: %d, Current state is: %s"
                , msgBytes.length, offset, length, currentState.name()));

        boolean bResult = false;

        if(currentState == TCPTransportState.CONNECTED) {
            synchronized (this) {
                if (mOutputStream != null) {
                    logInfo("TCPTransport: sendBytesOverTransport request accepted. Trying to send data");
                    try {
                        mOutputStream.write(msgBytes, offset, length);
                        bResult = true;
                        logInfo("TCPTransport.sendBytesOverTransport: successfully send data");
                    } catch (IOException e) {
                        logWarning("TCPTransport.sendBytesOverTransport: error during sending data: " + e.getMessage());
                        bResult = false;
                    }
                } else {
                    logError("TCPTransport: sendBytesOverTransport request accepted, but output stream is null");
                }
            }
        } else {
            logInfo("TCPTransport: sendBytesOverTransport request rejected. Transport is not connected");
            bResult = false;
        }

        return bResult;
    }

    /**
     * Tries to open connection to ApplinkCore.
     * Actual try will be performed only if no actual connection is available
     * @throws SyncException
     */
    @Override
    public void openConnection() throws SyncException {
        TCPTransportState currentState = getCurrentState();
        logInfo(String.format("TCPTransport: openConnection requested. Current state is: %s", currentState.name()));

        if(currentState == TCPTransportState.IDLE) {
            synchronized (this) {
                setCurrentState(TCPTransportState.CONNECTING);
                logInfo("TCPTransport: openConnection request accepted. Starting transport thread");
                try {
                    mThread = new TCPTransportThread();
                    mThread.setName(TCPTransportThread.class.getSimpleName());
                    mThread.setDaemon(true);
                    mThread.start();

                    // Initialize the SiphonServer
                    SiphonServer.init();
                } catch (Exception e) {
                    logError("TCPTransport: Exception during transport thread starting", e);
                    throw new SyncException(e);
                }
            }
        } else {
            logInfo("TCPTransport: openConnection request rejected. Another connection is not finished");
        }
    }


    /**
     * Tries to disconnect from ApplinkCore.
     * Actual try will be performed only if connection is available
     */
    @Override
    public void disconnect() {
        TCPTransportState currentState = getCurrentState();
        logInfo(String.format("TCPTransport: disconnect requested from client. Current state is: %s", currentState.name()));

        if(currentState == TCPTransportState.CONNECTED) {
            logInfo("TCPTransport: disconnect request accepted.");
            synchronized (this) {
                disconnect(null, null, true);
            }
        } else {
            logInfo("TCPTransport: disconnect request rejected. Transport is not connected");
        }
    }

    /**
     * Performs actual work related to disconnecting from ApplinkCore.
     *
     * @param message Message that describes disconnect reason
     * @param exception Some of the possible exceptions that was the reason of disconnecting
     * @param stopThread True if not only disconnection must be done but also thread that handles connection must be
     *                   also stopped so no reconnect attempts will be made
     */
    private synchronized void disconnect(String message, final Exception exception, boolean stopThread) {

        if(getCurrentState() == TCPTransportState.DISCONNECTING) {
            logInfo("TCPTransport: disconnecting already in progress");
            return;
        }

        setCurrentState(TCPTransportState.DISCONNECTING);

        String disconnectMsg = (message == null ? "" : message);
        if (exception != null) {
            disconnectMsg += ", " + exception.toString();
        }

        try {
            if(mThread != null && stopThread) {
                mThread.halt();
                mThread.interrupt();
            }

            if(mSocket != null){
                mSocket.close();
            }
            mSocket = null;
        } catch (IOException e) {
            logInfo("TCPTransport.disconnect: Exception during disconnect: " + e.getMessage());
        }

        if (exception == null) {
            // This disconnect was not caused by an error, notify the proxy that
            // the transport has been disconnected.
            logInfo("Disconnect is correct. Handling it");
            handleTransportDisconnected(disconnectMsg);
        } else {
            // This disconnect was caused by an error, notify the proxy
            // that there was a transport error.
            logInfo("Disconnect is incorrect. Handling it as error");
            final String finalDisconnectMsg = disconnectMsg;
            new Timer().schedule(new TimerTask() {
                @Override
                public void run() {
                    handleTransportError(finalDisconnectMsg, exception);
                }
            }, 0);
        }
    }

    /**
     * Overridden abstract method which returns specific type of this transport.
     *
     * @return Constant value - TransportType.TCP.
     *
     * @see TransportType
     */
    public TransportType getTransportType() {
        return TransportType.TCP;
    }

    /**
     * Internal method for logging information messages
     * @param message Message to log
     */
    protected void logInfo(String message) {
        Log.i(getClass().getName(), message);
    }

    /**
     * Internal method for logging error messages
     * @param message Message to log
     */
    protected void logError(String message) {
        Log.e(getClass().getName(), message);
    }

    /**
     * Internal method for logging warning messages
     * @param message Message to log
     */
    protected void logWarning(String message) {
        Log.w(getClass().getName(), message);
    }

    /**
     * Internal method for logging error message together with information about exception that was the reason of it
     * @param message Message to log
     * @param throwable Exception, that was the main reason for logged error message
     */
    protected void logError(String message, Throwable throwable) {
        Log.e(getClass().getName(), message, throwable);
    }

    /**
     * Internal class that represents separate thread, that does actual work, related to connecting/reading/writing data
     */
    private class TCPTransportThread extends Thread {

        /**
         * Represents current thread state - halted or not. This flag is used to change internal behavior depending
         * on current state.
         */
        private Boolean isHalted = false;

        /**
         * Method that marks thread as halted.
         */
        public void halt() {
            isHalted = true;
        }

        /**
         * Tries to connect to the applink core. Behavior depends autoReconnect configuration param:
         *      a) If autoReconnect is false, then only one connect try will be performed.
         *      b) If autoReconnect is true, then in case of connection error continuous reconnect will be performed
         *          after short delay until connection will be established or retry count will be reached
         *
         * @return true if connection established and false otherwise
         */
        private boolean connect() {
            boolean bConnected;
            int remainingRetry = RECONNECT_RETRY_COUNT;

            synchronized (TCPTransport.this) {
                do {
                    try {

                        if ((null != mSocket) && (!mSocket.isClosed())) {
                            logInfo("TCPTransport.connect: Socket is not closed. Trying to close it");
                            mSocket.close();
                        }

                        logInfo(String.format("TCPTransport.connect: Socket is closed. Trying to connect to %s", mConfig));
                        mSocket = new Socket();
                        mSocket.connect(new InetSocketAddress(mConfig.getIPAddress(), mConfig.getPort()));
                        mOutputStream = mSocket.getOutputStream();
                        mInputStream = mSocket.getInputStream();

                    } catch (IOException e) {
                        logInfo("TCPTransport.connect: Exception during connect stage: " + e.getMessage());
                    }

                    bConnected = (null != mSocket) && mSocket.isConnected();

                    if(bConnected){
                        logInfo("TCPTransport.connect: Socket connected");
                    }else{
                        if(mConfig.getAutoReconnect()){
                            remainingRetry--;
                            logInfo(String.format("TCPTransport.connect: Socket not connected. AutoReconnect is ON. retryCount is: %d. Waiting for reconnect delay: %d"
                                    , remainingRetry, RECONNECT_DELAY));
                            waitFor(RECONNECT_DELAY);
                        } else {
                            logInfo("TCPTransport.connect: Socket not connected. AutoReconnect is OFF");
                        }
                    }
                } while ((!bConnected) && (mConfig.getAutoReconnect()) && (remainingRetry > 0) && (!isHalted));

                return bConnected;
            }
        }

        /**
         * Performs actual thread work
         */
        @Override
        public void run() {
            logInfo("TCPTransport.run: transport thread created. Starting connect stage");

            while(!isHalted) {
                setCurrentState(TCPTransportState.CONNECTING);
                if(!connect()){
                    if (isHalted) {
                        logInfo("TCPTransport.run: Connection failed, but thread already halted");
                    } else {
                        disconnect("Failed to connect to Sync", new SyncException("Failed to connect to Sync"
                                , SyncExceptionCause.SYNC_CONNECTION_FAILED), true);
                    }
                    break;
                }

                synchronized (TCPTransport.this) {
                    setCurrentState(TCPTransportState.CONNECTED);
                    handleTransportConnected();
                }

                byte[] buffer = new byte[READ_BUFFER_SIZE];

                while (!isHalted) {
                    logInfo("TCPTransport.run: Waiting for data...");
                    int bytesRead;
                    try {
                        bytesRead = mInputStream.read(buffer);
                    } catch (IOException e) {
                        internalHandleStreamReadError();
                        break;
                    }

                    synchronized (TCPTransport.this) {
                        if (mThread.isInterrupted()) {
                            logInfo("TCPTransport.run: Got new data but thread is interrupted");
                            break;
                        }
                    }

                    logInfo("TCPTransport.run: Got new data");
                    if (-1 == bytesRead) {
                        internalHandleTCPDisconnect();
                        break;
                    } else if (0 == bytesRead) {
                        logInfo("TCPTransport.run: Received zero bytes");
                    } else {
                        logInfo(String.format("TCPTransport.run: Received %d bytes", bytesRead));
                        synchronized (TCPTransport.this) {
                            handleReceivedBytes(buffer, bytesRead);
                        }
                    }
                }
            }

            logInfo("TCPTransport.run: Thread terminated");
            setCurrentState(TCPTransportState.IDLE);
        }

        /**
         * Internal handling of Tcp disconnection
         */
        private void internalHandleTCPDisconnect() {
            if(isHalted){
                logInfo("TCPTransport.run: TCP disconnect received, but thread already halted");
            } else {
                logInfo("TCPTransport.run: TCP disconnect received");
                disconnect("TCPTransport.run: End of stream reached", null, false);
            }
        }

        /**
         * Internal handling of reading data from input stream
         */
        private void internalHandleStreamReadError() {
            if(isHalted){
                logInfo("TCPTransport.run: Exception during reading data, but thread already halted");
            } else {
                logInfo("TCPTransport.run: Exception during reading data");
                disconnect("Failed to read data from Sync", new SyncException("Failed to read data from Sync"
                        , SyncExceptionCause.SYNC_CONNECTION_FAILED), false);
            }
        }
    }

    /**
     * Returns current TCP transport state
     *
     * @return current state
     */
    private synchronized TCPTransportState getCurrentState() {
        return mCurrentState;
    }

    /**
     * Sets current TCP transport state
     * @param currentState New state
     */
    private synchronized void setCurrentState(TCPTransportState currentState) {
        logInfo(String.format("Current state changed to: %s", currentState));
        this.mCurrentState = currentState;
    }

    /**
     * Implementation of waiting required delay that cannot be interrupted
     * @param timeMs Time in milliseconds of required delay
     */
    private void waitFor(long timeMs) {
        long endTime = System.currentTimeMillis() +timeMs;
        while (System.currentTimeMillis() < endTime) {
            synchronized (this) {
                try {
                    wait(endTime - System.currentTimeMillis());
                } catch (Exception e) {
                    // Nothing To Do, simple wait
                }
            }
        }
    }

    /**
     * Defines available states of the TCP transport
     */
    private enum TCPTransportState {
        /**
         * TCP transport is created. No connection opened
         */
        IDLE,

        /**
         * TCP transport is in progress of establishing connection.
         */
        CONNECTING,

        /**
         * TCP transport is connected to applink core
         */
        CONNECTED,

        /**
         * TCP transport is in progress of disconnecting
         */
        DISCONNECTING
    }
} // end-class
