package com.ford.syncV4.transport;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.util.logger.Logger;

import java.io.InputStream;
import java.io.OutputStream;

public abstract class SyncTransport {

    private static final String SYNC_LIB_TRACE_KEY = "42baba60-eb57-11df-98cf-0800200c9a66";
    private final static String CLASS_NAME = SyncTransport.class.getSimpleName();
    private static final String SEND_LOCK_OBJ = "lock";

    private final static String FailurePropagating_Msg = "Failure propagating ";
    private Boolean isConnected = false;

    private String _sendLockObj = "lock";

    public abstract InputStream getInputStream();

    public abstract OutputStream getOutputStream();

    // Get status of transport connection
    public Boolean getIsConnected() {
        return isConnected;
    }

    //protected SyncTransport(String endpointName, String param2, ITransportListener transportListener)
    protected SyncTransport(ITransportListener transportListener) {
<<<<<<< HEAD
        if (transportListener == null) {
            throw new IllegalArgumentException("Provided transport listener interface reference is null");
        } // end-if
        _transportListener = transportListener;
    } // end-method

=======
    	if (transportListener == null) {
    		throw new IllegalArgumentException("Provided transport listener interface reference is null");
    	} // end-if
    	mTransportListener = transportListener;
    } // end-method

    public void removeListener() {
        mTransportListener = null;
    }
    
>>>>>>> develop
    // This method is called by the subclass to indicate that data has arrived from
    // the transport.
    protected void handleReceivedBytes(byte[] receivedBytes, int receivedBytesLength) {
        try {
            // Trace received data
            if (receivedBytesLength > 0) {
                // Send transport data to the siphon server
                //SiphonServer.sendBytesFromSYNC(receivedBytes, 0, receivedBytesLength);
                Logger.d(CLASS_NAME + " Receive Bytes");
<<<<<<< HEAD

                _transportListener.onTransportBytesReceived(receivedBytes, receivedBytesLength);
            } // end-if
        } catch (Exception excp) {
            Logger.e(FailurePropagating_Msg + "handleBytesFromTransport: " + excp.toString(), excp);
            handleTransportError(FailurePropagating_Msg, excp);
        } // end-catch
=======
				
				mTransportListener.onTransportBytesReceived(receivedBytes, receivedBytesLength);
			} // end-if
		} catch (Exception excp) {
			Logger.e(FailurePropagating_Msg + "handleBytesFromTransport: " + excp.toString(), excp);
			handleTransportError(FailurePropagating_Msg, excp);
		} // end-catch
>>>>>>> develop
    } // end-method

    // This method must be implemented by transport subclass, and is called by this
    // base class to actually send an array of bytes out over the transport.  This
    // method is meant to only be callable within the class hierarchy.
    protected abstract boolean sendBytesOverTransport(byte[] msgBytes, int offset, int length);

    // This method is called by whomever has reference to transport to have bytes
    // sent out over transport.
    public boolean sendBytes(byte[] message) {
        return sendBytes(message, 0, message.length);
    } // end-method

    // This method is called by whomever has reference to transport to have bytes
    // sent out over transport.
    public boolean sendBytes(byte[] message, int offset, int length) {
        boolean bytesWereSent = false;
        synchronized (SEND_LOCK_OBJ) {
            bytesWereSent = sendBytesOverTransport(message, offset, length);
        } // end-lock
        // Send transport data to the siphon server
        //SiphonServer.sendBytesFromAPP(message, offset, length);
        Logger.d(CLASS_NAME + " Send Bytes");
        return bytesWereSent;
    } // end-method

    private ITransportListener mTransportListener = null;

    // This method is called by the subclass to indicate that transport connection
    // has been established.
    protected void handleTransportConnected() {
        isConnected = true;
        try {
            Logger.d(CLASS_NAME + " Connected");
<<<<<<< HEAD
            _transportListener.onTransportConnected();
        } catch (Exception excp) {
            Logger.e(FailurePropagating_Msg + "onTransportConnected: " + excp.toString(), excp);
            handleTransportError(FailurePropagating_Msg + "onTransportConnected", excp);
        } // end-catch
    } // end-method

=======
			mTransportListener.onTransportConnected();
		} catch (Exception excp) {
			Logger.e(FailurePropagating_Msg + "onTransportConnected: " + excp.toString(), excp);
			handleTransportError(FailurePropagating_Msg + "onTransportConnected", excp);
		} // end-catch
	} // end-method
	
>>>>>>> develop
    // This method is called by the subclass to indicate that transport disconnection
    // has occurred.
    protected void handleTransportDisconnected(final String info) {
        isConnected = false;

        try {
            Logger.d(CLASS_NAME + " Disconnected");
<<<<<<< HEAD
            _transportListener.onTransportDisconnected(info);
        } catch (Exception excp) {
            Logger.e(FailurePropagating_Msg + "onTransportDisconnected: " + excp.toString(), excp);
        } // end-catch
    } // end-method


=======
			mTransportListener.onTransportDisconnected(info);
		} catch (Exception excp) {
			Logger.e(FailurePropagating_Msg + "onTransportDisconnected: " + excp.toString(), excp);
		}
	}
	
>>>>>>> develop
	// This method is called by the subclass to indicate a transport error has occurred.
	protected void handleTransportError(final String message, final Exception ex) {
		isConnected = false;
		mTransportListener.onTransportError(message, ex);
	}

    protected void handleOnServerSocketInit(int serverSocketPort) {
        mTransportListener.onServerSocketInit(serverSocketPort);
    }

    public abstract void openConnection() throws SyncException;

    public abstract void disconnect();

    // This method is called when the SDK doesn't want to read anything from the
    // transport anymore. It is required for the USB transport.
    public abstract void stopReading();

    /**
     * Abstract method which should be implemented by subclasses in order to return actual type of the transport.
     *
     * @return One of {@link TransportType} enumeration values.
     * @see TransportType
     */
    public abstract TransportType getTransportType();
} // end-class
