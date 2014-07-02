package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.PutFile;

/**
 * The default Sync factory.
 *
 * Created by enikolsky on 2014-01-20.
 */
public class SyncRPCRequestConverterFactory
        implements IRPCRequestConverterFactory {
    private IRPCRequestConverter defaultConverter;
    private IRPCRequestConverter systemPutFileConverter;

    @Override
    public IRPCRequestConverter getConverterForRequest(RPCRequest request) {
        IRPCRequestConverter converter = getDefaultConverter();

        final String functionName = request.getFunctionName();

        if (Names.PutFile.equals(functionName)) {
            PutFile putFile = (PutFile) request;
            final Boolean isSystemFile = putFile.getSystemFile();
            if ((isSystemFile != null) && isSystemFile) {
                converter = getSystemPutFileConverter();
            }
        }else if ( Names.internalTestSecurityError.equals(functionName)){
            converter = getSecureErrorRPCRequestConverter();
        }

        return converter;
    }

    private IRPCRequestConverter getDefaultConverter() {
        if (defaultConverter == null) {
            defaultConverter = new DefaultRPCRequestConverter();
        }

        return defaultConverter;
    }

    private IRPCRequestConverter getSystemPutFileConverter() {
        if (systemPutFileConverter == null) {
            systemPutFileConverter = new SystemPutFileRPCRequestConverter();
        }

        return systemPutFileConverter;
    }

    private IRPCRequestConverter getSecureErrorRPCRequestConverter() {
        return new SecureErrorRPCRequestConverter();
    }

}
