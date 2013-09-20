package com.ford.syncV4.util;

import java.util.Vector;

public abstract class StringEnumer {	 
	
	protected StringEnumer(int value, String name) {
		this.value = value;
		this.name = name;
	}
	
	private int value;
	private String name;
	
	public int getValue() { return value; }
	public String getName() { return name; }
	
	public boolean equals(StringEnumer other) {
		return name == other.getName();
	}
	
	public boolean eq(StringEnumer other) {
		return equals(other);
	}
	
	public String toString() {
		return name;
	}
	
	public static StringEnumer get(Vector<? extends StringEnumer> theList, int value) {
		for (StringEnumer current : theList) {
			if (current.getValue() == value) {
				return current;
			}
		}
		return null;
	}
	
	public static StringEnumer get(Vector<? extends StringEnumer> theList, String name) {
		for (StringEnumer current : theList) {
			if (current.getName().equalsIgnoreCase(name)) {
				return current;
			}
		}
		return null;
	}
}
