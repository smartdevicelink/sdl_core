package com.ford.syncV4.android.module.reader;

/**
 * Interface for different readers of binary data. Used in XML test files. The
 * whole data is read and returned in one pass. Thus, the readers should not
 * store an internal state.
 * 
 * @author enikolsky
 * 
 */
public interface BinaryDataReader {
	/**
	 * Checks if this class supports the data format of the input string.
	 * 
	 * @param input
	 *            data source
	 * @return true if the class supports reading
	 */
	public boolean supportsReading(String input);

	/**
	 * Interprets the input string and returns an array of bytes representing
	 * the data.
	 * 
	 * @param input
	 *            data source
	 * @return interpreted data or null if failed to read
	 */
	public byte[] read(String input);
}
