package com.ford.syncV4.messageDispatcher;

import java.util.Comparator;
import java.util.concurrent.PriorityBlockingQueue;

import com.ford.syncV4.util.DebugTool;

public class ProxyMessageDispatcher<messageType> {
	PriorityBlockingQueue<messageType> _queue = null;
	private Thread _messageDispatchingThread = null;
	IDispatchingStrategy<messageType> _strategy = null;

	// Boolean to track if disposed
	private Boolean dispatcherDisposed = false;
	
	public ProxyMessageDispatcher(String THREAD_NAME, Comparator<messageType> messageComparator, 
			IDispatchingStrategy<messageType> strategy) {
		_queue = new PriorityBlockingQueue<messageType>(10, messageComparator);
		
		_strategy = strategy;
		
		// Create dispatching thread
		_messageDispatchingThread = new Thread(new Runnable() {public void run(){handleMessages();}});
		_messageDispatchingThread.setName(THREAD_NAME);
		_messageDispatchingThread.setDaemon(true);
		_messageDispatchingThread.start();
	}
	
	public void dispose() {
		dispatcherDisposed = true;
		
		if(_messageDispatchingThread != null) {
			_messageDispatchingThread.interrupt();
			_messageDispatchingThread = null;
		}
	}
		
	private void handleMessages() {
		
		try {
			messageType thisMessage;
		
			while(dispatcherDisposed == false) {
				thisMessage = _queue.take();
				_strategy.dispatch(thisMessage);
			}
		} catch (InterruptedException e) {
			// Thread was interrupted by dispose() method, no action required
			return;
		} catch (Exception e) {
			DebugTool.logError("Error occurred dispating message.", e);
			_strategy.handleDispatchingError("Error occurred dispating message.", e);
		}
	}
		
	public void queueMessage(messageType message) {
		try {
			_queue.put(message);
		} catch(ClassCastException e) { 
			_strategy.handleQueueingError("ClassCastException encountered when queueing message.", e);
		} catch(Exception e) {
			_strategy.handleQueueingError("Exception encountered when queueing message.", e);
		}
	}
}
