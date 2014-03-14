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
import com.ford.syncV4.proxy.TTSChunkFactory;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThru;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.enums.AudioType;
import com.ford.syncV4.proxy.rpc.enums.BitsPerSample;
import com.ford.syncV4.proxy.rpc.enums.SamplingRate;

import java.util.Vector;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 14.03.14
 * Time: 10:51
 */
public class PerformAudioPassThruDialog extends DialogFragment {

    private static final String LOG_TAG = "PerformAudioPassThruDialog";

    public static PerformAudioPassThruDialog newInstance() {
        PerformAudioPassThruDialog performAudioPassThruDialog = new PerformAudioPassThruDialog();
        return performAudioPassThruDialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.performaudiopassthru,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        final EditText txtInitialPrompt = (EditText) layout
                .findViewById(R.id.performaudiopassthru_txtInitialPrompt);
        final EditText txtAudioPassThruDisplayText1 = (EditText) layout
                .findViewById(R.id.performaudiopassthru_txtAudioPassThruDisplayText1);
        final EditText txtAudioPassThruDisplayText2 = (EditText) layout
                .findViewById(R.id.performaudiopassthru_txtAudioPassThruDisplayText2);
        final Spinner spnSamplingRate = (Spinner) layout
                .findViewById(R.id.performaudiopassthru_spnSamplingRate);
        final EditText txtMaxDuration = (EditText) layout
                .findViewById(R.id.performaudiopassthru_txtMaxDuration);
        final Spinner spnBitsPerSample = (Spinner) layout
                .findViewById(R.id.performaudiopassthru_spnBitsPerSample);
        final Spinner spnAudioType = (Spinner) layout
                .findViewById(R.id.performaudiopassthru_spnAudioType);
        final CheckBox chkMuteAudio = (CheckBox) layout
                .findViewById(R.id.performaudiopassthru_muteAudio);

        ArrayAdapter<String> adapter = ((SyncProxyTester) getActivity()).getFunctionsAdapter();

        ArrayAdapter<SamplingRate> spinnerAdapterSamplingRate = new ArrayAdapter<SamplingRate>(
                adapter.getContext(),
                android.R.layout.simple_spinner_item, SamplingRate.values());
        spinnerAdapterSamplingRate
                .setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spnSamplingRate.setAdapter(spinnerAdapterSamplingRate);

        ArrayAdapter<BitsPerSample> spinnerAdapterBitsPerSample = new ArrayAdapter<BitsPerSample>(
                adapter.getContext(),
                android.R.layout.simple_spinner_item, BitsPerSample.values());
        spinnerAdapterBitsPerSample
                .setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spnBitsPerSample.setAdapter(spinnerAdapterBitsPerSample);

        ArrayAdapter<AudioType> spinnerAdapterAudioType = new ArrayAdapter<AudioType>(
                adapter.getContext(),
                android.R.layout.simple_spinner_item, AudioType.values());
        spinnerAdapterAudioType.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spnAudioType.setAdapter(spinnerAdapterAudioType);

        return new AlertDialog.Builder(mContext)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        Vector<TTSChunk> initChunks = TTSChunkFactory
                                .createSimpleTTSChunks(txtInitialPrompt.getText().toString());
                        try {
                            PerformAudioPassThru performAudioPassThru = new PerformAudioPassThru();
                            performAudioPassThru.setInitialPrompt(initChunks);
                            performAudioPassThru.setAudioPassThruDisplayText1(txtAudioPassThruDisplayText1.getText().toString());
                            performAudioPassThru.setAudioPassThruDisplayText2(txtAudioPassThruDisplayText2.getText().toString());
                            performAudioPassThru.setSamplingRate((SamplingRate) spnSamplingRate.getSelectedItem());
                            performAudioPassThru.setMaxDuration(Integer.parseInt(txtMaxDuration.getText().toString()));
                            performAudioPassThru.setBitsPerSample((BitsPerSample) spnBitsPerSample.getSelectedItem());
                            performAudioPassThru.setAudioType((AudioType) spnAudioType.getSelectedItem());
                            performAudioPassThru.setMuteAudio(chkMuteAudio.isChecked());
                            performAudioPassThru.setCorrelationID(((SyncProxyTester) getActivity())
                                    .getCorrelationid());

                            ((SyncProxyTester) getActivity())
                                    .onPerformAudioPassThruDialogResult(performAudioPassThru);
                        } catch (NumberFormatException e) {
                            SafeToast.showToastAnyThread("Couldn't parse number");
                        }
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