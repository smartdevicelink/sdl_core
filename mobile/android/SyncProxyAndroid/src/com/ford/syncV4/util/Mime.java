package com.ford.syncV4.util;

public class Mime {
	
	private static String m_base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
	public static Boolean isSafeASCII(String check) {
		if (check.contains("&") || check.contains("<") || check.contains("]]") || check.contains("}") )
			return false;  // these things aren't reliably CDATA encodable
		
		return true;
	}
	
	public static String base64Encode(String toEncode) {
		
		byte[] bytesToEncode = new byte[toEncode.length()];   
		for (int i=0; i< toEncode.length(); i++) 
			bytesToEncode[i] = (byte)toEncode.charAt(i);      
		
		StringBuilder sb = new StringBuilder();

		int		idxin = 0;
		int		b64idx = 0;

		for (idxin=0;idxin < bytesToEncode.length;idxin++) {
			switch (idxin % 3) {
				case 0:
					b64idx = (bytesToEncode[idxin] >> 2) & 0x3f;
					break;
				case 1:
					b64idx = (bytesToEncode[idxin] >> 4) & 0x0f;
					b64idx |= ((bytesToEncode[idxin-1] << 4)& 0x30);
					break;
				case 2:
					b64idx = (bytesToEncode[idxin] >> 6) & 0x03;
					b64idx |= ((bytesToEncode[idxin-1] << 2)& 0x3c);
					sb.append(m_base64Chars.charAt(b64idx));
					b64idx = bytesToEncode[idxin] & 0x3f;
					break;
			} // end-switch
			sb.append(m_base64Chars.charAt(b64idx));
		} // end-for

		switch (idxin % 3) {
			case 0:
				break;
			case 1:
				b64idx = (bytesToEncode[idxin-1] << 4) & 0x30;
				sb.append(m_base64Chars.charAt(b64idx));
				sb.append("==");
				break;
			case 2:
				b64idx = ((bytesToEncode[idxin-1] << 2)& 0x3c);
				sb.append(m_base64Chars.charAt(b64idx));
				sb.append('=');
				break;
		} // end-switch
		
		return sb.toString();
	} // end-method
	
	
	private byte[] base64Decode(String base64String) {
		byte[] outBytes = null;
		byte[] base64ASCIIString = null;
		final String ASCII_Encoding = "US-ASCII";
		
		// Convert b64 string to raw bytes
		try {
			base64ASCIIString = base64String.getBytes(ASCII_Encoding);
		} catch (Exception ex){
			// TODO
			//publishStatus("Failure converting b64string to ASCII string: " + ex.toString());
			return null;
		}
		
		if (!m_decodeInitComplete) {
			m_decodeInitComplete = true;
			initForDecode();
		} // end-if

		int	numInChars = base64ASCIIString.length;

		if ((numInChars % 4) > 0) {
			return null;
		} // end-if

		int numOutBytes = base64ASCIIString.length / 4;
		numOutBytes *= 3;
		int	eqpos = base64String.indexOf("=");
		if (eqpos >= 0) {
			numOutBytes--;
			if (base64String.substring(eqpos).indexOf("==") >= 0) {
				numOutBytes--;
			} // end-if
		} // end-if
		outBytes = new byte[numOutBytes];

		byte	b64idxbits = 0x00;
		int		iidx = 0, oidx = 0;
		byte	writeByte = 0x00;
		byte	b64ASCIIChar = 0x00;
		for (iidx=0, oidx=0;iidx < numInChars;iidx++) {
			b64ASCIIChar = base64ASCIIString[iidx];
			if (b64ASCIIChar == 0x3d /*'='*/) {
				return outBytes;
			} // end-if

			if (!isb64char(b64ASCIIChar)) {
				return null;
			} // end-if

			switch (iidx % 4) {
				case 0:
					break;
				case 1:
					b64idxbits = b64decode[base64ASCIIString[iidx-1]];
					writeByte = (byte)((b64idxbits << 2) | ((b64decode[b64ASCIIChar] >> 4) & 0x03));
					outBytes[oidx++] = writeByte;
					break;
				case 2:
					b64idxbits = b64decode[base64ASCIIString[iidx-1]];
					writeByte = (byte)(((b64idxbits << 4) & 0xf0) | ((b64decode[b64ASCIIChar] >> 2) & 0x0f));
					outBytes[oidx++] = writeByte;
					break;
				case 3:
					b64idxbits = b64decode[base64ASCIIString[iidx-1]];
					writeByte = (byte)(((b64idxbits << 6) & 0xc0) | ((b64decode[b64ASCIIChar]) & 0x3f));
					outBytes[oidx++] = writeByte;
					break;
			} // end-switch
		} // end-for

		return outBytes;

	} // end-method


	private static	byte[]	b64decode = new byte[256];
	// A-Z is 0x41-0x5a
	// a-z is 0x61-0x7a
	// 0-9 is 0x30-0x39
	// + is 0x2b
	// / is 0x2f

	private static	boolean	m_decodeInitComplete = false;

	private void initForDecode() {
		int aidx = 0;
		int lidx = 0;
		// Set A-Z
		for (aidx=0x41, lidx=0;aidx <= 0x5a;aidx++, lidx++) {
			b64decode[aidx] = (byte)lidx;
		} // end-for
		// Set a-z
		for (aidx=0x61;aidx <= 0x7a;aidx++, lidx++) {
			b64decode[aidx] = (byte)lidx;
		} // end-for
		// Set 0-9
		for (aidx=0x30;aidx <= 0x39;aidx++, lidx++) {
			b64decode[aidx] = (byte)lidx;
		} // end-for
		// Set '+'
		b64decode[0x2b] = (byte)lidx++;
		// Set '/'
		b64decode[0x2f] = (byte)lidx++;
	} // end-method

	private boolean	isb64char(byte b) {
		// A-Z is 0x41-0x5a
		// a-z is 0x61-0x7a
		// 0-9 is 0x30-0x39
		// + is 0x2b
		// / is 0x2f
		return (   (b >= 0x41 && b <= 0x5a)
			|| (b >= 0x61 && b <= 0x7a)
			|| (b >= 0x30 && b <= 0x39)
			|| (b == 0x2b)
			|| (b == 0x2f)
			);
	} // end-method
}
