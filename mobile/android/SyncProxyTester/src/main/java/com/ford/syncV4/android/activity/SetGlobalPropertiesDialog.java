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
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Spinner;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.TTSChunkFactory;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.KeyboardProperties;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.VrHelpItem;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.KeyboardLayout;
import com.ford.syncV4.proxy.rpc.enums.KeypressMode;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.util.logger.Logger;

import java.util.Vector;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/24/14
 * Time: 11:44 AM
 */
public class SetGlobalPropertiesDialog extends BaseDialogFragment {

    private static final String LOG_TAG = SetGlobalPropertiesDialog.class.getSimpleName();

    /**
     * KeyboardProperties object passed between KeyboardPropertiesActivity and
     * this activity.
     */
    private KeyboardProperties mCurrentKbdProperties;

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.setglobalproperties,
                (ViewGroup) getActivity().findViewById(R.id.itemRoot));

        final EditText helpPrompt = (EditText) layout.findViewById(R.id.setglobalproperties_helpPrompt);
        final EditText timeoutPrompt = (EditText) layout.findViewById(R.id.setglobalproperties_timeoutPrompt);
        final EditText vrHelpTitle = (EditText) layout.findViewById(R.id.setglobalproperties_vrHelpTitle);
        final EditText vrHelpItemText = (EditText) layout.findViewById(R.id.setglobalproperties_vrHelpItemText);
        final CheckBox useVRHelpItemImage = (CheckBox) layout.findViewById(R.id.setglobalproperties_useVRHelpItemImage);
        final EditText vrHelpItemImage = (EditText) layout.findViewById(R.id.setglobalproperties_vrHelpItemImage);
        final EditText vrHelpItemPos = (EditText) layout.findViewById(R.id.setglobalproperties_vrHelpItemPos);
        final CheckBox choiceHelpPrompt = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceHelpPrompt);
        final CheckBox choiceTimeoutPrompt = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceTimeoutPrompt);
        final CheckBox choiceVRHelpTitle = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceVRHelpTitle);
        final CheckBox choiceVRHelpItem = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceVRHelpItem);
        final CheckBox choiceMenuTitle = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceMenuTitle);
        final EditText menuTitle = (EditText) layout.findViewById(R.id.setglobalproperties_menuTitle);
        final CheckBox choiceMenuIcon = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceMenuIcon);
        final EditText menuIcon = (EditText) layout.findViewById(R.id.setglobalproperties_menuIcon);
        final Spinner menuIconType = (Spinner) layout.findViewById(R.id.setglobalproperties_menuIconType);
        final CheckBox chkKbdProperties = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceKbdProperties);

        final ArrayAdapter<ImageType> imageTypeArrayAdapter =
                ((SyncProxyTester) getActivity()).getImageTypeAdapter();
        menuIconType.setAdapter(imageTypeArrayAdapter);
        menuIconType.setSelection(imageTypeArrayAdapter.getPosition(ImageType.DYNAMIC));

        mCurrentKbdProperties = new KeyboardProperties();
        mCurrentKbdProperties.setLanguage(Language.EN_US);
        mCurrentKbdProperties.setKeyboardLayout(KeyboardLayout.QWERTY);
        mCurrentKbdProperties.setKeypressMode(KeypressMode.SINGLE_KEYPRESS);
        mCurrentKbdProperties.setAutoCompleteText(getString(
                R.string.keyboardproperties_autoCompleteTextDefault));
        mCurrentKbdProperties.setLimitedCharacterList(new Vector<String>() {{
            add("a");
            add("b");
            add("c");
        }});

        Button btnKbdProperties = (Button) layout.findViewById(R.id.setglobalproperties_kbdProperties);
        btnKbdProperties.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        IntentHelper.addObjectForKey(
                                mCurrentKbdProperties,
                                Const.INTENTHELPER_KEY_KEYBOARDPROPERTIES);
                        Intent intent = new Intent(mContext, KeyboardPropertiesActivity.class);
                        startActivityForResult(intent, Const.REQUEST_EDIT_KBDPROPERTIES);
                    }
                });

        return new AlertDialog.Builder(mContext)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        SetGlobalProperties setGlobalProperties =
                                RPCRequestFactory.buildSetGlobalProperties();
                        int numberOfChoices = 0;

                        if (choiceHelpPrompt.isChecked()) {
                            Vector<TTSChunk> help = new Vector<TTSChunk>();
                            String helpString = helpPrompt.getText().toString();
                            for (String ttsChunk : helpString.split(SyncProxyTester.JOIN_STRING)) {
                                TTSChunk chunk = TTSChunkFactory.createChunk(
                                        SpeechCapabilities.TEXT, ttsChunk);
                                help.add(chunk);
                            }
                            setGlobalProperties.setHelpPrompt(help);
                            ++numberOfChoices;
                        }

                        if (choiceTimeoutPrompt.isChecked()) {
                            Vector<TTSChunk> timeout = new Vector<TTSChunk>();
                            String timeoutString = timeoutPrompt.getText().toString();
                            for (String ttsChunk : timeoutString.split(SyncProxyTester.JOIN_STRING)) {
                                TTSChunk chunk = TTSChunkFactory.createChunk(
                                        SpeechCapabilities.TEXT, ttsChunk);
                                timeout.add(chunk);
                            }
                            setGlobalProperties.setTimeoutPrompt(timeout);
                            ++numberOfChoices;
                        }

                        if (choiceVRHelpTitle.isChecked()) {
                            setGlobalProperties.setVrHelpTitle(vrHelpTitle.getText().toString());
                            ++numberOfChoices;
                        }

                        if (choiceVRHelpItem.isChecked()) {
                            Vector<VrHelpItem> vrHelpItems = new Vector<VrHelpItem>();
                            String[] itemTextArray = vrHelpItemText.getText().toString()
                                    .split(SyncProxyTester.JOIN_STRING);
                            String[] itemPosArray = vrHelpItemPos.getText().toString()
                                    .split(SyncProxyTester.JOIN_STRING);
                            String[] itemImageArray = vrHelpItemImage.getText().toString()
                                    .split(SyncProxyTester.JOIN_STRING);
                            int itemsCount = Math.min(itemTextArray.length,
                                    Math.min(itemPosArray.length, itemImageArray.length));

                            for (int i = 0; i < itemsCount; ++i) {
                                VrHelpItem item = new VrHelpItem();
                                item.setText(itemTextArray[i]);

                                try {
                                    item.setPosition(Integer.parseInt(itemPosArray[i]));
                                } catch (NumberFormatException e) {
                                    // set default position
                                    item.setPosition(1);
                                }

                                if (useVRHelpItemImage.isChecked()) {
                                    Image image = new Image();
                                    image.setValue(itemImageArray[i]);
                                    image.setImageType(ImageType.DYNAMIC);
                                    item.setImage(image);
                                }

                                vrHelpItems.add(item);
                            }

                            setGlobalProperties.setVrHelp(vrHelpItems);
                            ++numberOfChoices;
                        }

                        if (choiceMenuTitle.isChecked()) {
                            String title = menuTitle.getText().toString();
                            setGlobalProperties.setMenuTitle(title);
                            ++numberOfChoices;
                        }

                        if (choiceMenuIcon.isChecked()) {
                            Image image = new Image();
                            image.setValue(menuIcon.getText().toString());
                            image.setImageType(imageTypeArrayAdapter.getItem(menuIconType.getSelectedItemPosition()));
                            setGlobalProperties.setMenuIcon(image);
                            ++numberOfChoices;
                        }

                        if (chkKbdProperties.isChecked() && (mCurrentKbdProperties != null)) {
                            setGlobalProperties.setKeyboardProperties(mCurrentKbdProperties);
                            ++numberOfChoices;
                        }

                        if (numberOfChoices > 0) {
                            setGlobalProperties.setCorrelationId(((SyncProxyTester) getActivity())
                                    .getNextCorrelationIdForCurrentFragment());

                            ((SyncProxyTester) getActivity())
                                    .onSetGlobalPropertiesDialogResult(
                                            getArguments().getString(APP_ID_KEY),
                                            setGlobalProperties);

                            mCurrentKbdProperties = null;
                        } else {
                            SafeToast.showToastAnyThread("No items selected");
                        }
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        mCurrentKbdProperties = null;
                        dialog.dismiss();
                    }
                })
                .setView(layout).show();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        Logger.i(LOG_TAG + " OnActivityResult, request:" + requestCode + ", result:" + resultCode +
                ", data:" + data);

        if (resultCode != Activity.RESULT_OK) {
            return;
        }

        if (requestCode == Const.REQUEST_EDIT_KBDPROPERTIES) {
            if (IntentHelper.containsKey(Const.INTENTHELPER_KEY_KEYBOARDPROPERTIES)) {
                mCurrentKbdProperties = (KeyboardProperties) IntentHelper.getObjectForKey(
                        Const.INTENTHELPER_KEY_KEYBOARDPROPERTIES);
                if (mCurrentKbdProperties == null) {
                    Logger.w(LOG_TAG + " Returned kbdProperties is null");
                }
                IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_KEYBOARDPROPERTIES);
            }
            if (IntentHelper.containsKey(Const.INTENTHELPER_KEY_KEYBOARDPROPERTIES_EMPTY)) {
                mCurrentKbdProperties = new KeyboardProperties();
                Logger.w(LOG_TAG + " Returned kbdProperties is null, probably none of the properties " +
                        "were selected");
                IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_KEYBOARDPROPERTIES_EMPTY);
            }
        }
    }
}