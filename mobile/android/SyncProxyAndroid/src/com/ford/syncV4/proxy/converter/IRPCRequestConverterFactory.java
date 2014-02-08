package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.proxy.RPCRequest;

/**
 * The interface for factories that can create IRPCRequestConverter objects.
 *
 * Created by enikolsky on 2014-01-20.
 */
public interface IRPCRequestConverterFactory {
    /**
     * Returns a suitable converter for the given RPCRequest object. Since they
     * should be stateless, the implementation caches them between calls.
     *
     * @param request RPCRequest to convert; must not be null
     * @return a converter for the request, or a default one in case of an error
     * (e.g., the request is invalid)
     */
    public IRPCRequestConverter getConverterForRequest(RPCRequest request);
}
