package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.ProtocolConstants;

/**
* Created by Andrew Batutin on 8/21/13.
*/
public class ProtocolMessageConverter {
    private ProtocolMessage _protocolMsg;
    private byte[] _data;
    private ServiceType _serviceType;
    private int mProtocolVersion = ProtocolConstants.PROTOCOL_VERSION_ONE;

    public ProtocolMessageConverter(ProtocolMessage protocolMsg, int version) {
        this._protocolMsg = protocolMsg;
        this.mProtocolVersion = version;
    }

    public byte[] getData() {
        return _data;
    }

    public ServiceType getSessionType() {
        return _serviceType;
    }

    public ProtocolMessageConverter generate() {
        _data = null;
        _serviceType = _protocolMsg.getServiceType();
        // TODO - get rid of this ugly if statements. FAST!
        if ((_serviceType == ServiceType.Mobile_Nav ||
                _serviceType == ServiceType.Audio_Service ) &&
                mProtocolVersion == ProtocolConstants.PROTOCOL_VERSION_TWO) {
            _data = _protocolMsg.getData();
            return this;
        }
        if (mProtocolVersion == ProtocolConstants.PROTOCOL_VERSION_TWO) {
            if (_protocolMsg.getBulkData() != null) {
                _data = new byte[ProtocolConstants.HEADER_SIZE_V_2 + _protocolMsg.getJsonSize() +
                        _protocolMsg.getBulkData().length];
                _serviceType = ServiceType.Bulk_Data;
            } else {
                _data = new byte[ProtocolConstants.HEADER_SIZE_V_2 + _protocolMsg.getJsonSize()];
            }
            BinaryFrameHeader binFrameHeader =
                    ProtocolFrameHeaderFactory.createBinaryFrameHeader(_protocolMsg.getRPCType(),
                            _protocolMsg.getFunctionID(), _protocolMsg.getCorrID(),
                            _protocolMsg.getJsonSize());
            System.arraycopy(binFrameHeader.assembleHeaderBytes(), 0, _data, 0,
                    ProtocolConstants.HEADER_SIZE_V_2);
            System.arraycopy(_protocolMsg.getData(), 0, _data, ProtocolConstants.HEADER_SIZE_V_2,
                    _protocolMsg.getJsonSize());
            if (_protocolMsg.getBulkData() != null) {
                System.arraycopy(_protocolMsg.getBulkData(), 0, _data,
                        ProtocolConstants.HEADER_SIZE_V_2 + _protocolMsg.getJsonSize(),
                        _protocolMsg.getBulkData().length);
            }
        } else {
            _data = _protocolMsg.getData();
        }
        return this;
    }
}