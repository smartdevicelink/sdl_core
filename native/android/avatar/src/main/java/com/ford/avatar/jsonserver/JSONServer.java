package com.ford.avatar.jsonserver;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.InterruptedIOException;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Vector;

import com.ford.avatar.util.Const;
import com.ford.avatar.util.RPCConst;

import android.util.Log;

public class JSONServer extends Thread{

	
	public JSONServer() {
		this(RPCConst.TCP_SERVER_PORT);
	}
	
	public JSONServer(int port) {
		mPort = port;
		//instantiate MessageBrokerWrapper
		logMsg("Instantiate MessageBrokerWrapper");
		mMsgBroker = MBWrapper.CreateMessageBroker();
		logMsg("Start MessageBroker : " + mMsgBroker.toString());
		mMsgBroker.start(this);
		this.setPriority(Thread.MIN_PRIORITY);
	}
	
	@Override
	public void run(){
		super.run();
		//run thread to listen socket for new connections
		listenSocket();
		//read buffers of clients for messages
		waitMessages();
		//close server
		try {
			close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public boolean isReady(){
		return mIsReady;
	}
	
	// =====================================================
	// private section
	// =====================================================
	private final boolean DEBUG = true;
	private final String TAG_NAME = "JSONServer";
	private MBWrapper mMsgBroker = null;
	private int mPort = -1;
	private volatile boolean mIsReady = false;
	private Vector<Socket> mClientsSocketsPool = new Vector<Socket>();
	private Vector<BufferedReader> mClientsReadersPool = new Vector<BufferedReader>();
	private Vector<BufferedWriter> mClientsWritersPool = new Vector<BufferedWriter>();
	

	
	private void listenSocket(){
		logMsg("Start new thread for socket listener");
		//start server
		Thread socketListenerThread = new Thread(new Runnable() {
			
			public void run() {
				ServerSocket serverSocket = null;
				try {
					logMsg("Create server socket");
					serverSocket = new ServerSocket(mPort);
					// wait for connection
					mIsReady = true;
					while(true){
						Socket connectionSocket = serverSocket.accept();
						connectionSocket.setSoTimeout(0);
						connectionSocket.setTcpNoDelay(true);
						//put socket info in clients vectors
						mClientsSocketsPool.add(connectionSocket);
						mClientsWritersPool.add(new BufferedWriter(new OutputStreamWriter(
								connectionSocket.getOutputStream())));
						mClientsReadersPool.add(new BufferedReader(new InputStreamReader(
								connectionSocket.getInputStream())));
					}
				} catch (InterruptedIOException e) {
					e.printStackTrace();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}//run()
		});//new Thread
		socketListenerThread.setName("ServerListenerThread");
		socketListenerThread.start();
	}
	
	private void waitMessages(){
		logMsg("Wait for messages");
		//read msg and send it to MB
		while (true) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e1) {
				e1.printStackTrace();
			}
				//go through all clients readers and try to read msg
				if (mClientsReadersPool != null)
					for(int i=0; i<mClientsReadersPool.size();i++){
						// receive a message
						String incomingMsg = null;
						try{
							if (mClientsReadersPool.get(i).ready())
								incomingMsg = mClientsReadersPool.get(i).readLine();
							if (incomingMsg!=null) //redirect message to MB
								mMsgBroker.onMsgReceived(i+1, incomingMsg);
						}
						catch (IOException e) {
							incomingMsg = null;
						}	
					}//for			
		}// while

	}
	
	//method for sending messages from native code
	private int Send(int ctrlIndex, String data){
		logMsg("Send message to client: " + data);
		try {
			mClientsWritersPool.get(ctrlIndex-1).write(data);
			mClientsWritersPool.get(ctrlIndex-1).flush();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e1) {
			e1.printStackTrace();
		}
		
		return data.length();
		
	}
	
	public void close() throws IOException{
			for(int i=0; i<mClientsReadersPool.size();i++)
				mClientsReadersPool.get(i).close();
			mClientsReadersPool.clear();
			for(int i=0; i<mClientsWritersPool.size();i++)
				mClientsWritersPool.get(i).close();
			mClientsWritersPool.clear();
			for(int i=0; i<mClientsSocketsPool.size();i++)
				mClientsSocketsPool.get(i).close();
			mClientsSocketsPool.clear();
			mMsgBroker.destroy();
	}
	
	private void logMsg(String msg){
		if (DEBUG && Const.DEBUG){
			Log.i(TAG_NAME, msg);
		}
	}
	
}
