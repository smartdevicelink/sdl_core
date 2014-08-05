package com.ford.syncV4.android.utils;

import android.os.Environment;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.SafeToast;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.util.logger.Logger;

import java.io.File;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 05.08.14
 * Time: 16:25
 */
public class PolicyUtils {

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
     * Help method to get Policy Update data for the Test purposes
     *
     * @return data as bytes array
     */
    public static byte[] getPolicyUpdateData() {

        String policyTableUpdatePath = AppPreferencesManager.getPolicyTableUpdateFilePath();

        byte[] data = new byte[0];
        if (policyTableUpdatePath.equals("")) {
            data = AppUtils.contentsOfResource(Environment.getExternalStorageDirectory() + "/" +
                    Const.TEST_DATA_DIR_NAME + "/policyTableUpdate.json");
        } else {
            File policyUpdateFile = new File(policyTableUpdatePath);
            if (policyUpdateFile.exists()) {
                data = AppUtils.contentsOfResource(policyUpdateFile);
            }
        }

        if (data == null) {
            Logger.w("Policy Snapshot could not be found");
        }

        String infoMessage = "Custom Policy Update is found";

        // Use dummy JSON update if real file do not provided
        if (data.length == 0) {
            data = AppUtils.contentsOfResource(R.raw.policy_table_update);

            infoMessage = "Predefined Policy Update is found";
        }

        Logger.d(infoMessage);

        return data;
    }
}