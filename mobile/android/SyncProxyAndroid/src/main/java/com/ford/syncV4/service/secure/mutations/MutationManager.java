package com.ford.syncV4.service.secure.mutations;

import com.ford.syncV4.service.secure.SecurityInternalError;
import com.ford.syncV4.util.logger.Logger;

import java.util.HashMap;

/**
 * Created by admin on 7/2/14.
 */
public class MutationManager {

    private static HashMap<SecurityInternalError, Class<? extends AbstractMutation>> mutationMap = new HashMap<SecurityInternalError, Class<? extends AbstractMutation>>();

    {
        mutationMap.put(SecurityInternalError.ERROR_INVALID_QUERY_ID, InvalidQueryIdMutation.class);
        mutationMap.put(SecurityInternalError.ERROR_INVALID_QUERY_SIZE, InvalidQuerySizeMutation.class);
        mutationMap.put(SecurityInternalError.ERROR_SSL_INVALID_DATA, InvalidSslDataMutation.class);
        mutationMap.put(SecurityInternalError.ERROR_SERVICE_NOT_PROTECTED, InvalidEncryptionFlagMutation.class);
        mutationMap.put(SecurityInternalError.ERROR_DECRYPTION_FAILED, InvalidPayloadMutation.class);
        mutationMap.put(SecurityInternalError.ERROR_INVALID_QUERY_SIZE, InvalidQuerySizeMutation.class);
        mutationMap.put(SecurityInternalError.UNKNOWN, NeutralMutation.class);
    }

    private AbstractMutation headMutation;

    public AbstractMutation getHeadMutation() {
        return headMutation;
    }

    public void addMutationForError(SecurityInternalError securityInternalError) {
        if (headMutation == null) {
            headMutation = reflectMutation(securityInternalError);
        } else {
            AbstractMutation mutation = reflectMutation(securityInternalError);
            headMutation.setLastMutation(mutation);
        }
    }

    private AbstractMutation reflectMutation(SecurityInternalError securityInternalError) {
        try {
            return createMutation(securityInternalError);
        } catch (IllegalAccessException e) {
            Logger.e(e.toString(), e);
        } catch (InstantiationException e) {
            Logger.e(e.toString(), e);
        }
        return new NeutralMutation();
    }

    private AbstractMutation createMutation(SecurityInternalError securityInternalError) throws IllegalAccessException, InstantiationException {
        return mutationMap.get(securityInternalError).newInstance();
    }

}
