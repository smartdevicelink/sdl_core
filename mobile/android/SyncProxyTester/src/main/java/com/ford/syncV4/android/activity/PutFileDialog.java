package com.ford.syncV4.android.activity;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Spinner;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.lamerman.FileDialog;
import com.lamerman.SelectionMode;

import java.io.File;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/30/14
 * Time: 2:16 PM
 */
public class PutFileDialog extends DialogFragment {

    public interface PutFileDialogListener {
        public void onPutFileSelected(String fileName);
    }

    private static final String LOG_TAG = "PutFileDialog";
    private static final String CORRELATION_ID_KEY = "CorrelationId";
    private static final int PUTFILE_MAXFILESIZE = 4 * 1024 * 1024; // 4MB
    private EditText mSelectedFileNameView;

    public static PutFileDialog newInstance(int commandCorrelationId) {
        PutFileDialog putFileDialog = new PutFileDialog();
        Bundle bundle = new Bundle();
        bundle.putInt(CORRELATION_ID_KEY, commandCorrelationId);
        putFileDialog.setArguments(bundle);
        return putFileDialog;
    }

    // Use this instance of the interface to deliver action events
    PutFileDialogListener mListener;

    // Override the Fragment.onAttach() method to instantiate the NoticeDialogListener
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        // Verify that the host activity implements the callback interface
        try {
            // Instantiate the NoticeDialogListener so we can send events to the host
            mListener = (PutFileDialogListener) activity;
        } catch (ClassCastException e) {
            // The activity doesn't implement the interface, throw exception
            throw new ClassCastException(activity.toString() + " must implement NoticeDialogListener");
        }
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.putfile, null);

        // Just for the tests we assume that getArguments have CORRELATION_ID_KEY
        final int mCorrelationid = getArguments().getInt(CORRELATION_ID_KEY);

        final Button btnSelectLocalFile = (Button) layout.findViewById(R.id.putfile_selectFileButton);
        btnSelectLocalFile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // show Choose File dialog
                Intent intent = new Intent(mContext, FileDialog.class);
                intent.putExtra(FileDialog.START_PATH, "/sdcard");
                intent.putExtra(FileDialog.CAN_SELECT_DIR, false);
                intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
                startActivityForResult(intent, Const.REQUEST_FILE_OPEN);
            }
        });

        final EditText txtSyncFileName = (EditText) layout.findViewById(R.id.syncFileName);

        final Spinner spnFileType = (Spinner) layout.findViewById(R.id.spnFileType);
        ArrayAdapter<FileType> spinnerAdapter = new ArrayAdapter<FileType>(mContext,
                android.R.layout.simple_spinner_item, FileType.values());
        spinnerAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spnFileType.setAdapter(spinnerAdapter);

        final CheckBox chkPersistentFile = (CheckBox) layout.findViewById(R.id.chkPersistentFile);
        final CheckBox chkSystemFile = (CheckBox) layout.findViewById(R.id.putfile_chkSystemFile);
        mSelectedFileNameView = (EditText) layout.findViewById(R.id.putfile_localFileName);

        final CheckBox chkOffset = (CheckBox) layout.findViewById(R.id.putfile_useOffset);
        final EditText txtOffset = (EditText) layout.findViewById(R.id.putfile_offset);
        final CheckBox chkLength = (CheckBox) layout.findViewById(R.id.putfile_useLength);
        final EditText txtLength = (EditText) layout.findViewById(R.id.putfile_length);
        final CheckBox doEncrypt = (CheckBox) layout.findViewById(R.id.putfile_do_encrypt_view);

        // Use the Builder class for convenient dialog construction
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder = new AlertDialog.Builder(mContext);
        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                String filename = mSelectedFileNameView.getText().toString();
                long fileSize = new File(filename).length();
                if (fileSize <= PUTFILE_MAXFILESIZE) {
                    byte[] data = AppUtils.contentsOfResource(filename);
                    if (data != null) {
                        String syncFileName = txtSyncFileName.getText().toString();

                        mListener.onPutFileSelected(syncFileName);

                        if (MainApp.getInstance().getBoundProxyService() != null) {
                            Integer offset = null;
                            if (chkLength.isChecked()) {
                                try {
                                    offset = Integer.valueOf(txtOffset.getText().toString());
                                } catch (NumberFormatException e) {
                                    offset = 0;
                                    SafeToast.showToastAnyThread("Can't convert offset to integer");
                                }
                            }

                            Integer length = null;
                            if (chkLength.isChecked()) {
                                try {
                                    length = Integer.valueOf(txtLength.getText().toString());
                                } catch (NumberFormatException e) {
                                    length = 0;
                                    SafeToast.showToastAnyThread("Can't convert length to integer");
                                }
                            }

                            MainApp.getInstance().getBoundProxyService().commandPutFile(
                                    (FileType) spnFileType.getSelectedItem(), syncFileName, data,
                                    mCorrelationid, chkPersistentFile.isChecked(),
                                    chkSystemFile.isChecked(), length, offset, null, doEncrypt.isChecked());
                        }
                    } else {
                        SafeToast.showToastAnyThread("Can't read data from file");
                    }
                } else {
                    SafeToast.showToastAnyThread("The size of the file exceeds the limit of " +
                            (PUTFILE_MAXFILESIZE / (1024 * 1024)) + " MB");
                }
            }
        });
        builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                dialog.cancel();
            }
        });
        builder.setView(layout);
        return builder.create();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode == Activity.RESULT_OK) {
            if (requestCode == Const.REQUEST_FILE_OPEN) {
                String fileName = data.getStringExtra(FileDialog.RESULT_PATH);
                if (mSelectedFileNameView != null) {
                    mSelectedFileNameView.setText(fileName);
                }
            }
        }
    }
}