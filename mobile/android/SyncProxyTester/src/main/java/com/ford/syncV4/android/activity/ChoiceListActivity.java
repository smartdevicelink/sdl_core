package com.ford.syncV4.android.activity;

import android.app.Activity;

import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.enums.ImageType;

import java.util.Vector;

/**
 * Created by enikolsky on 2013-11-25.
 */
public class ChoiceListActivity extends RPCStructListActivity<Choice> {
    @Override
    protected Class<? extends Activity> getObjectEditActivityClass() {
        return ChoiceEditActivity.class;
    }

    @Override
    protected RPCStructAdapter<Choice> getAdapter(Vector<Choice> objects,
                                                  int maxObjectsNumber) {
        return new ChoiceAdapter(this, objects, maxObjectsNumber);
    }

    @Override
    protected Choice createNewObject() {
        Choice choice = new Choice();

        choice.setChoiceID(SyncProxyTester.getNewChoiceId());
        choice.setMenuName("The Show");
        choice.setSecondaryText("Must");
        choice.setTertiaryText("Go On");

        Image image = new Image();
        image.setImageType(ImageType.DYNAMIC);
        image.setValue("action.png");
        choice.setImage(image);

        return choice;
    }
}
