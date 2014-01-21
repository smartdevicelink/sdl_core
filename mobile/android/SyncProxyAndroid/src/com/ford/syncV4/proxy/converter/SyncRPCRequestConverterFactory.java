package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.protocol.enums.FunctionID;

/**
 * The default Sync factory.
 *
 * Created by enikolsky on 2014-01-20.
 */
public class SyncRPCRequestConverterFactory
        implements IRPCRequestConverterFactory {
    private IRPCRequestConverter defaultConverter;

    @Override
    public IRPCRequestConverter getConverterForFunctionName(
            String functionName) {
        if (isFunctionNameKnown(functionName)) {
            return getDefaultConverter();
        }

        return null;
    }

    private boolean isFunctionNameKnown(String functionName) {
        return -1 != FunctionID.getFunctionID(functionName);
    }

    private IRPCRequestConverter getDefaultConverter() {
        if (defaultConverter == null) {
            defaultConverter = new DefaultRPCRequestConverter();
        }

        return defaultConverter;
    }
}
