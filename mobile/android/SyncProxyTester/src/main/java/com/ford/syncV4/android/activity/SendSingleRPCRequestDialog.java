package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.TextView;

import com.ford.syncV4.android.R;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.constants.Names;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 24.03.14
 * Time: 16:47
 */
public class SendSingleRPCRequestDialog extends DialogFragment {

    private static final String LOG_TAG = "SendSingleRPCRequestDialog";

    private RPCRequest mRpcRequest;

    public static SendSingleRPCRequestDialog newInstance() {
        SendSingleRPCRequestDialog sendSingleRPCRequestDialog = new SendSingleRPCRequestDialog();
        return sendSingleRPCRequestDialog;
    }

    public void setCommand(RPCRequest command) {
        mRpcRequest = command;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.send_single_rpc_request_dialog_view,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        TextView commandNameView = (TextView) layout.findViewById(R.id.single_rpc_request_name_view);
        final CheckBox commandEncode = (CheckBox) layout.findViewById(R.id.single_rpc_request_encoded_view);
        final CheckBox commandCloseUSBConnection = (CheckBox) layout.findViewById(
                R.id.single_rpc_request_usb_connection_view);

        if (mRpcRequest != null) {
            commandNameView.setText(mRpcRequest.getFunctionName());

            if (mRpcRequest.getFunctionName().equals(Names.UnregisterAppInterface)) {
                commandCloseUSBConnection.setVisibility(View.VISIBLE);
            }

        } else {
            commandNameView.setTextColor(Color.RED);
            commandNameView.setText("Command is NULL");
        }

        return new AlertDialog.Builder(mContext)
                .setTitle("Set up command")
                .setPositiveButton("Send", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        if (mRpcRequest == null) {
                            return;
                        }
                        ((SyncProxyTester) getActivity()).onSingleRPCCommandDialogResult(
                                mRpcRequest, commandEncode.isChecked(),
                                commandCloseUSBConnection.isChecked());
                    }
                })
                .setView(layout).show();
    }
}