package com.ford.syncV4.transport;

import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.BindException;
import java.net.ServerSocket;
import java.net.Socket;

import com.ford.syncV4.util.*;

public class SiphonServer {
	// Prohibit use of no-arg ctor
	private SiphonServer() {}
	
	enum SiphonDataType {
		fromSync,				
		fromApp,				
		appLog,
		formattedTrace,
		baselineTimeStamp,		
		traceSettings			
	}
	
	// Boolean to enable/disable the siphon
	private static Boolean m_siphonEnabled = false;
	
	// Boolean to determine if the siphon has been initialized
	private static Boolean m_siphonInitialized = false;
	
	private static Socket m_siphonSocket = null;
	private static Object m_siphonLock = new Object();
	private static ServerSocket m_listeningSocket = null;
	private static OutputStream m_siphonSocketOutputStream = null;
	private static SiphonServerThread m_siphonClientThread = null;
	
	// Initial timestamp in MS
	private static long m_startTimeStamp = 0;
	
	// SYNC Trace Message Version
	private static byte m_syncTraceMsgVersionNumber = 1;
	
	// Max number of ports to attempt a connection on
	private final static Integer MAX_NUMBER_OF_PORT_ATTEMPTS = 1000;
	
	// Starting port for future port attempts
	private final static short FIRST_PORT_TO_ATTEMPT_CONNECTION = 7474;

	// Boolean to determine if formatted trace is being sent
	private static Boolean m_sendingFormattedTrace = false;
	
	public static void enableSiphonServer() {
		m_siphonEnabled = true;
	}
	
	public static void disableSiphonServer() {
		m_siphonEnabled = false;
	}

	public static boolean init() {		
		// Only initialize if the siphon has not been initialized previously

		// Check here to be lean. If true, no need to synchronize
		if (m_siphonInitialized) {
			return true;
		}
		
		synchronized (m_siphonLock) {
			// To prevent a race condition, re-check m_siphonInitialized inside of synchronize block
			if (!m_siphonInitialized) {
				if (m_siphonClientThread == null) {
					// Set current time stamp 
					m_startTimeStamp = System.currentTimeMillis();
					
					// Start Siphon Thread
		            m_siphonClientThread = new SiphonServerThread();
			        m_siphonClientThread.setName("Siphon");
			        m_siphonClientThread.setDaemon(true);
			        m_siphonClientThread.start();
			        
			        m_siphonInitialized = true;
				} // end-if
			} // end-lock
		}
		
		return m_siphonInitialized;
	} // end-method

	public static void closeServer() throws IOException {
		
		if (m_siphonClientThread != null) {
			m_siphonClientThread.halt();
			m_siphonClientThread = null;
		}
		
		if (m_listeningSocket != null) {
			m_listeningSocket.close();
			m_listeningSocket = null;
		}
		
		if (m_siphonSocket != null) {
			m_siphonSocket.close();
			m_siphonSocket = null;
		}
		
		if (m_siphonSocketOutputStream != null) {
			m_siphonSocketOutputStream.close();
			m_siphonSocketOutputStream = null;
		}
	}
    
	public static Boolean sendBytesFromAPP(byte[] msgBytes, int offset, int length) {
		
		if (m_sendingFormattedTrace) {
			return false;
		}
		
		return sendSiphonData(SiphonDataType.fromApp, msgBytes, offset, length);
	} // end-method

	public static Boolean sendBytesFromSYNC(byte[] msgBytes, int offset, int length) {		

		if (m_sendingFormattedTrace) {
			return false;
		}
		
		return sendSiphonData(SiphonDataType.fromSync, msgBytes, offset, length);
	} // end-method
	
	public static Boolean sendSiphonLogData(String message) {
		
		if (m_sendingFormattedTrace) {
			return false;
		}
		
		if (message == null || message.length() == 0) {
			return false;
		}
				
		byte messageBytes[] = null;
		int messageLength = 0;
		
		try {
			messageBytes = message.getBytes("UTF-8");
		} catch (UnsupportedEncodingException e) {
			return false;
		}
		
		messageLength = messageBytes.length;
		return sendSiphonData(SiphonDataType.appLog, messageBytes, 0, messageLength);
		
	}
	
	public static Boolean sendFormattedTraceMessage(String message) {
				
		if (message == null || message.length() == 0) {
			return false;
		}
		
		byte messageBytes[] = null;
		int messageLength = 0;
				
		try {
			messageBytes = message.getBytes("UTF-8");
		} catch (UnsupportedEncodingException e) {
			return false;
		}
		
		messageLength = messageBytes.length;
		if (sendSiphonData(SiphonDataType.formattedTrace, messageBytes, 0, messageLength)) {
			m_sendingFormattedTrace = true;
			return true;
		} else {
			return false;
		}
	}
	
