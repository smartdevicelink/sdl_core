package com.ford.syncV4.proxy.systemrequest;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.SystemRequest;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 11.07.14
 * Time: 10:22
 */

/**
 * This is a class which help to process different scenarios of the
 * {@link com.ford.syncV4.proxy.rpc.OnSystemRequest} notification processing algorithm (whether to
 * create {@link com.ford.syncV4.proxy.rpc.PutFile} or
 * {@link com.ford.syncV4.proxy.rpc.SystemRequest}) or do any other actions)
 */
public class SystemRequestProxyImpl implements ISystemRequestProxy {

    /**
     * Interface that provide a callback methods for the
     * {@link com.ford.syncV4.proxy.systemrequest.SystemRequestProxyImpl} processing results
     */
    public interface ISystemRequestProxyCallback {

        /**
         * Method to indicated that {@link com.ford.syncV4.proxy.RPCRequest} has been created and
         * ready to pass for further processing
         *
         * @param appId      Application unique identifier
         * @param rpcRequest created {@link com.ford.syncV4.proxy.RPCRequest}
         *
         * @throws SyncException
         */
        public void onSendingRPCRequest(String appId, RPCRequest rpcRequest) throws SyncException;
    }

    /**
     * Callback reference
     */
    private ISystemRequestProxyCallback mCallback;

    /**
     * Constructor
     *
     * @param callback reference to the
     * {@link com.ford.syncV4.proxy.systemrequest.SystemRequestProxyImpl.ISystemRequestProxyCallback}
     * implementation
     */
    public SystemRequestProxyImpl(ISystemRequestProxyCallback callback) {
        if (callback == null) {
            throw new NullPointerException(getClass().getSimpleName() + "-> constructor -> " +
                    " callback reference can not be NULL");
        }
        mCallback = callback;
    }

    @Override
    public void putSystemFile(String appId, String filename, byte[] data, FileType fileType)
            throws SyncException {

        putSystemFile(appId, filename, data, null, fileType);
    }

    @Override
    public void putSystemFile(String appId, String filename, byte[] data, Integer offset,
                              FileType fileType) throws SyncException {

        final PutFile putFile = RPCRequestFactory.buildPutFile(filename, fileType, null, data);
        putFile.setSystemFile(true);
        putFile.setOffset(offset != null ? offset : 0);

        mCallback.onSendingRPCRequest(appId, putFile);
    }

    @Override
    public void putPolicyTableUpdateFile(String appId, String filename, byte[] data,
                                         FileType fileType, RequestType requestType)
            throws SyncException {

        if (fileType == FileType.BINARY) {
            PutFile putFile = RPCRequestFactory.buildPutFile(filename, FileType.JSON, null, data);
            putFile.setSystemFile(true);

            // Up to current requirements:
            // /doc/technical/requirements/ISVU/SYNCP Final Interfaces v2.pdf
            // there is no specification about offset, we set it to 0 (assuming that we
            // upload a hole file to the SDL)
            putFile.setOffset(0);

            mCallback.onSendingRPCRequest(appId, putFile);
        } else if (fileType == FileType.JSON) {
            SystemRequest systemRequest = RPCRequestFactory.buildSystemRequest(filename, data,
                    requestType);

            mCallback.onSendingRPCRequest(appId, systemRequest);
        }
    }
}