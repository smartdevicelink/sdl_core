package com.ford.syncV4.transport;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.trace.SyncTrace;
import com.ford.syncV4.trace.enums.InterfaceActivityDirection;
import com.ford.syncV4.util.DebugTool;

/**
 * Bluetooth Transport Implementation. This transport advertises its existence to SYNC by publishing an SDP record and waiting for an incoming connection from SYNC. Connection is verified by checking for the SYNC UUID. For more detailed information please refer to the <a href="#bluetoothTransport">Bluetooth Transport Guide</a>.
 *
 */
public class BTTransport extends SyncTransport {	
	//936DA01F9ABD4D9D80C702AF85C822A8
	private final static UUID SYNC_V4_MOBILE_APPLICATION_SVC_CLASS = new UUID(0x936DA01F9ABD4D9DL, 0x80C702AF85C822A8L);

	private static final String SYNC_LIB_TRACE_KEY = "42baba60-eb57-11df-98cf-0800200c9a66";
	
	private BluetoothAdapter _adapter = null;
	private BluetoothSocket _activeSocket = null;
	private InputStream _input = null;
	private UUID _listeningServiceUUID = SYNC_V4_MOBILE_APPLICATION_SVC_CLASS;
	private BluetoothAdapterMonitor _bluetoothAdapterMonitor = null;
	private TransportReaderThread _transportReader = null;
	private OutputStream _output = null;
	private BluetoothServerSocket _serverSocket = null;
	
	// Boolean to monitor if the transport is in a disconnecting state
    private boolean _disconnecting = false;
	
	public BTTransport(ITransportListener transportListener) {
		super(transportListener);
	} // end-ctor
	
	public void openConnection () throws SyncException {    	
		// Get the device's default Bluetooth Adapter
		_adapter = BluetoothAdapter.getDefaultAdapter();
		
		// Test if Adapter exists
		if (_adapter == null) {
			throw new SyncException("No Bluetooth adapter found. Bluetooth adapter must exist to communicate with SYNC.", SyncExceptionCause.BLUETOOTH_ADAPTER_NULL);
		}
		
		// Test if Bluetooth is enabled
		try {
			if (!_adapter.isEnabled()) {
				throw new SyncException("Bluetooth adapter must be enabled to instantiate a SyncProxy object.", SyncExceptionCause.BLUETOOTH_DISABLED);
			}
		} catch (SecurityException e) {
			throw new SyncException("Insufficient permissions to interact with the Bluetooth Adapter.", SyncExceptionCause.PERMISSION_DENIED);
		}
		
		// Start BluetoothAdapterMonitor to ensure the Bluetooth Adapter continues to be enabled
		_bluetoothAdapterMonitor = new BluetoothAdapterMonitor(_adapter);
		
		try {
			_serverSocket = _adapter.listenUsingRfcommWithServiceRecord("SyncProxy", _listeningServiceUUID);
		} catch (IOException ex) {
			
			// Test to determine if the bluetooth has been disabled since last check			
			if (!_adapter.isEnabled()) {
				throw new SyncException("Bluetooth adapter must be on to instantiate a SyncProxy object.", SyncExceptionCause.BLUETOOTH_DISABLED);
			}
			
			throw new SyncException("Could not open connection to SYNC.", ex, SyncExceptionCause.SYNC_CONNECTION_FAILED);
		} 
		
		// Test to ensure serverSocket is not null
		if (_serverSocket == null) {
			throw new SyncException("Could not open connection to SYNC.", SyncExceptionCause.SYNC_CONNECTION_FAILED);
		}
		
		SyncTrace.logTransportEvent("BTTransport: listening for incoming connect to service ID " + _listeningServiceUUID, null, InterfaceActivityDirection.Receive, null, 0, SYNC_LIB_TRACE_KEY);
		
		// Setup transportReader thread
		_transportReader = new TransportReaderThread();
		_transportReader.setName("TransportReader");
		_transportReader.setDaemon(true);
		_transportReader.start();
		
		// Initialize the SiphonServer
		SiphonServer.init();
	} // end-method

	public void disconnect() {
		disconnect(null, null);
	}

