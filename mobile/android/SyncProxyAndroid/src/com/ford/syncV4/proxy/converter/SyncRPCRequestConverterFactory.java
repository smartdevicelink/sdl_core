package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.protocol.enums.FunctionID;
import com.ford.syncV4.proxy.constants.Names;

/**
 * The default Sync factory.
 *
 * Created by enikolsky on 2014-01-20.
 */
public class SyncRPCRequestConverterFactory
        implements IRPCRequestConverterFactory {
    private IRPCRequestConverter defaultConverter;
    private IRPCRequestConverter putFileConverter;

    @Override
    public IRPCRequestConverter getConverterForFunctionName(
            String functionName) {
        IRPCRequestConverter converter = null;

        if (isFunctionNameKnown(functionName)) {
            if (Names.PutFile.equals(functionName)) {
                if (putFileConverter == null) {
                    putFileConverter = new PutFileRPCRequestConverter();
                }

                converter = putFileConverter;
            } else {
                converter = getDefaultConverter();
            }
        }

        return converter;
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
