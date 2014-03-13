package com.ford.syncV4.protocol;

import android.util.Log;

import com.ford.syncV4.util.BitConverter;

public class BinaryFrameHeader {

    private static final String TAG = "BinaryFrameHeader";

    private byte mRpcType;
    private int mFunctionID;
    private int mCorrelationID;
    private int mJsonSize;

    private byte[] mJsonData;
    private byte[] mBulkData;

    public BinaryFrameHeader() {

    }

    /**
     * Parse Secure Service binary protocol data
     *
     * @param binHeader Secure service binary protocol
     *
     * @return Secure Service binary protocol data as array of the bytes
     */
    public static byte[] parseSecureServiceBinaryHeaderData(byte[] binHeader) {

        final int binarySecureDataSize = binHeader.length - ProtocolConst.PROTOCOL_V2_HEADER_SIZE;
        if (binarySecureDataSize > 0) {
            byte[] mSecureData = new byte[binarySecureDataSize];
            System.arraycopy(binHeader,
                    ProtocolConst.PROTOCOL_V2_HEADER_SIZE, mSecureData, 0, mSecureData.length);
            return mSecureData;
        }
        return new byte[0];
    }

    public static BinaryFrameHeader parseBinaryHeader(byte[] binHeader) {
        BinaryFrameHeader msg = new BinaryFrameHeader();

        byte RPC_Type = (byte) (binHeader[0] >>> 4);
        msg.setRPCType(RPC_Type);

        int functionID = (BitConverter.intFromByteArray(binHeader, 0) & 0x0FFFFFFF);
        msg.setFunctionID(functionID);

        int corrID = BitConverter.intFromByteArray(binHeader, 4);
        msg.setCorrID(corrID);

        int jsonSize = BitConverter.intFromByteArray(binHeader, 8);
        msg.setJsonSize(jsonSize);

        if (jsonSize > 0) {

            Log.d(TAG, "JSON size:" + jsonSize + ", header:" + binHeader.length);

            byte[] jsonData = new byte[jsonSize];
            System.arraycopy(binHeader, ProtocolConst.PROTOCOL_V2_HEADER_SIZE, jsonData, 0,
                    jsonSize);
            msg.setJsonData(jsonData);
        } else if (jsonSize < 0) {
            // the size is over 2 GB; Java doesn't allow us to create such arrays
            final long jsonSizeNew = BitConverter.unsignedIntFromByteArray(binHeader, 8);
            throw new OutOfMemoryError(
                    String.format("Can't allocate memory for JSON of %d bytes", jsonSizeNew));
        }

        final int binaryDataSize = binHeader.length - jsonSize - ProtocolConst.PROTOCOL_V2_HEADER_SIZE;
        if (binaryDataSize > 0) {
            byte[] bulkData = new byte[binaryDataSize];
            System.arraycopy(binHeader,
                    ProtocolConst.PROTOCOL_V2_HEADER_SIZE + jsonSize, bulkData, 0, bulkData.length);
            msg.setBulkData(bulkData);
        }

        return msg;
    }

    protected byte[] assembleHeaderBytes() {
        int binHeader = mFunctionID;
        // reset the 4 leftmost bits, for mRpcType
        binHeader &= 0xFFFFFFFF >>> 4;
        binHeader |= (mRpcType << 28);

        byte[] ret = new byte[ProtocolConst.PROTOCOL_V2_HEADER_SIZE];
        System.arraycopy(BitConverter.intToByteArray(binHeader), 0, ret, 0, 4);
        System.arraycopy(BitConverter.intToByteArray(mCorrelationID), 0, ret, 4, 4);
        System.arraycopy(BitConverter.intToByteArray(mJsonSize), 0, ret, 8, 4);

        return ret;
    }

    public byte getRPCType() {
        return mRpcType;
    }

    public void setRPCType(byte rpcType) {
        this.mRpcType = rpcType;
    }

    public int getFunctionID() {
        return mFunctionID;
    }

    public void setFunctionID(int functionID) {
        this.mFunctionID = functionID;
    }

    public int getCorrID() {
        return mCorrelationID;
    }

    public void setCorrID(int correlationID) {
        this.mCorrelationID = correlationID;
    }

    public int getJsonSize() {
        return mJsonSize;
    }

    public void setJsonSize(int jsonSize) {
        this.mJsonSize = jsonSize;
    }

    public byte[] getJsonData() {
        return mJsonData;
    }

    public void setJsonData(byte[] jsonData) {
        this.mJsonData = new byte[this.mJsonSize];
        System.arraycopy(jsonData, 0, this.mJsonData, 0, mJsonSize);
        //this.mJsonData = mJsonData;
    }

    public byte[] getBulkData() {
        return mBulkData;
    }

    public void setBulkData(byte[] bulkData) {
        this.mBulkData = bulkData;
    }

    @Override
    public String toString() {
        return "BinaryFrameHeader{" +
                "mRpcType=" + mRpcType +
                ", mFunctionID=" + mFunctionID +
                ", mCorrelationID=" + mCorrelationID +
                ", mJsonSize=" + mJsonSize +
                ", mJsonData=" + BitConverter.bytesToHex(mJsonData) +
                ", mBulkData=" + BitConverter.bytesToHex(mBulkData) +
                '}';
    }
}