package com.ford.syncV4.messageDispatcher;

public interface IDispatchingStrategy<messageType> {
	public void dispatch(messageType message);
	
	public void handleDispatchingError(String info, Exception ex);
	
	public void handleQueueingError(String info, Exception ex);
}
