package com.ford.syncV4.android.activity;

import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.proxy.RPCStruct;

import java.util.Vector;

/**
 * Created by enikolsky on 2013-11-25.
 */
public abstract class RPCStructListActivity<T extends RPCStruct>
        extends ListActivity {
    protected static final int MAXOBJECTS_DEFAULT = 10;
    private static final int REQUEST_EDIT_OBJECT = 42;
    private static final String LOG_TAG =
            RPCStructListActivity.class.getSimpleName();
    /**
     * Index of the object being edited.
     */
    protected int currentObjectIndex = -1;
    protected RPCStructAdapter<T> adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_rpcstructlist);

        Button btnOk = ((Button) findViewById(R.id.rpcstructlist_ok));
        btnOk.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                IntentHelper.addObjectForKey(adapter.objects,
                        Const.INTENTHELPER_KEY_OBJECTSLIST);
                setResult(RESULT_OK);
                finish();
            }
        });

        Button btnCancel = ((Button) findViewById(R.id.rpcstructlist_cancel));
        btnCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                setResult(RESULT_CANCELED);
                finish();
            }
        });

        Vector<T> objects = (Vector<T>) IntentHelper.getObjectForKey(
                Const.INTENTHELPER_KEY_OBJECTSLIST);
        objects = (Vector<T>) objects.clone();
        int maxObjectsNumber =
                getIntent().getIntExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER,
                        MAXOBJECTS_DEFAULT);
        if (objects.size() > maxObjectsNumber) {
            objects.setSize(maxObjectsNumber);
        }

        adapter = getAdapter(objects, maxObjectsNumber);
        setListAdapter(adapter);
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        if ((position == adapter.objects.size()) && !adapter.isMaxReached()) {
            adapter.objects.add(createNewObject());
            adapter.notifyDataSetChanged();
        } else {
            currentObjectIndex = position;
            onObjectClick(position);
        }
    }

    protected void onObjectClick(int position) {
        T object = adapter.objects.get(position);
        IntentHelper.addObjectForKey(object, Const.INTENTHELPER_KEY_OBJECT);
        startActivityForResult(new Intent(this, getObjectEditActivityClass()),
                REQUEST_EDIT_OBJECT);
    }

    protected abstract Class<? extends Activity> getObjectEditActivityClass();

    protected abstract RPCStructAdapter<T> getAdapter(Vector<T> objects,
                                                      int maxObjectsNumber);

    protected abstract T createNewObject();

    @Override
    protected void onActivityResult(int requestCode, int resultCode,
                                    Intent data) {
        switch (requestCode) {
            case REQUEST_EDIT_OBJECT:
                if (resultCode == RESULT_OK) {
                    T result = (T) IntentHelper.getObjectForKey(
                            Const.INTENTHELPER_KEY_OBJECT);
                    adapter.objects.set(currentObjectIndex, result);
                    adapter.notifyDataSetChanged();
                }
                currentObjectIndex = -1;
                IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_OBJECT);
                break;

            default:
                Log.i(LOG_TAG, "Unknown request code: " + requestCode);
                break;
        }
    }
}
