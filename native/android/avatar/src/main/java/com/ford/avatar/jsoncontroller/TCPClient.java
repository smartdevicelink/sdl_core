package com.ford.avatar.jsoncontroller;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class TCPClient implements ITcpClient {

	public TCPClient(String adress, int port) {
		mAdress = adress;
		mPort = port;
		// connect to server
		connect();
	}
	
	@Override
	public void connect() {
		try {
			// create connection
			mSocket = new Socket(mAdress, mPort);
			mSocket.setTcpNoDelay(true);
			mSocket.setSoTimeout(0);
			mSocketReader = new BufferedReader(new InputStreamReader(
					mSocket.getInputStream()));
			mSocketWriter = new BufferedWriter(new OutputStreamWriter(
					mSocket.getOutputStream()));
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void sendMsg(String msg) {
		try {
			mSocketWriter.write(msg);
			mSocketWriter.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	@Override
	public String receiveMsg() {
		String response = null;
		try {
			if (mSocketReader.ready())
				response = mSocketReader.readLine();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return response;
	}

	@Override
	public void disconnect() {
		try {
			mSocketReader.close();
			mSocketWriter.close();
			mSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	// Getters

	public int getPort() {
		return mPort;
	}

	public String getAdress() {
		return mAdress;
	}

	public Socket getSocket() {
		return mSocket;
	}

	// ===============================================
	// private section
	// ===============================================
	private int mPort = -1;
	private String mAdress = null;
	private Socket mSocket = null;
	private BufferedReader mSocketReader = null;
	private BufferedWriter mSocketWriter = null;

}
