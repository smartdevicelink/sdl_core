package com.ford.syncV4.android.manager;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.PlaceholderFragment;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.util.logger.Logger;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 15.04.14
 * Time: 13:20
 */
public class ApplicationIconManager {

    private static final String TAG = ApplicationIconManager.class.getSimpleName();
    private static final String ICON_SYNC_FILENAME = "icon.png";

    private int mAwaitingInitIconResponseCorrelationID = 0;
    private String mAppId = PlaceholderFragment.EMPTY_APP_ID;

    public ApplicationIconManager(String appId) {
        mAppId = appId;
    }

    public void setApplicationIcon(ProxyService proxyService) {

        if (isApplicationIconSet()) {
            Logger.d(TAG + " Application Icon has been set up before");
            return;
        }
        sendPutFileForAppIcon(proxyService);
    }

    public boolean isApplicationIconSet() {
        return mAwaitingInitIconResponseCorrelationID == -1;
    }

    public void reset() {
        Logger.d(TAG + " Reset");
        mAwaitingInitIconResponseCorrelationID = 0;
    }

    private void sendPutFileForAppIcon(ProxyService proxyService) {
        Logger.d(TAG + " PutFile");
        mAwaitingInitIconResponseCorrelationID = proxyService.getNextCorrelationID();
        proxyService.commandPutFile(mAppId, FileType.GRAPHIC_PNG, ICON_SYNC_FILENAME,
                AppUtils.contentsOfResource(R.raw.main_logo),
                mAwaitingInitIconResponseCorrelationID, true);
    }

    public void setAppIcon(ProxyService proxyService, int receivedCorrelationId) {
        if (isApplicationIconSet()) {
            Logger.d(TAG + " Application Icon has been set up before");
            return;
        }

        if (mAwaitingInitIconResponseCorrelationID != receivedCorrelationId) {
            Logger.d(TAG + " Application Icon set up correlation Id's does not match");
            return;
        }

        Logger.d(TAG + " SetAppIcon");
        SetAppIcon setAppIcon = RPCRequestFactory.buildSetAppIcon();
        setAppIcon.setSyncFileName(ICON_SYNC_FILENAME);
        setAppIcon.setCorrelationID(proxyService.getNextCorrelationID());

        proxyService.syncProxySendRPCRequestWithPreprocess(mAppId, setAppIcon);

        Logger.i(TAG + " Application Icon set up complete");

        mAwaitingInitIconResponseCorrelationID = -1;
    }
}