package com.ford.syncV4.android.activity;

import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ListView;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;

import java.util.Vector;

public class SoftButtonsListActivity extends ListActivity {
    private final static String LOG_TAG =
            SoftButtonsListActivity.class.getSimpleName();
    private final static int MAXBUTTONS_DEFAULT = 10;
    /**
     * Index of the softButton being edited.
     */
    private int currentSoftButtonIndex = -1;
    private SoftButtonsAdapter adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.softbuttons);

        Button btnOk = ((Button) findViewById(R.id.softbuttons_ok));
        btnOk.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                IntentHelper.addObjectForKey(adapter.objects,
                        Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
                setResult(RESULT_OK);
                finish();
            }
        });

        Button btnCancel = ((Button) findViewById(R.id.softbuttons_cancel));
        btnCancel.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                setResult(RESULT_CANCELED);
                finish();
            }
        });

        Vector<SoftButton> softButtons =
                (Vector<SoftButton>) IntentHelper.getObjectForKey(
                        Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
        softButtons = (Vector<SoftButton>) softButtons.clone();
        int maxSoftButtonsNumber =
                getIntent().getIntExtra(Const.INTENT_KEY_SOFTBUTTONS_MAXNUMBER,
                        MAXBUTTONS_DEFAULT);
        if (softButtons.size() > maxSoftButtonsNumber) {
            softButtons.setSize(maxSoftButtonsNumber);
        }

        adapter =
                new SoftButtonsAdapter(this, softButtons, maxSoftButtonsNumber);
        setListAdapter(adapter);
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        if ((position == adapter.objects.size()) &&
                !adapter.isMaxReached()) {
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

            adapter.objects.add(sb);
            adapter.notifyDataSetChanged();
        } else {
            currentSoftButtonIndex = position;
            SoftButton softButton = adapter.objects.get(position);
            IntentHelper.addObjectForKey(softButton,
                    Const.INTENTHELPER_KEY_SOFTBUTTON);
            startActivityForResult(
                    new Intent(this, SoftButtonEditActivity.class),
                    Const.REQUEST_EDIT_SOFTBUTTON);
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode,
                                    Intent data) {
        switch (requestCode) {
            case Const.REQUEST_EDIT_SOFTBUTTON:
                if (resultCode == RESULT_OK) {
                    SoftButton result =
                            (SoftButton) IntentHelper.getObjectForKey(
                                    Const.INTENTHELPER_KEY_SOFTBUTTON);
                    adapter.objects.set(currentSoftButtonIndex, result);
                    adapter.notifyDataSetChanged();
                }
                currentSoftButtonIndex = -1;
                IntentHelper.removeObjectForKey(
                        Const.INTENTHELPER_KEY_SOFTBUTTON);
                break;
            default:
                Log.i(LOG_TAG, "Unknown request code: " + requestCode);
                break;
        }
    }
}
