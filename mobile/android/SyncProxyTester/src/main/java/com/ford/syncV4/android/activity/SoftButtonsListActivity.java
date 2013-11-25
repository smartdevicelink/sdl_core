package com.ford.syncV4.android.activity;

import android.app.Activity;

import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;

import java.util.Vector;

public class SoftButtonsListActivity extends RPCStructListActivity<SoftButton> {
    @Override
    protected SoftButton createNewObject() {
        // create and add default soft button
        Image img = new Image();
        img.setValue("action.png");
        img.setImageType(ImageType.DYNAMIC);

        SoftButton sb = new SoftButton();
        sb.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
        sb.setText("Close");
        sb.setType(SoftButtonType.SBT_BOTH);
        sb.setImage(img);
        sb.setIsHighlighted(true);
        sb.setSystemAction(SystemAction.DEFAULT_ACTION);

        return sb;
    }

    @Override
    protected Class<? extends Activity> getObjectEditActivityClass() {
        return SoftButtonEditActivity.class;
    }

    @Override
    protected RPCStructAdapter<SoftButton> getAdapter(
            Vector<SoftButton> objects, int maxObjectsNumber) {
        return new SoftButtonsAdapter(this, objects, maxObjectsNumber);
    }

}
