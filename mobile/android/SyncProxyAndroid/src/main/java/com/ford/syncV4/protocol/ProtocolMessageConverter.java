package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.secure.SecureServiceRequestResponseSeqNumberHolder;

/**
 * Created by Andrew Batutin on 8/21/13
 * Co-author Yuriy Chernyshov
 */
public class ProtocolMessageConverter {

    private static final int FRAME_HEADER_LENGTH = 12;
    private ProtocolMessage mProtocolMsg;
    private byte[] mData;
    private ServiceType mServiceType;
    private int mProtocolVersion = 1;

    public ProtocolMessageConverter(ProtocolMessage protocolMsg, int version) {
        this.mProtocolMsg = protocolMsg;
        this.mProtocolVersion = version;
    }

    public byte[] getData() {
        return mData;
    }

    public ServiceType getSessionType() {
        return mServiceType;
    }

    /**
     *
     * @param serviceTypeToBeSecured
     * @return
     */
    public ProtocolMessageConverter generate(ServiceType serviceTypeToBeSecured) {
        if (serviceTypeToBeSecured == null){
            serviceTypeToBeSecured = ServiceType.Audio_Service;
        }
        mData = null;
        mServiceType = mProtocolMsg.getServiceType();

        if (mProtocolVersion == 2) {

            // TODO - Ugly way to create Secure Service payload data (binary frame header + data)
//            if (mServiceType == ServiceType.Heartbeat && ) {
//                byte[] secureData = mProtocolMsg.getData().clone();
//                mData = new byte[FRAME_HEADER_LENGTH + secureData.length];
//
//                SecureServiceRequestResponseSeqNumberHolder holder =
//                        SecureServiceRequestResponseSeqNumberHolder.getInstance();
//                int seqNumber = holder.getNextSeqNumber();
//
//                holder.addServiceWithKey(seqNumber, serviceTypeToBeSecured.getName());
//
//                BinaryFrameHeader binFrameHeader =
//                        ProtocolFrameHeaderFactory.createBinaryFrameHeader(mProtocolMsg.getRPCType(),
//                                mProtocolMsg.getFunctionID(), seqNumber, 0);
//                System.arraycopy(binFrameHeader.assembleHeaderBytes(), 0, mData, 0,
//                        FRAME_HEADER_LENGTH);
//                System.arraycopy(secureData, 0, mData, FRAME_HEADER_LENGTH, secureData.length);
//
//                return this;
//            }

            // TODO - get rid of this ugly if statements. FAST!
            if (mServiceType == ServiceType.Mobile_Nav ||
                    mServiceType == ServiceType.Audio_Service) {
                mData = mProtocolMsg.getData();
                return this;
            }

            if (mProtocolMsg.getBulkData() != null) {
                mData = new byte[FRAME_HEADER_LENGTH + mProtocolMsg.getJsonSize() +
                        mProtocolMsg.getBulkData().length];
                mServiceType = ServiceType.Bulk_Data;
            } else {
                mData = new byte[FRAME_HEADER_LENGTH + mProtocolMsg.getJsonSize()];
            }
            BinaryFrameHeader binFrameHeader =
                    ProtocolFrameHeaderFactory.createBinaryFrameHeader(mProtocolMsg.getRPCType(),
                            mProtocolMsg.getFunctionID(), mProtocolMsg.getCorrID(),
                            mProtocolMsg.getJsonSize());
            System.arraycopy(binFrameHeader.assembleHeaderBytes(), 0, mData, 0, FRAME_HEADER_LENGTH);
            System.arraycopy(mProtocolMsg.getData(), 0, mData, FRAME_HEADER_LENGTH,
                    mProtocolMsg.getJsonSize());
            if (mProtocolMsg.getBulkData() != null) {
                System.arraycopy(mProtocolMsg.getBulkData(), 0, mData,
                        FRAME_HEADER_LENGTH + mProtocolMsg.getJsonSize(),
                        mProtocolMsg.getBulkData().length);
            }
        } else {
            mData = mProtocolMsg.getData();
        }
        return this;
    }

    public ProtocolMessageConverter generate() {
        return generate(null);
    }
}