package com.ford.syncV4.messageDispatcher;

import java.util.Comparator;

import com.ford.syncV4.proxy.callbacks.InternalProxyMessage;

public class InternalProxyMessageComparitor implements Comparator<InternalProxyMessage> {

	@Override
	public int compare(InternalProxyMessage arg0, InternalProxyMessage arg1) {
		// Always return 0, turning the priority queue into a FIFO queue. 
		return 0;
	}
}
