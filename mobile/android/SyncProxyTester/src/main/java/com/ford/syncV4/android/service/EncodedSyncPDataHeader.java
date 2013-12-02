package com.ford.syncV4.android.service;

import com.ford.syncV4.util.BitConverter;

public class EncodedSyncPDataHeader {
	private byte protocolVersion;
	private boolean responseRequired;
	private boolean highBandwidth;
	private boolean signed;
	private boolean encrypted;
	private boolean hasESN;
	private byte serviceType;
	private byte commandType;
	private boolean CPUDestination;
	private byte encryptionKeyIndex;
	private int payloadSize;
	private byte[] ESN;
	private int moduleMessageID;
	private int serverMessageID;
	private byte messageStatus;
	private byte[] IV;
	private byte[] payload;
	private byte[] signatureTag;
	
	public EncodedSyncPDataHeader() {}
	
	public static EncodedSyncPDataHeader parseEncodedSyncPDataHeader(byte[] header) {
		EncodedSyncPDataHeader msg = new EncodedSyncPDataHeader();
		
		int bandwidthStart;
		
		byte protocolVersion = (byte) (header[0] >>> 5);
		msg.setProtocolVersion(protocolVersion);
		
		boolean responseRequired = 1 == ((header[0] & 0x10) >>> 4);
		msg.setResponseRequired(responseRequired);
		
		boolean highBandwidth = 1 == ((header[0] & 0x08) >>> 3);
		msg.setHighBandwidth(highBandwidth);
		
		boolean signed = 1 == ((header[0] & 0x04) >>> 2);
		msg.setSigned(signed);
		
		boolean encrypted = 1 == ((header[0] & 0x02) >>> 1);
		msg.setEncrypted(encrypted);
		
		boolean hasESN = 1 == (header[0] & 0x01);
		msg.setHasESN(hasESN);
		
		byte serviceType = header[1];
		msg.setServiceType(serviceType);

		byte commandType = (byte) (header[2] >>> 4);
		msg.setCommandType(commandType);

		boolean CPUDestination = 1 == ((header[2] & 0x08) >>> 3);
		msg.setCPUDestination(CPUDestination);

		byte encryptionKeyIndex = (byte) (header[2] & 0x07);
		msg.setEncryptionKeyIndex(encryptionKeyIndex);
		
		if (msg.getHighBandwidth()) {
			bandwidthStart = 7;
			int payloadSize = BitConverter.intFromByteArray(header, 3);
			msg.setPayloadSize(payloadSize);
		} else {
			bandwidthStart = 5;
			short payloadSize = BitConverter.shortFromByteArray(header, 3);
			msg.setPayloadSize(payloadSize);
		}

		byte[] ESN = new byte[8];
		int ESNLoc = 0;
		for (int i = bandwidthStart; i < bandwidthStart + 8; i++) {
			ESN[ESNLoc] = (byte) (header[i]);
			ESNLoc++;
		}
		msg.setESN(ESN);
		
		if (msg.getHighBandwidth()) {
			if (msg.getResponseRequired()) {
				bandwidthStart += 17;
				
				int moduleMessageID = BitConverter.intFromByteArray(header, 15);
				msg.setModuleMessageID(moduleMessageID);
	
				int serverMessageID = BitConverter.intFromByteArray(header, 19);
				msg.setServerMessageID(serverMessageID);
	
				byte messageStatus = header[23];
				msg.setMessageStatus(messageStatus);
			} else {
				bandwidthStart += 8;
			}

			byte[] IV = new byte[16];
			int IVLoc = 0;
			for (int i = bandwidthStart; i < bandwidthStart + 16; i++) {
				IV[IVLoc] = (byte) (header[i]);
				IVLoc++;
			}
			msg.setIV(IV);
			
			bandwidthStart += 16;

			byte[] payload = new byte[msg.getPayloadSize()];
			int payloadLoc = 0;
			for (int i = bandwidthStart; i < bandwidthStart + msg.getPayloadSize(); i++) {
				payload[payloadLoc] = (byte) (header[i]);
				payloadLoc++;
			}
			msg.setPayload(payload);
			
			bandwidthStart += msg.getPayloadSize();

			byte[] signatureTag = new byte[16];
			int signatureTagLoc = 0;
			for (int i = bandwidthStart; i < bandwidthStart + 16; i++) {
				signatureTag[signatureTagLoc] = (byte) (header[i]);
				signatureTagLoc++;
			}
			msg.setSignatureTag(signatureTag);
			
		} else {
			bandwidthStart += 8;

			byte[] IV = new byte[8];
			int IVLoc = 0;
			for (int i = bandwidthStart; i < bandwidthStart + 8; i++) {
				IV[IVLoc] = (byte) (header[i]);
				IVLoc++;
			}
			msg.setIV(IV);
			
			bandwidthStart += 8;

			byte[] payload = new byte[msg.getPayloadSize()];
			int payloadLoc = 0;
			for (int i = bandwidthStart; i < bandwidthStart + msg.getPayloadSize(); i++) {
				payload[payloadLoc] = (byte) (header[i]);
				payloadLoc++;
			}
			msg.setPayload(payload);
			
			bandwidthStart += msg.getPayloadSize();

			byte[] signatureTag = new byte[8];
			int signatureTagLoc = 0;
			for (int i = bandwidthStart; i < bandwidthStart + 8; i++) {
				signatureTag[signatureTagLoc] = (byte) (header[i]);
				signatureTagLoc++;
			}
			msg.setSignatureTag(signatureTag);
		}
		
		return msg;
	}
	
