package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Spinner;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.SyncSubMenu;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.rpc.AddCommand;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.MenuParams;
import com.ford.syncV4.proxy.rpc.enums.ImageType;

import java.util.Arrays;
import java.util.Vector;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/21/14
 * Time: 10:09 AM
 */
public class AddCommandDialog extends DialogFragment {

    private static final String LOG_TAG = "AddCommandDialog";

    private static int sItemCmdId = 1;

    public static AddCommandDialog newInstance() {
        AddCommandDialog sendAddCommandDialog = new AddCommandDialog();
        return sendAddCommandDialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.addcommand,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        final EditText editCmdID = (EditText) layout.findViewById(R.id.addcommand_commandID);
        final CheckBox chkUseCommandName = (CheckBox) layout.findViewById(R.id.addcommand_useCommandName);
        final EditText er = (EditText) layout.findViewById(R.id.addcommand_commandName);
        final CheckBox chkUseVrSynonyms = (CheckBox) layout.findViewById(R.id.addcommand_useVRSynonyms);
        final EditText editVrSynonyms = (EditText) layout.findViewById(R.id.addcommand_vrSynonym);
        final CheckBox chkUseMenuParams = (CheckBox) layout.findViewById(R.id.addcommand_useMenuParams);
        final CheckBox chkUseParentID = (CheckBox) layout.findViewById(R.id.addcommand_useParentID);
        final Spinner s = (Spinner) layout.findViewById(R.id.addcommand_availableSubmenus);
        s.setAdapter(((SyncProxyTester) getActivity()).getSubMenuAdapter());
        final CheckBox chkUseMenuPos = (CheckBox) layout.findViewById(R.id.addcommand_useMenuPos);
        final EditText editMenuPos = (EditText) layout.findViewById(R.id.addcommand_menuPos);
        final CheckBox chkUseIcon = (CheckBox) layout.findViewById(R.id.addcommand_useIcon);
        final EditText editIconValue = (EditText) layout.findViewById(R.id.addcommand_iconValue);
        final Spinner spnIconType = (Spinner) layout.findViewById(R.id.addcommand_iconType);

        // set suggested value
        editCmdID.setText(String.valueOf(sItemCmdId++));

        ArrayAdapter<ImageType> imageTypeArrayAdapter =
                ((SyncProxyTester) getActivity()).getImageTypeAdapter();
        spnIconType.setAdapter(imageTypeArrayAdapter);
        spnIconType.setSelection(imageTypeArrayAdapter.getPosition(ImageType.DYNAMIC));

        return new AlertDialog.Builder(mContext)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        String cmdIDString = editCmdID.getText().toString();
                        int cmdID;
                        try {
                            cmdID = Integer.parseInt(cmdIDString);
                        } catch (NumberFormatException e) {
                            SafeToast.showToastAnyThread("Couldn't parse number " + cmdIDString);
                            return;
                        }

                        int pos = -1;
                        if (chkUseMenuPos.isChecked()) {
                            String posString = editMenuPos.getText().toString();
                            try {
                                pos = Integer.parseInt(posString);
                            } catch (NumberFormatException e) {
                                SafeToast.showToastAnyThread("Couldn't parse number " + posString);
                                return;
                            }
                        }

                        AddCommand addCommand = RPCRequestFactory.buildAddCommand();
                        addCommand.setCorrelationID(((SyncProxyTester) getActivity())
                                .getCorrelationid());
                        addCommand.setCmdID(cmdID);

                        if (chkUseMenuParams.isChecked()) {
                            MenuParams menuParams = new MenuParams();
                            if (chkUseCommandName.isChecked()) {
                                String menuNameValue = er.getText().toString();
                                //Log.d(LOG_TAG, "AddCommand MenuName:" + menuNameValue);
                                menuParams.setMenuName(menuNameValue);
                            }
                            if (chkUseMenuPos.isChecked()) {
                                menuParams.setPosition(pos);
                            }
                            if (chkUseParentID.isChecked()) {
                                SyncSubMenu sm = (SyncSubMenu) s.getSelectedItem();
                                if (sm != null) {
                                    menuParams.setParentID(sm.getSubMenuId());
                                }
                            }
                            addCommand.setMenuParams(menuParams);
                        }

                        if (chkUseVrSynonyms.isChecked()) {
                            addCommand.setVrCommands(new Vector<String>(
                                    Arrays.asList(editVrSynonyms.getText()
                                            .toString().split(SyncProxyTester.JOIN_STRING))));
                        }

                        if (chkUseIcon.isChecked()) {
                            Image icon = new Image();
                            icon.setValue(editIconValue.getText().toString());
                            icon.setImageType((ImageType) spnIconType.getSelectedItem());
                            addCommand.setCmdIcon(icon);
                        }

                        ((SyncProxyTester) getActivity()).onAddCommandDialogResult(addCommand);
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                }).setView(layout).show();
    }
}