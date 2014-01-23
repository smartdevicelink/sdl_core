package com.ford.syncV4.proxy.converter;

/**
 * The interface for factories that can create IRPCRequestConverter objects.
 *
 * Created by enikolsky on 2014-01-20.
 */
public interface IRPCRequestConverterFactory {
    /**
     * Returns a suitable converter for RPCRequest objects given the request
     * name. Since they should be stateless, the implementation caches them
     * between calls.
     *
     * @param functionName RPCRequest's method name; must not be null
     * @return a converter for the request, or null if function is unknown
     */
    public IRPCRequestConverter getConverterForFunctionName(
            String functionName);
}