	public byte[] assembleEncodedSyncPDataHeaderBytes() {
		int EncodedSyncPDataHeader = 0;
		//assign value on the right to EncodedSyncPHeader
		EncodedSyncPDataHeader |= protocolVersion;
		//shift 1 bit to the left
		EncodedSyncPDataHeader <<= 1;
		//convert boolean to int, then assign value 
		EncodedSyncPDataHeader |= (responseRequired ? 1 : 0);
		EncodedSyncPDataHeader <<= 1;
		EncodedSyncPDataHeader |= (highBandwidth ? 1 : 0);
		EncodedSyncPDataHeader <<= 1;
		EncodedSyncPDataHeader |= (signed ? 1 : 0);
		EncodedSyncPDataHeader <<= 1;
		EncodedSyncPDataHeader |= (encrypted ? 1 : 0);
		EncodedSyncPDataHeader <<= 1;
		EncodedSyncPDataHeader |= (hasESN ? 1 : 0);
		EncodedSyncPDataHeader <<= 8;
		EncodedSyncPDataHeader |= serviceType;
		EncodedSyncPDataHeader <<= 4;
		EncodedSyncPDataHeader |= commandType;
		EncodedSyncPDataHeader <<= 1;
		EncodedSyncPDataHeader |= (CPUDestination ? 1 : 0);
		EncodedSyncPDataHeader <<= 3;
		EncodedSyncPDataHeader |= encryptionKeyIndex;
		
		if (highBandwidth) {
			if (responseRequired) {
				int packetSize = 
						/*Standard Header Elements*/ 3 +
						/*Payload Size*/ 4 + 
						/*ESN*/ 8 + 
						/*Module Message ID*/ 4 + 
						/*Server Message ID*/ 4 + 
						/*Message Status*/ 1 + 
						/*IV*/ 16 + 
						/*Payload*/ payloadSize + 
						/*Signature Tag*/ 16;
				
				byte[] ret = new byte[packetSize];
				
				System.arraycopy(BitConverter.intToByteArray(EncodedSyncPDataHeader), 1, ret, 0, 3);
				System.arraycopy(BitConverter.intToByteArray(payloadSize), 0, ret, 3, 4);
				System.arraycopy(ESN, 0, ret, 7, 8);
				System.arraycopy(BitConverter.intToByteArray(moduleMessageID), 0, ret, 15, 4);
				System.arraycopy(BitConverter.intToByteArray(serverMessageID), 0, ret, 19, 4);
				ret[23] = messageStatus;
				System.arraycopy(IV, 0, ret, 24, 16);
				System.arraycopy(payload, 0, ret, 40, payloadSize);
				System.arraycopy(signatureTag, 0, ret, 40 + payloadSize, 16);
				
				return ret;
			} else {
				int packetSize = 
						/*Standard Header Elements*/ 3 +
						/*Payload Size*/ 4 + 
						/*ESN*/ 8 + 
						/*IV*/ 16 + 
						/*Payload*/ payloadSize + 
						/*Signature Tag*/ 16;
				
				byte[] ret = new byte[packetSize];
				
				System.arraycopy(BitConverter.intToByteArray(EncodedSyncPDataHeader), 1, ret, 0, 3);
				System.arraycopy(BitConverter.intToByteArray(payloadSize), 0, ret, 3, 4);
				System.arraycopy(ESN, 0, ret, 7, 8);
				System.arraycopy(IV, 0, ret, 15, 16);
				System.arraycopy(payload, 0, ret, 31, payloadSize);
				System.arraycopy(signatureTag, 0, ret, 31 + payloadSize, 16);
				
				return ret;
			}
		} else {
			int packetSize = 
					/*Standard Header Elements*/ 3 +
					/*Payload Size*/ 2 + 
					/*ESN*/ 8 + 
					/*IV*/ 8 + 
					/*Payload*/ payloadSize + 
					/*Signature Tag*/ 8;
			
			byte[] ret = new byte[packetSize];
			
			System.arraycopy(BitConverter.intToByteArray(EncodedSyncPDataHeader), 1, ret, 0, 3);
			System.arraycopy(BitConverter.shortToByteArray((short) payloadSize), 0, ret, 3, 2);
			System.arraycopy(ESN, 0, ret, 5, 8);
			System.arraycopy(IV, 0, ret, 13, 8);
			System.arraycopy(payload, 0, ret, 21, payloadSize);
			System.arraycopy(signatureTag, 0, ret, 21 + payloadSize, 8);
			
			return ret;
		}
	}
	
