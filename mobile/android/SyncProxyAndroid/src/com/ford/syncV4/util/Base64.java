package com.ford.syncV4.util;

public class Base64 {

	private static final String defaultString = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure";

	private static char[] map = new char[64];
	private static byte[] unmap = new byte['z' + 1];
	static {
		byte i = 0;
		for (char c = 'A'; c <= 'Z'; c++) {
			unmap[c] = i;
			map[i++] = c;
		}
		for (char c = 'a'; c <= 'z'; c++) {
			unmap[c] = i;
			map[i++] = c;
		}
		for (char c = '0'; c <= '9'; c++) {
			unmap[c] = i;
			map[i++] = c;
		}
		unmap['+'] = i;
		map[i++] = '+';
		unmap['/'] = i;
		map[i++] = '/';

		unmap['='] = 0;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		String toEncode = defaultString;
		if (args.length > 0) {
			toEncode = args[0];
		}
		System.out.println(toEncode);
		String encoded = encode(toEncode.getBytes());
		System.out.println(encoded);
		String decoded = new String(decode(encoded));
		System.out.println(decoded);
	}

	public static String encode(byte[] toEncode) {

		int iSrc = 0;
		int iDest = 0;
		int srcLen = toEncode.length;
		int destLen = (srcLen * 4 + 2) / 3;
		int padLen = ((srcLen + 2) / 3) * 4;
		char[] dest = new char[padLen];

		while (iSrc < srcLen) {
			int src1 = toEncode[iSrc++];
			int src2 = (iSrc < srcLen) ? toEncode[iSrc++] : 0;
			int src3 = (iSrc < srcLen) ? toEncode[iSrc++] : 0;
			int dest1 = (src1 >>> 2);
			int dest2 = (src1 << 4) | (src2 >> 4);
			dest2 &= 0x3F;
			int dest3 = (src2 << 2) | (src3 >>> 6);
			dest3 &= 0x3F;
			int dest4 = src3 & 0x3F;
			dest[iDest++] = map[dest1];
			dest[iDest++] = map[dest2];
			dest[iDest] = iDest < destLen ? map[dest3] : '=';
			iDest++;
			dest[iDest] = iDest < destLen ? map[dest4] : '=';
			iDest++;
		}

		return new String(dest);
	}

	public static byte[] decode(String toDecode) {
		int iSrc = 0;
		int iDest = 0;
		char[] src = toDecode.toCharArray();
		int srcLen = src.length;
		int destLen = ((srcLen / 4) * 3);
		if (src[srcLen - 1] == '=') {
			destLen--;
		}
		if (src[srcLen - 2] == '=') {
			destLen--;
		}
		byte[] dest = new byte[destLen];

		while (iSrc < srcLen) {
			byte src1 = unmap[src[iSrc++]];
			byte src2 = unmap[src[iSrc++]];
			byte src3 = unmap[src[iSrc++]];
			byte src4 = unmap[src[iSrc++]];
			int dest1 = (src1 << 2) | (src2 >>> 4);
			int dest2 = (src2 << 4) | (src3 >>> 2);
			int dest3 = (src3 << 6) | src4;
			dest[iDest++] = (byte) dest1;
			if (iDest < destLen) {
				dest[iDest++] = (byte) dest2;
			}
			if (iDest < destLen) {
				dest[iDest++] = (byte) dest3;
			}
		}

		return dest;
	}

}
