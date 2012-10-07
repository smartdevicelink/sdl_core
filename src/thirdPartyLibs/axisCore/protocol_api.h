
/**
 * Start Session
 * @param servType - always RPC (0x07)
 * @param sessionID returned in the StartSessionACK message
 * @param hashCode returned in the StartSessionACK message
 */
P_STATUS startSession(SERVICE_TYPE servType, U32 &sessionID, U32 &hashCode);

/**
 * End Session
 * @param sessionID - id of session
 * @param hashCode - code, received in startSession()
 */
P_STATUS endSession(U32 sessionID, U32 hashCode);

/**
 * Send Data
 * @param sessionID - id of session
 * @param servType - service type (always RPC (0x07) )
 * @param dataLen - data size (bytes)
 * @param data - data array
 * @param compress - compressing (always 0)
 */
P_STATUS sendData(U8 sessionID, SERVICE_TYPE servType, U32 dataLen, U8 *data, BOOL compress);

/**
 * Receive Data
 * @param sessionID - id of session
 * @param messageID - id of message
 * @param servType - service type (always RPC (0x07) )
 * @param receivedDataLen - received data size (bytes)
 * @param data - data array
 */
P_STATUS receiveData(U8 sessionID, U32 messageID, SERVICE_TYPE servType, U32 *receivedDataLen, U8* data);