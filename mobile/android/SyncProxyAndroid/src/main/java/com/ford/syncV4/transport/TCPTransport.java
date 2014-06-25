package com.ford.syncV4.transport;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.util.logger.Logger;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;

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

    private static final String CLASS_NAME = TCPTransport.class.getSimpleName();

    /**
     * Size of the read buffer.
     */
    private static final int READ_BUFFER_SIZE = 4096;

    /**
     * Instance of TCP transport configuration
     */
    private TCPTransportConfig mConfig = null;

    /**
     * Instance of the client socket
     */
    private Socket mSocket = null;

    /**
     * Instance of the server socket
     */
    private ServerSocket mServerSocket = null;

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
     * @param transportListener  Listener that will be notified on different TCP transport activities
     */
    public TCPTransport(TCPTransportConfig tcpTransportConfig, ITransportListener transportListener) {
        super(transportListener);
        this.mConfig = tcpTransportConfig;
    }

    /**
     * Performs actual work of sending array of bytes over the transport
     *
     * @param msgBytes Bytes to send
     * @param offset   Offset in the bytes array to send data from
     * @param length   Number of bytes to send
     * @return True if data was sent successfully, False otherwise
     */
    @Override
    protected boolean sendBytesOverTransport(byte[] msgBytes, int offset, int length) {
        TCPTransportState currentState = getCurrentState();
        //Logger.i(String.format("TCPTransport: sendBytesOverTransport requested. Size: %d, Offset: %d, Length: %d, Current state is: %s"
        //        , msgBytes.length, offset, length, currentState.name()));

        boolean bResult = false;

        if (currentState == TCPTransportState.CONNECTED) {
            synchronized (this) {
                if (mOutputStream != null) {
                    //Logger.i("TCPTransport: sendBytesOverTransport request accepted. Trying to send data");
                    try {
                        mOutputStream.write(msgBytes, offset, length);
                        bResult = true;
                        //Logger.i("TCPTransport.sendBytesOverTransport: successfully send data:" + msgBytes.length);
                    } catch (IOException e) {
                        Logger.e(CLASS_NAME + " SendBytesOverTransport: error during sending data: " + e.getMessage());
                        bResult = false;
                    }
                } else {
                    Logger.e(CLASS_NAME + " SendBytesOverTransport request accepted, but output stream is null");
                }
            }
        } else {
            Logger.i(CLASS_NAME + " SendBytesOverTransport request rejected. Transport is not connected");
            bResult = false;
        }

        return bResult;
    }

    /**
     * Tries to open connection to ApplinkCore.
     * Actual try will be performed only if no actual connection is available
     *
     * @throws SyncException
     */
    @Override
    public void openConnection() throws SyncException {
        TCPTransportState currentState = getCurrentState();
        Logger.i(String.format("TCPTransport: openConnection requested. Current state is: %s", currentState.name()));

        if (currentState == TCPTransportState.IDLE) {
            setCurrentState(TCPTransportState.CONNECTING);
            Logger.i("TCPTransport: openConnection request accepted. Starting transport thread");
            try {
                mThread = new TCPTransportThread();
                mThread.setName(TCPTransportThread.class.getSimpleName());
                mThread.setDaemon(true);
                mThread.start();

                // Initialize the SiphonServer
                //SiphonServer.init();
            } catch (Exception e) {
                Logger.e("TCPTransport: Exception during transport thread starting", e);
                throw new SyncException(e);
            }
        } else {
            Logger.i("TCPTransport: openConnection request rejected. Another connection is not finished");
        }
    }


    /**
     * Tries to disconnect from ApplinkCore.
     * Actual try will be performed only if connection is available
     */
    @Override
    public void disconnect() {
        TCPTransportState currentState = getCurrentState();
        Logger.i(String.format("TCPTransport: disconnect requested from client. Current state is: %s", currentState.name()));

        if (currentState == TCPTransportState.CONNECTED || currentState == TCPTransportState.CONNECTING) {
            Logger.i("TCPTransport: disconnect request accepted.");
            disconnect(null, null);
        } else {
            Logger.i("TCPTransport: disconnect request rejected. Transport is not connected");
        }
    }

    @Override
    public void stopReading() {
        Logger.i("TCPTransport: stop reading requested, doing nothing");
    }

    /**
     * Performs actual work related to disconnecting from ApplinkCore.
     *
     * @param message   Message that describes disconnect reason
     * @param exception Some of the possible exceptions that was the reason of disconnecting
     */
    private void disconnect(String message, Exception exception) {

        if (getCurrentState() == TCPTransportState.DISCONNECTING) {
            Logger.i("TCPTransport: disconnecting already in progress");
            return;
        }

        setCurrentState(TCPTransportState.DISCONNECTING);

        String disconnectMsg = (message == null ? "" : message);
        if (exception != null) {
            disconnectMsg += ", " + exception.toString();
        }

        try {
            if (mThread != null) {
                mThread.halt();
                mThread.interrupt();
                //mThread.join();

                mThread = null;
            }

            if (mSocket != null) {
                mSocket.close();
            }
            mSocket = null;

            if (mServerSocket != null) {
                mServerSocket.close();
            }
            mServerSocket = null;
        } catch (IOException e) {
            Logger.i("TCPTransport.disconnect: IOException: " + e.getMessage());
        } /*catch (InterruptedException e) {
            Logger.i("TCPTransport.disconnect: Join Exception: " + e.getMessage());
        }*/

        if (exception == null) {
            // This disconnect was not caused by an error, notify the proxy that
            // the transport has been disconnected.
            Logger.i("Disconnect is correct. Handling it");
            handleTransportDisconnected(disconnectMsg);
        } else {
            // This disconnect was caused by an error, notify the proxy
            // that there was a transport error.
            Logger.e("Disconnect is incorrect. Handling it as error");
            final String finalDisconnectMsg = disconnectMsg;
            handleTransportError(finalDisconnectMsg, exception);
        }
    }

    /**
     * Overridden abstract method which returns specific type of this transport.
     *
     * @return Constant value - TransportType.TCP.
     * @see TransportType
     */
    public TransportType getTransportType() {
        return TransportType.TCP;
    }

    /**
     * Internal class that represents separate thread, that does actual work, related to connecting/reading/writing data
     */
    private class TCPTransportThread extends Thread {

        /**
         * Represents current thread state - halted or not. This flag is used to change internal behavior depending
         * on current state.
         */
        private volatile Boolean isHalted = false;

        /**
         * Method that marks thread as halted.
         */
        public void halt() {
            isHalted = true;
        }

        /**
         * Tries to connect to the applink core. Only one connect try will be performed.
         *
         * @return true if connection established and false otherwise
         */
        private boolean connect() {
            boolean bConnected = false;
            if (!isHalted) {
                try {

                    if ((null != mSocket) && (!mSocket.isClosed())) {
                        Logger.i("TCPTransport.connect: Socket is not closed. Trying to close it");
                        mSocket.close();
                    }

                    if ((null != mServerSocket) && (!mServerSocket.isClosed())) {
                        Logger.i("TCPTransport.connect: ServerSocket is not closed. Trying to close it");
                        mServerSocket.close();
                    }

                    Logger.i(String.format("TCPTransport.connect: Socket is closed. Trying to connect to %s", mConfig));

                    if (mConfig.getIsNSD()) {
                        if (mServerSocket == null) {
                            mServerSocket = new ServerSocket(mConfig.getPort());

                            handleOnServerSocketInit(mServerSocket.getLocalPort());
                        }
                        Logger.i("Waiting for accept ...");
                        mSocket = mServerSocket.accept();
                        Logger.i("Socket accepted");
                    } else {
                        mSocket = new Socket();
                        mSocket.connect(new InetSocketAddress(mConfig.getIPAddress(), mConfig.getPort()));
                    }

                    mOutputStream = mSocket.getOutputStream();
                    mInputStream = new BufferedInputStream(mSocket.getInputStream());

                } catch (IOException e) {
                    Logger.i("TCPTransport.connect: Exception during connect stage: " + e.getMessage());
                }

                bConnected = (null != mSocket) && mSocket.isConnected();

                if (bConnected) {
                    Logger.i("TCPTransport.connect: Socket connected");
                } else {
                    Logger.i("TCPTransport.connect: Socket not connected");
                }
            }

            return bConnected;
        }

        @Override
        protected void finalize() throws Throwable {
            super.finalize();
            Logger.i("TCPTransport finalize thread");
        }

        /**
         * Performs actual thread work
         */
        @Override
        public void run() {
            Logger.i("TCPTransport.run: transport thread created. Starting connect stage");

            while (!isHalted) {
                setCurrentState(TCPTransportState.CONNECTING);
                if (!connect()) {
                    if (isHalted) {
                        Logger.i("TCPTransport.run: Connection failed, but thread already halted");
                    } else {
                        disconnect("Failed to connect to Sync", new SyncException("Failed to connect to Sync"
                                , SyncExceptionCause.SYNC_CONNECTION_FAILED));
                    }
                    break;
                }

                setCurrentState(TCPTransportState.CONNECTED);
                handleTransportConnected();

                byte[] buffer = new byte[READ_BUFFER_SIZE];

                while (!isHalted) {
                    Logger.i("TCPTransport.run: Waiting for data...");
                    int bytesRead;
                    try {
                        bytesRead = mInputStream.read(buffer);
                    } catch (IOException e) {
                        internalHandleStreamReadError();
                        break;
                    }

                    synchronized (TCPTransport.this) {
                        if (mThread.isInterrupted()) {
                            Logger.i("TCPTransport.run: Got new data but thread is interrupted");
                            break;
                        }
                    }

                    Logger.i("TCPTransport.run: Got new data");
                    if (-1 == bytesRead) {
                        internalHandleTCPDisconnect();
                        break;
                    } else if (0 == bytesRead) {
                        Logger.i("TCPTransport.run: Received zero bytes");
                    } else {
                        Logger.i(String.format("TCPTransport.run: Received %d bytes", bytesRead));
                        handleReceivedBytes(buffer, bytesRead);
                    }
                }
            }

            Logger.i("TCPTransport.run: Thread terminated");
            setCurrentState(TCPTransportState.IDLE);
        }

        /**
         * Internal handling of Tcp disconnection
         */
        private void internalHandleTCPDisconnect() {
            if (isHalted) {
                Logger.i("TCPTransport.run: TCP disconnect received, but thread already halted");
            } else {
                Logger.i("TCPTransport.run: TCP disconnect received");
                disconnect("TCPTransport.run: End of stream reached", null);
            }
        }

        /**
         * Internal handling of reading data from input stream
         */
        private void internalHandleStreamReadError() {
            if (isHalted) {
                Logger.i("TCPTransport.run: Exception during reading data, but thread already halted");
            } else {
                Logger.i("TCPTransport.run: Exception during reading data");
                disconnect("Failed to read data from Sync", new SyncException("Failed to read data from Sync"
                        , SyncExceptionCause.SYNC_CONNECTION_FAILED));
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
     *
     * @param currentState New state
     */
    private synchronized void setCurrentState(TCPTransportState currentState) {
        Logger.i(String.format("Current state changed to: %s", currentState));
        this.mCurrentState = currentState;
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
