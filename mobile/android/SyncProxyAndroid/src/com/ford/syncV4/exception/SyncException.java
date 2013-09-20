package com.ford.syncV4.exception;


public class SyncException extends Exception {
	
	private static final long serialVersionUID = 5922492291870772815L;
	
	protected Throwable detail = null;
	private SyncExceptionCause _syncExceptionCause = null;
	
	public SyncException(String msg, SyncExceptionCause exceptionCause) {
		super(msg);
		_syncExceptionCause = exceptionCause;
	}
	
	public SyncException(String msg, Throwable ex, SyncExceptionCause exceptionCause) {
		super(msg + " --- Check inner exception for diagnostic details");
		detail = ex;
		_syncExceptionCause = exceptionCause;
	}
	
	public SyncException(Throwable ex) {
		super(ex.getMessage());
		detail = ex;
	}
	
	public SyncExceptionCause getSyncExceptionCause() {
		return _syncExceptionCause;
	}
	
	public Throwable getInnerException() {
		return detail;
	}
	
	public String toString() {
		String ret = this.getClass().getName();
		ret += ": " + this.getMessage();
		if(this.getSyncExceptionCause() != null){
			ret += "\nSyncExceptionCause: " + this.getSyncExceptionCause().name();
		}
		if (detail != null) {
			ret += "\nnested: " + detail.toString();
			detail.printStackTrace();
		}
		return ret;
	}
}
