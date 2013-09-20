package com.ford.syncV4.util;

import java.util.Enumeration;
import java.util.Vector;

public abstract class ByteEnumer {	
	
	protected ByteEnumer(byte value, String name) {
		this.value = value;
		this.name = name;
	}
	
	private byte value;
	private String name;
	
	public byte getValue() { return value; }
	public String getName() { return name; }
	
	public boolean equals(ByteEnumer other) {
		return name == other.getName();
	}
	
	public boolean eq(ByteEnumer other) {
		return equals(other);
	}
		
	public byte value() {
		return value;
	}
	
	public static ByteEnumer get(Vector theList, byte value) {
		Enumeration enumer = theList.elements();
		while (enumer.hasMoreElements()) {
			ByteEnumer current = (ByteEnumer)enumer.nextElement();
			if (current.getValue() == value) {
				return current;
			}
		}
		return null;
	}
	
	public static ByteEnumer get(Vector theList, String name) {
		Enumeration enumer = theList.elements();
		while (enumer.hasMoreElements()) {
			ByteEnumer current = (ByteEnumer)enumer.nextElement();
			if (current.getName().equals(name)) {
				return current;
			}
		}
		return null;
	}
}
