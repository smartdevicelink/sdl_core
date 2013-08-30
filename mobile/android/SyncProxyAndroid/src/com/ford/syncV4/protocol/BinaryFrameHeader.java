package com.ford.syncV4.protocol;

import com.ford.syncV4.util.BitConverter;

public class BinaryFrameHeader {
	private byte _rpcType;
	private int _functionID;
	private int _correlationID;
	private int _jsonSize;
	
	private byte[] _jsonData;
	private byte[] _bulkData;
	
	public BinaryFrameHeader() {}
	
	public static BinaryFrameHeader parseBinaryHeader(byte[] binHeader) {
		BinaryFrameHeader msg = new BinaryFrameHeader();
		
		byte RPC_Type = (byte) (binHeader[0] >>> 4);
		msg.setRPCType(RPC_Type);
		
		int _functionID = (BitConverter.intFromByteArray(binHeader, 0) & 0x0FFFFFFF);
		msg.setFunctionID(_functionID);
		
		int corrID = BitConverter.intFromByteArray(binHeader, 4);
		msg.setCorrID(corrID);
		
		int _jsonSize = BitConverter.intFromByteArray(binHeader, 8);
		msg.setJsonSize(_jsonSize);
		
		if (_jsonSize > 0) {
			byte[] _jsonData = new byte[_jsonSize];
			System.arraycopy(binHeader, 12, _jsonData, 0, _jsonSize);
			msg.setJsonData(_jsonData);
		}
		
		if (binHeader.length - _jsonSize - 12 > 0) {
			byte[] _bulkData = new byte[binHeader.length - _jsonSize - 12];
			System.arraycopy(binHeader, 12 + _jsonSize, _bulkData, 0, _bulkData.length);
			msg.setBulkData(_bulkData);
		}		
		
		return msg;
	}
	
	protected byte[] assembleHeaderBytes() {
		int binHeader = _functionID;
        // reset the 4 leftmost bits, for _rpcType
        binHeader &= 0xFFFFFFFF >>> 4;
		binHeader |= (_rpcType << 28);
		
		byte[] ret = new byte[12];
		System.arraycopy(BitConverter.intToByteArray(binHeader), 0, ret, 0, 4);
		System.arraycopy(BitConverter.intToByteArray(_correlationID), 0, ret, 4, 4);
		System.arraycopy(BitConverter.intToByteArray(_jsonSize), 0, ret, 8, 4);
		
		return ret;
	}
	
	public byte getRPCType() {
		return _rpcType;
	}

	public void setRPCType(byte _rpcType) {
		this._rpcType = _rpcType;
	}

	public int getFunctionID() {
		return _functionID;
	}

	public void setFunctionID(int _functionID) {
		this._functionID = _functionID;
	}

	public int getCorrID() {
		return _correlationID;
	}

	public void setCorrID(int _correlationID) {
		this._correlationID = _correlationID;
	}

	public int getJsonSize() {
		return _jsonSize;
	}

	public void setJsonSize(int _jsonSize) {
		this._jsonSize = _jsonSize;
	}
	
	public byte[] getJsonData() {
		return _jsonData;
	}
	
	public void setJsonData(byte[] _jsonData) {
		this._jsonData = new byte[this._jsonSize];
		System.arraycopy(_jsonData, 0, this._jsonData, 0, _jsonSize);
		//this._jsonData = _jsonData;
	}
	
	public byte[] getBulkData() {
		return _bulkData;
	}
	
	public void setBulkData(byte[] _bulkData) {
		this._bulkData = _bulkData;
	}
}
