package com.ford.syncV4.android.policies;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/26/14
 * Time: 9:59 AM
 */

import android.os.Environment;
import android.util.Log;

import com.ford.syncV4.android.activity.SafeToast;
import com.ford.syncV4.android.adapters.LogAdapter;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;
import com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy;

import java.io.File;

/**
 * This class provide possibility to save Policy Table Snapshot file (which comes from SDL)
 * and to send back Policy Table Update file
 */
public class PolicyFilesManager {

    /**
     * Save Policy Table Snapshot data to file
     *
     * @param data Policy Table Snapshot data
     */
    public static void savePolicyTableSnapshot(byte[] data) {
        //final byte[] fileData = AppUtils.contentsOfResource(R.raw.policy_table_shanpshot);

        boolean result = AppUtils.saveDataToFile(data, Environment.getExternalStorageDirectory() + "/" +
                Const.TEST_DATA_DIR_NAME, "policyTableSnapshot.json");
        if (result) {
            SafeToast.showToastAnyThread("File '" + Environment.getExternalStorageDirectory() + "/" +
                    Const.TEST_DATA_DIR_NAME + "/policyTableSnapshot.json" + "' successfully saved");
        } else {
            SafeToast.showToastAnyThread("File '" + Environment.getExternalStorageDirectory() + "/" +
                    Const.TEST_DATA_DIR_NAME + "/policyTableSnapshot.json" + "' could not be save");
        }
    }

    /**
     * Send Policy Table Update data to SDL
     *
     * @param proxy {@link com.ford.syncV4.proxy.systemrequest.ISystemRequestProxy} implementation
     * @param fileType type of the file
     * @param logAdapter Log Adapter (TO BE REMOVED)
     */
    public static void sendPolicyTableUpdate(String appId, ISystemRequestProxy proxy, FileType fileType,
                                             RequestType requestType, LogAdapter logAdapter) {
        String mPolicyTableUpdatePath = AppPreferencesManager.getPolicyTableUpdateFilePath();
        //Environment.getExternalStorageDirectory() +
        //        "/policyTableUpdate.json";

        byte[] data = null;
        if (mPolicyTableUpdatePath.equals("")) {
            //data = AppUtils.contentsOfResource(R.raw.policy_table_update);
            data = AppUtils.contentsOfResource(Environment.getExternalStorageDirectory() + "/" +
                    Const.TEST_DATA_DIR_NAME + "/policyTableUpdate.json");
        } else {
            File mPolicyUpdateFile = new File(mPolicyTableUpdatePath);
            if (mPolicyUpdateFile.exists()) {
                data = AppUtils.contentsOfResource(mPolicyUpdateFile);
            }
        }

        if (data == null) {
            SafeToast.showToastAnyThread("Policy Snapshot could not be found");
            // TODO : Logging to be refactored
            if (logAdapter != null) {
                logAdapter.logMessage("Policy Snapshot could not be found", Log.ERROR, true);
            }
            return;
        }

        SafeToast.showToastAnyThread("Policy Update is found");

        // TODO : Logging to be refactored
        if (logAdapter != null) {
            logAdapter.logMessage("Policy Update is found", Log.DEBUG, true);
        }

        try {
            String mPolicyTableUpdateFileName = "PolicyTableUpdate";
            proxy.putPolicyTableUpdateFile(appId, mPolicyTableUpdateFileName, data, fileType, requestType);
            SafeToast.showToastAnyThread("Policy Update sent");
            if (logAdapter != null) {
                logAdapter.logMessage("Policy Update sent", Log.DEBUG, true);
            }
        } catch (SyncException e) {
            // TODO : Logging to be refactored
            if (logAdapter != null) {
                logAdapter.logMessage("Can't upload policy table update mPolicyUpdateFile:" +
                        e.getMessage(), Log.ERROR, true);
            }
        }
    }
}