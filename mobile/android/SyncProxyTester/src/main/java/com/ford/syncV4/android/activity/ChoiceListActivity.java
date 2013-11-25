package com.ford.syncV4.android.activity;

import android.app.Activity;

import com.ford.syncV4.proxy.rpc.Choice;

import java.util.Vector;

/**
 * Created by enikolsky on 2013-11-25.
 */
public class ChoiceListActivity extends RPCStructListActivity<Choice> {
    @Override
    protected Class<? extends Activity> getObjectEditActivityClass() {
        return null;
    }

    @Override
    protected RPCStructAdapter<Choice> getAdapter(Vector<Choice> objects,
                                                  int maxObjectsNumber) {
        return new ChoiceAdapter(this, objects, maxObjectsNumber);
    }

    @Override
    protected Choice createNewObject() {
        Choice choice = new Choice();
        return choice;
    }
}
