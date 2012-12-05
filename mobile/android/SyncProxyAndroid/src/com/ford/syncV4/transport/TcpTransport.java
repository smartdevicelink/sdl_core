package com.ford.syncV4.transport;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.trace.SyncTrace;
import com.ford.syncV4.trace.enums.InterfaceActivityDirection;
import com.ford.syncV4.util.DebugTool;

public class TcpTransport extends SyncTransport {
	private static final String SYNC_LIB_TRACE_KEY = "42baba60-eb57-11df-98cf-0800200c9a66";
	private static final String SYNC_LIB_PRIVATE_TOKEN = "{DAE1A88C-6C16-4768-ACA5-6F1247EA01C2}";
	
	private Socket _connectSocket = null;
	private InputStream _input = null;
	private OutputStream _output = null;
    private Thread _transportReader = null;
    private static int m_tcpListenPort = 7390;
	private static ServerSocket m_tcpListenerSoc = null;

    public TcpTransport(int listenPort, ITransportListener transportListener)
    throws SyncException {
    	super(transportListener);
    	m_tcpListenPort = listenPort;
    } // end-ctor

    @Override
	public void openConnection() throws SyncException {
		try {
	        m_tcpListenerSoc = new ServerSocket(m_tcpListenPort);
        } catch (Exception ex) {
        	String msg = "Failure creating tcp listener for port " + m_tcpListenPort + ": " + ex.toString();
			DebugTool.logError(msg, ex);
			throw new SyncException(msg, SyncExceptionCause.SYNC_CONNECTION_FAILED);
        } // end-catch

		_transportReader = new Thread(new Runnable() {public void run(){acceptIncomingConnection();}});
		_transportReader.setDaemon(true);
		_transportReader.setName("TransportReader");
		_transportReader.start();
	}

    private void acceptIncomingConnection() {
    	// First, listen to accept incoming connection from SYNC
    	try {
			SyncTrace.logTransportEvent("TcpTransport: listening for incoming connect on port " + m_tcpListenPort, null, InterfaceActivityDirection.Receive, null, 0, SYNC_LIB_TRACE_KEY);
			_connectSocket = m_tcpListenerSoc.accept();
    		_connectSocket.setKeepAlive(true);
    		
			_output = _connectSocket.getOutputStream();
			_input = _connectSocket.getInputStream();

			String syncIPInfo = _connectSocket.getInetAddress().getHostAddress();
			SyncTrace.logTransportEvent("TcpTransport: TCP Connection Accepted from SYNC at: " + syncIPInfo, "", InterfaceActivityDirection.Receive, null, 0, SYNC_LIB_TRACE_KEY);
			
			handleTransportConnected();

    	} catch (Exception ex) {
    		DebugTool.logError("Failure accepting incoming TCP connection: " + ex.toString(), ex);
			disconnect("Failed to accept connection", ex);
    	} finally {
			if (m_tcpListenerSoc != null) {
				try {
					m_tcpListenerSoc.close();
				} catch (IOException e) {
					//do nothing
				}
				m_tcpListenerSoc = null;
			} // end-if
		} // end-finally
		
		// Now start reading transport
		readTransport();
    } // end-method

    private void readTransport() {
		byte[] buf = new byte[4096];
		try {
			while (true) {
				int bytesRead = -1;
				try {
					bytesRead = _input.read(buf);
				} catch (Exception e) {
					disconnect("Failed to read from TCP transport", e);
					break; // out of while loop
				} // end-catch

				if (bytesRead == 0) {
					throw new SyncException("Failed to read from TCP transport", SyncExceptionCause.SYNC_CONNECTION_FAILED);
				} // end-if
				
				handleReceivedBytes(buf, bytesRead);
				
			} // end-while

		} catch (Exception excp) {
			String errString = "Failure reading TcpTransport stream: " + excp.toString();
			DebugTool.logError(errString, excp);
			disconnect(errString, excp);
			return;
		} // end-catch
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
	private void disconnect(String msg, Exception ex) {
		DebugTool.logError("TcpTransport.disconnect: " + msg, ex);
		String disconnectMsg = (msg == null ? "" : msg);
		if (ex != null) {
			disconnectMsg += ", " + ex.toString();
		} // end-if

		//SyncTrace.logTransportEvent("BTTransport.disconnect: " + disconnectMsg, null, InterfaceActivityDirection.Transmit, null, 0, TraceKey.key);

		try {
			if (_transportReader != null) {
				_transportReader.interrupt();
				_transportReader = null;
			}
		} catch (Exception e) {
			DebugTool.logError("Failed to stop transport reader thread", e);
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

		try {
			if (_connectSocket != null) {
				//logDiagnostics("activeSocket closing...");
				_connectSocket.close();
				_connectSocket = null;
			}
		} catch (Exception e) {
			DebugTool.logError("Failed to close activeSocket", e);
		} // end-catch

		try {
			if (m_tcpListenerSoc != null) {
				//logDiagnostics("serverSocket closing...");
				m_tcpListenerSoc.close();
				m_tcpListenerSoc = null;
			}
		} catch (Exception e) {
			DebugTool.logError("Failed to close listener socket", e);
		} // end-catch
		
		// If generated from an error, call handleTransportError, otherwise, call handleTransportDisconnect
		if (ex == null) {
			handleTransportDisconnected(msg);
		} else {
			handleTransportError(msg, ex);
		}
	} // end-method

	public boolean sendBytesOverTransport(byte[] msgBytes, int offset, int length) {
		boolean sendResult = false;
		try {
			_output.write(msgBytes, offset, length);
			sendResult = true;
		} catch (Exception ex) {
			DebugTool.logError("Error writing to TCP socket: " + ex.toString(), ex);
			sendResult = false;
		} // end-catch
		return sendResult;
	} // end-method
} // end-class