	/**
	 * Destroys the transport between SYNC and the mobile app
	 * 
	 * @param msg
	 * @param ex
	 */
	private synchronized void disconnect(String msg, Exception ex) {		
		// If already disconnecting, return
		if (_disconnecting) {
			// No need to recursively call
			return;
		}		
		_disconnecting = true;
		
		String disconnectMsg = (msg == null ? "" : msg);
		if (ex != null) {
			disconnectMsg += ", " + ex.toString();
		} // end-if

		SyncTrace.logTransportEvent("BTTransport.disconnect: " + disconnectMsg, null, InterfaceActivityDirection.Transmit, null, 0, SYNC_LIB_TRACE_KEY);

		try {			
			if (_transportReader != null) {
				_transportReader.halt();
				_transportReader = null;
			}
		} catch (Exception e) {
			DebugTool.logError("Failed to stop transport reader thread.", e);
		} // end-catch	
		
		try {
			if (_bluetoothAdapterMonitor != null) {
				_bluetoothAdapterMonitor.halt();
				_bluetoothAdapterMonitor = null;
			}
		} catch (Exception e) {
			DebugTool.logError("Failed to stop adapter monitor thread.", e);
		}
		
		try {
			if (_serverSocket != null) {
				_serverSocket.close();
				_serverSocket = null;
			} 
		} catch (Exception e) {
			DebugTool.logError("Failed to close serverSocket", e);
		} // end-catch
		
		try {
			if (_activeSocket != null) {
				_activeSocket.close();
				_activeSocket = null;
			}
		} catch (Exception e) {
			DebugTool.logError("Failed to close activeSocket", e);
		} // end-catch
		
		try {
			if (_input != null) {
				_input.close();
				_input = null;
			}
		} catch (Exception e) {
			DebugTool.logError("Failed to close input stream", e);
		} // end-catch
		
		try {
			if (_output != null) {
				_output.close();
				_output = null;
			}
		} catch (Exception e) {
			DebugTool.logError("Failed to close output stream", e);
		} // end-catch
		
		if (ex == null) {
			// This disconnect was not caused by an error, notify the proxy that 
			// the trasport has been disconnected.
			handleTransportDisconnected(msg);
		} else {
			// This disconnect was caused by an error, notify the proxy
			// that there was a transport error.
			handleTransportError(msg, ex);
		}
	} // end-method

	
	/**
	 * Sends data over the transport.  Takes a byte array and transmits data provided starting at the
	 * offset and of the provided length to fragment transmission.
	 */
	public boolean sendBytesOverTransport(byte[] msgBytes, int offset, int length) {
		boolean sendResult = false;
		try {
			_output.write(msgBytes, offset, length);
			sendResult = true;
		} catch (Exception ex) {
			DebugTool.logError("Error writing to Bluetooth socket: " + ex.toString(), ex);
			handleTransportError("Error writing to Bluetooth socket:", ex);
			sendResult = false;
		} // end-catch
		return sendResult;
	} // end-method
	
	
	
	private class TransportReaderThread extends Thread {
		
		byte[] buf = new byte[4096];
		private Boolean isHalted = false;
		
		public void halt() {
			isHalted = true;
		}
		
		private void acceptConnection() {
			SyncTrace.logTransportEvent("BTTransport: Waiting for incoming RFCOMM connect", "", InterfaceActivityDirection.Receive, null, 0, SYNC_LIB_TRACE_KEY);
			
			try {
				// Blocks thread until connection established.
				_activeSocket = _serverSocket.accept();
				
				// If halted after serverSocket.accept(), then return immediately
				if (isHalted) {
					return;
				}
				
				// Log info of the connected device
				BluetoothDevice btDevice = _activeSocket.getRemoteDevice();
				String btDeviceInfoXml = SyncTrace.getBTDeviceInfo(btDevice);
				SyncTrace.logTransportEvent("BTTransport: RFCOMM Connection Accepted", btDeviceInfoXml, InterfaceActivityDirection.Receive, null, 0, SYNC_LIB_TRACE_KEY);
				
				_output = _activeSocket.getOutputStream();
				_input = _activeSocket.getInputStream();

				handleTransportConnected();
				
			} catch (Exception e) {
				
				if (!isHalted) {					
					// Only call disconnect if the thread has not been halted
					
					// Check to see if Bluetooth was disabled
					if (_adapter != null && !_adapter.isEnabled()) {
						disconnect("Bluetooth Adapater has been disabled.", new SyncException("Bluetooth adapter must be enabled to instantiate a SyncProxy object.", e, SyncExceptionCause.BLUETOOTH_DISABLED));
					} else {
						disconnect("Failed to accept connection", e);
					}
				} 
			} finally {
				if (_serverSocket != null && !isHalted) {
					try {
						_serverSocket.close();
					} catch (IOException e) {
						//do nothing
					}
					_serverSocket = null;
				}
			}
		}
		
