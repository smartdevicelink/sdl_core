package com.ford.avarsdl.jsoncontroller;

public interface ITcpClient {
	public void sendMsg(String msg);
	public String receiveMsg();
	public void connect();
	public void disconnect();

}