	private static Boolean sendSiphonData(SiphonDataType direction, byte[] msgBytes, int offset, int length) {
		byte siphonDataTypeIndicator = 0x00;
		
		long currentDateTime = System.currentTimeMillis();
		Integer deltaTimeMills = null;
			
		deltaTimeMills = (int)(currentDateTime - m_startTimeStamp);
		
		switch(direction) {
			case fromSync:
				siphonDataTypeIndicator = 0x00;
				break;
			case fromApp:
				siphonDataTypeIndicator = 0x01;
				break;
			case appLog:
				siphonDataTypeIndicator = 0x02;
				break;
			case formattedTrace:
				siphonDataTypeIndicator = 0x03;
				break;
			case baselineTimeStamp:
				siphonDataTypeIndicator = 0x04;
				break;
			case traceSettings:
				siphonDataTypeIndicator = 0x05;
				break;
			default:
				siphonDataTypeIndicator = 0x00;
				break;
		}
		
		// Set high bit to indicate new format
		siphonDataTypeIndicator = (byte)((byte)0x80 | siphonDataTypeIndicator);
		
		return sendDataToSiphonSocket(siphonDataTypeIndicator, deltaTimeMills, msgBytes, offset, length);
	}
	
	private synchronized static Boolean sendDataToSiphonSocket(byte directionIndicator, Integer timeStamp, 
			byte[] msgBytes, int offset, int length) {
		if (!m_siphonEnabled) {
			return false;
		}
		
		if (msgBytes == null || length == 0) {
			return false;
		}
		
		OutputStream siphonOutputStream = null;

		synchronized (m_siphonLock) {
			siphonOutputStream = m_siphonSocketOutputStream;
		} // end-lock

		if (siphonOutputStream == null) {
			return false;
		}		
		
		try	{		
			// blobSize = length(of message) + 1(size of direction indicator)
			//				+ 1 (size of msgVersionNumber) + 4 (size of timeStamp)
			int blobSize = length + 1 + 1 + 4;
			
			siphonOutputStream.write(BitConverter.intToByteArray(blobSize));
			siphonOutputStream.write(new byte[] {directionIndicator});
			siphonOutputStream.write(new byte[] {m_syncTraceMsgVersionNumber});
			siphonOutputStream.write(intToByteArray(timeStamp));
			siphonOutputStream.write(msgBytes, offset, length);
		} catch (Exception ex) {
			return false;
		} // end-catch
		
		return true;
	} // end-method
    
    private static class SiphonServerThread extends Thread {
    	
    	private Boolean isHalted = false;
    	short listenPort;
		
		public void halt() {
			isHalted = true;
		}
    	
    	private boolean findOpenSocket(short port) {
    		// Accept incoming sihpon connection from trace utility.
    		Boolean foundOpenPort = false;
    		listenPort = port;
    		
    		// Listen to accept incoming connection from SYNC
    		while (!foundOpenPort) {
	    		try {
	    			m_listeningSocket = new ServerSocket(listenPort);
	    			foundOpenPort = true;
	    		} catch (BindException ex) {
	    			listenPort++;
	    			if(listenPort > port + MAX_NUMBER_OF_PORT_ATTEMPTS) {
	    				return false;
	    			}
	    		} catch (IOException e) {
	    			return false;
				}
    		}
    		
    		return foundOpenPort;
    	}
    	
    	private void startServerOnPort() throws IOException {
    		Socket newSocket = null;
    		
    		// Wait for a connection
    		newSocket = m_listeningSocket.accept(); 
            
            // If isHalted after accept() delay, return
        	if (isHalted) {
        		return;
        	}

    		synchronized (m_siphonLock) {
    			// Reset siphonSocketOutputStream
    			if (m_siphonSocketOutputStream != null) {
    				try {
    					m_siphonSocketOutputStream.close();
    				} catch (IOException e) {
    					// Do nothing
    				}
    				m_siphonSocketOutputStream = null;
    			}

    			// Reset siphonSocket
    			if (m_siphonSocket != null) {
    				try {
    					m_siphonSocket.close();
    				} catch (IOException e) {
    					// Do nothing
    				}
    				m_siphonSocket = null;
    			}

    			// Store the new socket
    			m_siphonSocket = newSocket;
    			
    			// Set Socket Options
				m_siphonSocket.setKeepAlive(true);
				
				// Get the output stream of the connection
				m_siphonSocketOutputStream = m_siphonSocket.getOutputStream();
				
	        	// Output version number to the Siphon upon connection (version number prepending to logInfo)
				DebugTool.logInfo("Siphon connected.");
    		} // end-lock
        } // end-method
    	
    	@Override
    	public void run() {
    		try	{
    			if (findOpenSocket(FIRST_PORT_TO_ATTEMPT_CONNECTION)) {
    				while (!isHalted) {
	    				startServerOnPort();
    				}
    			} 
	    	} catch (Exception ex) {
	    		// Do nothing
	    	} finally {
				if (m_listeningSocket != null) {
					try {
						m_listeningSocket.close();
					} catch (IOException e) {
						// Do nothing
					}
					m_listeningSocket = null;
				}
			}
    	}
    }
    
    private static final byte[] intToByteArray(int value) {
        return new byte[] {
                (byte)(value >>> 24),
                (byte)(value >>> 16),
                (byte)(value >>> 8),
                (byte)value};
    }
} // end-class