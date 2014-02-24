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
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Toast;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.SyncSubMenu;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.rpc.AddSubMenu;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/24/14
 * Time: 10:35 AM
 */
public class AddSubMenuDialog extends DialogFragment {

    private static final String LOG_TAG = "AddSubMenuDialog";

    private static int sSubMenuCmdID = 1000;

    public static AddSubMenuDialog newInstance() {
        AddSubMenuDialog addSubMenuDialog = new AddSubMenuDialog();
        return addSubMenuDialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.addsubmenu,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        final EditText editMenuName = (EditText) layout.findViewById(R.id.addsubmenu_menuName);
        final EditText editMenuID = (EditText) layout.findViewById(R.id.addsubmenu_menuID);
        final CheckBox chkUseMenuPos = (CheckBox) layout.findViewById(R.id.addsubmenu_useMenuPos);
        final EditText editMenuPos = (EditText) layout.findViewById(R.id.addsubmenu_menuPos);

        // set suggested value
        editMenuID.setText(String.valueOf(sSubMenuCmdID++));

        return new AlertDialog.Builder(mContext)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        String subMenuIDString = editMenuID.getText().toString();
                        int subMenuID = -1;
                        try {
                            subMenuID = Integer.parseInt(subMenuIDString);
                        } catch (NumberFormatException e) {
                            Toast.makeText(mContext, "Couldn't parse number " + subMenuIDString,
                                    Toast.LENGTH_LONG).show();
                            return;
                        }

                        int pos = -1;
                        if (chkUseMenuPos.isChecked()) {
                            String posString = editMenuPos.getText().toString();
                            try {
                                pos = Integer.parseInt(posString);
                            } catch (NumberFormatException e) {
                                Toast.makeText(mContext, "Couldn't parse number " + posString,
                                        Toast.LENGTH_LONG).show();
                                return;
                            }
                        }

                        AddSubMenu addSubMenu = RPCRequestFactory.buildAddSubMenu();
                        addSubMenu.setCorrelationID(((SyncProxyTester) getActivity())
                                .getCorrelationid());

                        SyncSubMenu subMenu = new SyncSubMenu();
                        subMenu.setName(editMenuName.getText().toString());
                        subMenu.setSubMenuId(subMenuID);
                        addSubMenu.setMenuID(subMenu.getSubMenuId());
                        addSubMenu.setMenuName(subMenu.getName());
                        if (chkUseMenuPos.isChecked()) {
                            addSubMenu.setPosition(pos);
                        }
                        ((SyncProxyTester) getActivity()).onAddSubMenuDialogResult(addSubMenu,
                                subMenu);
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                })
                .setView(layout).show();
    }
}