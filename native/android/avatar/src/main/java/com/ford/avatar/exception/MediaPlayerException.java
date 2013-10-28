package com.ford.avatar.exception;

public class MediaPlayerException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public MediaPlayerException() {
		super();
	}

	public MediaPlayerException(String detailMessage, Throwable throwable) {
		super(detailMessage, throwable);
	}

	public MediaPlayerException(String detailMessage) {
		super(detailMessage);
	}

	public MediaPlayerException(Throwable throwable) {
		super(throwable);
	}

}
