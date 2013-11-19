package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.SessionType;

/**
* Created by Andrew Batutin on 8/21/13.
*/
public class ProtocolMessageConverter {
    private ProtocolMessage _protocolMsg;
    private byte[] _data;
    private SessionType _sessionType;
    private int _version = 1;

    public ProtocolMessageConverter(ProtocolMessage protocolMsg, int version) {
        this._protocolMsg = protocolMsg;
        this._version = version;
    }

    public byte[] getData() {
        return _data;
    }

    public SessionType getSessionType() {
        return _sessionType;
    }

    public ProtocolMessageConverter generate() {
        _data = null;
        _sessionType = _protocolMsg.getSessionType();
        // TODO - get rid of this ugly if statements
        if (_sessionType == SessionType.Mobile_Nav && _version ==2){
            _data = _protocolMsg.getData();
            return this;
        }
        if (_version == 2) {
            if (_protocolMsg.getBulkData() != null) {
                _data = new byte[12 + _protocolMsg.getJsonSize() + _protocolMsg.getBulkData().length];
                _sessionType = SessionType.Bulk_Data;
            } else {
                _data = new byte[12 + _protocolMsg.getJsonSize()];
            }
            BinaryFrameHeader binFrameHeader = ProtocolFrameHeaderFactory.createBinaryFrameHeader(_protocolMsg.getRPCType(), _protocolMsg.getFunctionID(), _protocolMsg.getCorrID(), _protocolMsg.getJsonSize());
            System.arraycopy(binFrameHeader.assembleHeaderBytes(), 0, _data, 0, 12);
            System.arraycopy(_protocolMsg.getData(), 0, _data, 12, _protocolMsg.getJsonSize());
            if (_protocolMsg.getBulkData() != null) {
                System.arraycopy(_protocolMsg.getBulkData(), 0, _data, 12 + _protocolMsg.getJsonSize(), _protocolMsg.getBulkData().length);
            }
        } else {
            _data = _protocolMsg.getData();
        }
        return this;
    }
}