	public byte getProtocolVersion() {
		return protocolVersion;
	}

	public void setProtocolVersion(byte protocolVersion) {
		this.protocolVersion = protocolVersion;
	}

	public boolean getResponseRequired() {
		return responseRequired;
	}

	public void setResponseRequired(boolean responseRequired) {
		this.responseRequired = responseRequired;
	}

	public boolean getHighBandwidth() {
		return highBandwidth;
	}

	public void setHighBandwidth(boolean highBandwidth) {
		this.highBandwidth = highBandwidth;
	}

	public boolean getSigned() {
		return signed;
	}

	public void setSigned(boolean signed) {
		this.signed = signed;
	}

	public boolean getEncrypted() {
		return encrypted;
	}

	public void setEncrypted(boolean encrypted) {
		this.encrypted = encrypted;
	}

	public boolean getHasESN() {
		return hasESN;
	}

	public void setHasESN(boolean hasESN) {
		this.hasESN = hasESN;
	}

	public byte getServiceType() {
		return serviceType;
	}

	public void setServiceType(byte serviceType) {
		this.serviceType = serviceType;
	}

	public byte getCommandType() {
		return commandType;
	}

	public void setCommandType(byte commandType) {
		this.commandType = commandType;
	}

	public boolean getCPUDestination() {
		return CPUDestination;
	}

	public void setCPUDestination(boolean CPUDestination) {
		this.CPUDestination = CPUDestination;
	}

	public byte getEncryptionKeyIndex() {
		return encryptionKeyIndex;
	}

	public void setEncryptionKeyIndex(byte encryptionKeyIndex) {
		this.encryptionKeyIndex = encryptionKeyIndex;
	}

	public int getPayloadSize() {
		return payloadSize;
	}

	public void setPayloadSize(int payloadSize) {
		this.payloadSize = payloadSize;
	}

	public byte[] getESN() {
		return ESN;
	}

	public void setESN(byte[] ESN) {
		this.ESN = ESN;
	}

	public int getModuleMessageID() {
		return moduleMessageID;
	}

	public void setModuleMessageID(int moduleMessageID) {
		this.moduleMessageID = moduleMessageID;
	}

	public int getServerMessageID() {
		return serverMessageID;
	}

	public void setServerMessageID(int serverMessageID) {
		this.serverMessageID = serverMessageID;
	}

	public byte getMessageStatus() {
		return messageStatus;
	}

	public void setMessageStatus(byte messageStatus) {
		this.messageStatus = messageStatus;
	}

	public byte[] getIV() {
		return IV;
	}

	public void setIV(byte[] IV) {
		this.IV = IV;
	}

	public byte[] getPayload() {
		return payload;
	}

	public void setPayload(byte[] payload) {
		this.payload = payload;
	}

	public byte[] getSignatureTag() {
		return signatureTag;
	}

	public void setSignatureTag(byte[] signatureTag) {
		this.signatureTag = signatureTag;
	}
}
