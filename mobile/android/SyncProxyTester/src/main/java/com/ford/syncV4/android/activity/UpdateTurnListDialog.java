package com.ford.syncV4.android.activity;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.Turn;
import com.ford.syncV4.proxy.rpc.UpdateTurnList;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;
import com.ford.syncV4.util.logger.Logger;

import java.util.Vector;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 7/08/14
 * Time: 10:09 AM
 */
public class UpdateTurnListDialog extends BaseDialogFragment {

    @SuppressWarnings("unused")
    private static final String LOG_TAG = UpdateTurnListDialog.class.getSimpleName();

    private static final int UPDATE_TURN_LIST_MAX_SOFT_BUTTONS = 1;

    private final Vector<SoftButton> currentSoftButtons = new Vector<SoftButton>();

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context context = getActivity();
        final LayoutInflater inflater = (LayoutInflater) context.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        final View layout = inflater.inflate(R.layout.updateturnlist,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        final EditText txtTurnList = (EditText) layout.findViewById(R.id.updateturnlist_txtTurnList);
        final EditText txtIconList = (EditText) layout.findViewById(R.id.updateturnlist_txtIconList);
        final CheckBox useTurnList = (CheckBox) layout.findViewById(R.id.updateturnlist_useTurnList);
        final CheckBox useIconList = (CheckBox) layout.findViewById(R.id.updateturnlist_useIconList);
        final CheckBox useSoftButtons = (CheckBox) layout.findViewById(R.id.updateturnlist_chkIncludeSBs);

        final SoftButton softButton = new SoftButton();
        softButton.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
        softButton.setText("Close");
        softButton.setType(SoftButtonType.SBT_TEXT);
        softButton.setIsHighlighted(false);
        softButton.setSystemAction(SystemAction.DEFAULT_ACTION);
        currentSoftButtons.add(softButton);

        final Button btnSoftButtons = (Button) layout.findViewById(R.id.updateturnlist_btnSoftButtons);
        btnSoftButtons.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                IntentHelper.addObjectForKey(currentSoftButtons, Const.INTENTHELPER_KEY_OBJECTSLIST);

                // TODO : Incapsulate this Intent creation into builder patternt of the
                // SoftButtonsListActivity
                Intent intent = new Intent(getActivity(), SoftButtonsListActivity.class);
                intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER, UPDATE_TURN_LIST_MAX_SOFT_BUTTONS);

                startActivityForResult(intent, PlaceholderFragment.REQUEST_LIST_SOFTBUTTONS);
            }
        });

        return new AlertDialog.Builder(context)
                .setPositiveButton(getActivity().getString(R.string.ok_string),
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                        /*
                         * the number of items to send is determined as max of turn items
                         * and icon items. only when the both fields are empty, we
                         * don't send anything.
                         */
                                final boolean turnListEnabled = useTurnList.isChecked();
                                final boolean iconListEnabled = useIconList.isChecked();
                                final String turnListString = txtTurnList.getText().toString();
                                final String iconListString = txtIconList.getText().toString();
                                if ((turnListString.length() > 0) || (iconListString.length() > 0)) {
                                    Vector<Turn> turnsArray = new Vector<Turn>();

                                    final String[] iconNames = iconListString.split(SyncProxyTester.JOIN_STRING);
                                    final String[] turnNames = turnListString.split(SyncProxyTester.JOIN_STRING);
                                    int turnCount = Math.max(iconNames.length, turnNames.length);

                                    Turn turn;
                                    Image turnImage;
                                    for (int i = 0; i < turnCount; ++i) {
                                        turn = new Turn();
                                        if (turnListEnabled) {
                                            if (i < turnNames.length) {
                                                turn.setNavigationText(turnNames[i]);
                                            }
                                        }

                                        if (iconListEnabled) {
                                            if (i < iconNames.length) {
                                                turnImage = new Image();
                                                turnImage.setValue(iconNames[i]);
                                                turnImage.setImageType(ImageType.DYNAMIC);
                                                turn.setTurnIcon(turnImage);
                                            }
                                        }
                                        turnsArray.add(turn);
                                    }
                                    final UpdateTurnList updateTurnList = new UpdateTurnList();
                                    updateTurnList.setCorrelationId(((SyncProxyTester) getActivity())
                                            .getNextCorrelationIdForCurrentFragment());
                                    updateTurnList.setTurnList(turnsArray);
                                    if (useSoftButtons.isChecked()) {
                                        updateTurnList.setSoftButtons(currentSoftButtons);
                                    }

                                    ((SyncProxyTester) getActivity()).onUpdateTurnListDialogResult(
                                            getArguments().getString(APP_ID_KEY), updateTurnList);

                                } else {
                                    SafeToast.showToastAnyThread("Both fields are empty, " +
                                            "nothing to send");
                                }
                            }
                        }
                )
                .setNegativeButton(getActivity().getString(R.string.cancel_string),
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                dialog.cancel();
                            }
                        }
                ).setView(layout).show();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        Logger.i(LOG_TAG + " OnActivityResult, request:" + requestCode + ", result:" + resultCode +
                ", data:" + data);

        switch (requestCode) {
            case PlaceholderFragment.REQUEST_LIST_SOFTBUTTONS:
                if (resultCode == Activity.RESULT_OK) {
                    currentSoftButtons.clear();

                    // TODO : Reconsider this approach and probably replace with custom class
                    // that describes a response abd implement Serializable

                    Vector<SoftButton> resultVector = (Vector<SoftButton>) IntentHelper.
                            getObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);

                    for (SoftButton softButton : resultVector) {
                        currentSoftButtons.add(softButton);
                    }

                }
                IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);
                break;
        }
    }
}