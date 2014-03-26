package com.ford.syncV4.proxy;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 26.03.14
 * Time: 12:30
 */

import android.util.Log;

import com.ford.syncV4.proxy.constants.Names;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;

/**
 * This class implements the procedure of processing hash table of the notification in order to
 * identify Certificate Recall
 */
public class CertificateRecallProcessor {

    /**
     * Interface to provide callback of the certificate recall processor
     */
    public interface ICertificateRecallProcessor {

        /**
         * Dispatches when found a combination to recall certificate
         */
        public void onRecall();
    }

    private class CriteriaItem {

        private String[] mCriteria;

        private CriteriaItem(String[] mCriteria) {
            this.mCriteria = mCriteria;
        }
    }

    private static final String LOG_TAG = "CertificateRecallProcessor";

    /**
     * Callback implementation reference
     */
    private ICertificateRecallProcessor mProcessorCallback;

    private static final String[] CRITERIA_1 = new String[] {
            "name", "OnButtonPress", "buttonPressMode", "SHORT", "buttonName","TUNEUP"
    };
    private static final String[] CRITERIA_2 = new String[] {
            "name", "OnButtonPress", "buttonPressMode", "SHORT", "buttonName","SEEKLEFT"
    };
    private static final String[] CRITERIA_3 = new String[] {
            "name", "OnButtonPress", "buttonPressMode", "SHORT", "buttonName","TUNEDOWN"
    };
    private static final String[] CRITERIA_4 = new String[] {
            "name", "OnButtonPress", "buttonPressMode", "SHORT", "buttonName","SEEKRIGHT"
    };
    private static final String[] CRITERIA_5 = new String[] {
            "name", "OnButtonPress", "buttonPressMode", "SHORT", "buttonName","OK"
    };

    private static final List<CriteriaItem> sCriteriaList = new ArrayList<CriteriaItem>();

    /**
     * Counter of the matched criteria
     */
    private int mCounter = 0;

    public void setProcessorCallback(ICertificateRecallProcessor processorCallback) {
        mProcessorCallback = processorCallback;

        sCriteriaList.add(new CriteriaItem(CRITERIA_1));
        sCriteriaList.add(new CriteriaItem(CRITERIA_2));
        sCriteriaList.add(new CriteriaItem(CRITERIA_3));
        sCriteriaList.add(new CriteriaItem(CRITERIA_4));
        sCriteriaList.add(new CriteriaItem(CRITERIA_5));
    }

    /**
     * Process incoming data in order to find certificate recall combination
     * @param hashtable a data which comes from SDL via Notification
     */
    public void process(Hashtable hashtable) {
        if (mProcessorCallback == null) {
            throw new IllegalArgumentException(CertificateRecallProcessor.class.getSimpleName() +
                    " callback must be specified before start to process");
        }

        if (!hashtable.containsKey(Names.notification)) {
            Log.w(LOG_TAG, CertificateRecallProcessor.class.getSimpleName() + " this is not a " +
                    "Notification");
            return;
        }

        if (isUnderCriteria((Hashtable) hashtable.get(Names.notification), sCriteriaList.get(mCounter))) {
            mCounter++;

            if (mCounter == sCriteriaList.size()) {
                mCounter = 0;

                mProcessorCallback.onRecall();
            }
        } else {
            mCounter = 0;
        }
    }

    /**
     * Specify whether provided notification meet a criteria.
     *
     * @param hashtable    a data which comes from SDL via Notification
     * @param criteriaItem criteria item, object of
     *                     {@link com.ford.syncV4.proxy.CertificateRecallProcessor.CriteriaItem}
     * @return boolean value
     */
    private boolean isUnderCriteria(Hashtable hashtable, CriteriaItem criteriaItem) {
        if (hashtable == null) {
            return false;
        }
        if (!hashtable.containsKey(Names.name)) {
            return false;
        }
        String value = (String) hashtable.get(Names.name);
        if (value == null) {
            return false;
        }
        if (!value.equals(criteriaItem.mCriteria[1])) {
            return false;
        }
        if (!hashtable.containsKey(Names.parameters)) {
            return false;
        }
        Hashtable parametersHashtable = (Hashtable) hashtable.get(Names.parameters);
        if (parametersHashtable == null) {
            return false;
        }
        if (!parametersHashtable.containsKey(criteriaItem.mCriteria[2]) &&
                !parametersHashtable.containsKey(criteriaItem.mCriteria[4])) {
            return false;
        }
        value = (String) parametersHashtable.get(criteriaItem.mCriteria[2]);
        if (value == null) {
            return false;
        }
        if (!value.equals(criteriaItem.mCriteria[3])) {
            return false;
        }
        value = (String) parametersHashtable.get(criteriaItem.mCriteria[4]);
        return value != null && value.equals(criteriaItem.mCriteria[5]);
    }
}