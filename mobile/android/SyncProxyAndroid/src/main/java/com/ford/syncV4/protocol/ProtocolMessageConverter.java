package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
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
    private int mProtocolVersion = ProtocolConstants.PROTOCOL_VERSION_ONE;

    public ProtocolMessageConverter(ProtocolMessage protocolMsg, int version) {
        this.mProtocolMsg = protocolMsg;
        this.mProtocolVersion = version;
    }

    public byte[] getData() {
        return mData;
    }

    public ServiceType getServiceType() {
        return mServiceType;
    }


    /**
     * @param serviceTypeToBeSecured
     * @return
     */
    public ProtocolMessageConverter generate(ServiceType serviceTypeToBeSecured) {
        if (serviceTypeToBeSecured == null) {
            serviceTypeToBeSecured = ServiceType.Audio_Service;
        }
        mData = null;
        mServiceType = mProtocolMsg.getServiceType();
        // TODO - get rid of this ugly if statements. FAST!
        if ((mServiceType == ServiceType.Mobile_Nav ||
                mServiceType == ServiceType.Audio_Service) &&
                mProtocolVersion >= ProtocolConstants.PROTOCOL_VERSION_TWO) {

            mData = mProtocolMsg.getData();
            return this;
        }

        if (mProtocolVersion >= ProtocolConstants.PROTOCOL_VERSION_TWO) {

            // TODO - Ugly way to create Secure Service payload data (binary frame header + data)
            if (mServiceType == ServiceType.Heartbeat) {
                byte[] secureData = mProtocolMsg.getData().clone();
                mData = new byte[FRAME_HEADER_LENGTH + secureData.length];

                SecureServiceRequestResponseSeqNumberHolder holder =
                        SecureServiceRequestResponseSeqNumberHolder.getInstance();
                int seqNumber = holder.getNextSeqNumber();

                holder.addServiceWithKey(seqNumber, serviceTypeToBeSecured.getName());

                BinaryFrameHeader binFrameHeader =
                        ProtocolFrameHeaderFactory.createBinaryFrameHeader(mProtocolMsg.getRPCType(),
                                mProtocolMsg.getFunctionID(), seqNumber, 0);
                System.arraycopy(binFrameHeader.assembleHeaderBytes(), 0, mData, 0,
                        FRAME_HEADER_LENGTH);
                System.arraycopy(secureData, 0, mData, FRAME_HEADER_LENGTH, secureData.length);

                return this;
            }

            if (mProtocolMsg.getBulkData() != null) {
                mData = new byte[ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2 + mProtocolMsg.getJsonSize() +
                        mProtocolMsg.getBulkData().length];
                mServiceType = ServiceType.Bulk_Data;
            } else {
                mData = new byte[ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2 + mProtocolMsg.getJsonSize()];
            }
            BinaryFrameHeader binFrameHeader =
                    ProtocolFrameHeaderFactory.createBinaryFrameHeader(mProtocolMsg.getRPCType(),
                            mProtocolMsg.getFunctionID(), mProtocolMsg.getCorrID(),
                            mProtocolMsg.getJsonSize());
            System.arraycopy(binFrameHeader.assembleHeaderBytes(), 0, mData, 0,
                    ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2);
            System.arraycopy(mProtocolMsg.getData(), 0, mData,
                    ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2,
                    mProtocolMsg.getJsonSize());
            if (mProtocolMsg.getBulkData() != null) {
                System.arraycopy(mProtocolMsg.getBulkData(), 0, mData,
                        ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2 +
                                mProtocolMsg.getJsonSize(),
                        mProtocolMsg.getBulkData().length
                );
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
