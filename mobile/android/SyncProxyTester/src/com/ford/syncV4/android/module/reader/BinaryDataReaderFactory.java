package com.ford.syncV4.android.module.reader;

import java.util.ArrayList;
import java.util.List;

/**
 * Factory class that looks up and returns a class implementing the
 * {@link BinaryDataReader} interface to parse the input string.
 * 
 * Use the {@link #getReaderForString(String)} method to get a suitable reader.
 * 
 * @author enikolsky
 * @see BinaryDataReader
 * 
 */
public class BinaryDataReaderFactory {
	private List<BinaryDataReader> readers = new ArrayList<BinaryDataReader>(3);

	/**
	 * Default constructor. Initializes the class.
	 */
	public BinaryDataReaderFactory() {
		readers.add(new Base64BinaryDataReader());
		readers.add(new FileBinaryDataReader());
	}

	/**
	 * Finds and returns a reader suitable for reading data specified by the
	 * input string.
	 * 
	 * @param input
	 *            data source
	 * @return a class that can read the data or null if not found
	 */
	public BinaryDataReader getReaderForString(String input) {
		for (BinaryDataReader reader : readers) {
			if (reader.supportsReading(input)) {
				return reader;
			}
		}

		return null;
	}
}