		private void readFromTransport() {
			try {
				int bytesRead = -1;
				try {
					bytesRead = _input.read(buf);
				} catch (Exception e) {
					if (!isHalted) {
						// Only call disconnect if the thread has not been halted
						
						// Check to see if Bluetooth was disabled
						if (_adapter != null && !_adapter.isEnabled()) {
							disconnect("Bluetooth Adapater has been disabled.", new SyncException("Bluetooth adapter must be enabled to instantiate a SyncProxy object.", e, SyncExceptionCause.BLUETOOTH_DISABLED));
						} else {
							disconnect("Failed to read from Bluetooth transport.", e);
						}
					}
					return;
				} // end-catch
				
				if (bytesRead != -1) {
					handleReceivedBytes(buf, bytesRead);
				} else {
					// When bytesRead == -1, it indicates end of stream
					if (!isHalted) {
						// Only call disconnect if the thread has not been halted
						DebugTool.logError("End of stream reached!");
						disconnect("End of stream reached.", null);
					}
				}
			} catch (Exception excp) {
				if (!isHalted) {
					// Only call disconnect if the thread has not been halted
					String errString = "Failure in BTTransport reader thread: " + excp.toString();
					DebugTool.logError(errString, excp);
					disconnect(errString, excp);
				}
				return;
			} // end-catch
		} // end-method
		
		public void run() {
			// acceptConnection blocks until the connection has been accepted
			acceptConnection();
			
			while (!isHalted) {
				readFromTransport();
			}
		}
	}
	
	private class BluetoothAdapterMonitor {
		private boolean _isHalted = false;
		private BluetoothAdapter _bluetoothAdapter = null;
		private final String THREAD_NAME = "BluetoothAdapterMonitor";
		private Thread _bluetoothAdapterMonitorThread = null;
		
		public BluetoothAdapterMonitor(BluetoothAdapter bluetoothAdapter) {
			if (bluetoothAdapter == null) {
				throw new IllegalArgumentException("BluetoothAdapter cannot be null.");
			}
			
			// Set the bluetooth adapter
			_bluetoothAdapter = bluetoothAdapter;
			
			_bluetoothAdapterMonitorThread = new Thread(new Runnable() {
				@Override
				public void run() {
					while (!_isHalted) {
						checkIfBluetoothAdapterDisabled();
						try {
							Thread.sleep(15000);
						} catch (InterruptedException e) {
							// Break if interrupted
							break;
						}
					}
				}
			});
			_bluetoothAdapterMonitorThread.setName(THREAD_NAME);
			_bluetoothAdapterMonitorThread.setDaemon(true);
			_bluetoothAdapterMonitorThread.start();
		}
		
		private void checkIfBluetoothAdapterDisabled() {
			if (_bluetoothAdapter != null && !_bluetoothAdapter.isEnabled()) {
				// Bluetooth adapter has been disabled, disconnect the transport
				disconnect("Bluetooth adapter has been disabled.", 
						   new SyncException("Bluetooth adapter must be enabled to instantiate a SyncProxy object.", SyncExceptionCause.BLUETOOTH_DISABLED));
			}
		}
		
		public void halt() {
			_isHalted = true;
			_bluetoothAdapterMonitorThread.interrupt();
		}
	}
} // end-class
