package com.ford.syncV4.android.activity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Spinner;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.utils.StringUtils;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.proxy.rpc.KeyboardProperties;
import com.ford.syncV4.proxy.rpc.enums.KeyboardLayout;
import com.ford.syncV4.proxy.rpc.enums.KeypressMode;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.util.logger.Logger;

import java.util.Arrays;
import java.util.Vector;

public class KeyboardPropertiesActivity extends Activity {

    private static final String LOG_TAG = KeyboardPropertiesActivity.class.getSimpleName();

    //
    private KeyboardProperties kbdProp;
    //
    private CheckBox languageCheck;
    private Spinner languageSpinner;
    private CheckBox kbdLayoutCheck;
    private Spinner kbdLayoutSpinner;
    private CheckBox keypressModeCheck;
    private Spinner keypressModeSpinner;
    //private CheckBox sendDEntryCheck;
    //private CheckBox sendDEntry;
    private CheckBox charListCheck;
    private EditText charList;
    private CheckBox autocompleteTextCheck;
    private EditText autocompleteText;
    //
    private ArrayAdapter<Language> languageAdapter;
    private ArrayAdapter<KeyboardLayout> kbdLayoutAdapter;
    private ArrayAdapter<KeypressMode> keypressModeAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_keyboardproperties);

        setupUI();

        languageAdapter = new ArrayAdapter<Language>(this, android.R.layout.simple_spinner_item,
                Language.values());
        languageAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        languageSpinner.setAdapter(languageAdapter);

        kbdLayoutAdapter = new ArrayAdapter<KeyboardLayout>(this,
                android.R.layout.simple_spinner_item, KeyboardLayout.values());
        kbdLayoutAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        kbdLayoutSpinner.setAdapter(kbdLayoutAdapter);

        keypressModeAdapter = new ArrayAdapter<KeypressMode>(this,
                android.R.layout.simple_spinner_item, KeypressMode.values());
        keypressModeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        keypressModeSpinner.setAdapter(keypressModeAdapter);

        kbdProp = (KeyboardProperties) IntentHelper
                .getObjectForKey(Const.INTENTHELPER_KEY_KEYBOARDPROPERTIES);
        if (kbdProp == null) {
            kbdProp = new KeyboardProperties();
        }

        fillUI(kbdProp);
    }

    private void fillUI(KeyboardProperties kbdProperties) {
        assert kbdProperties != null;

        Language language = kbdProperties.getLanguage();
        languageCheck.setChecked(language != null);
        if (language != null) {
            languageSpinner.setSelection(languageAdapter.getPosition(language));
        }

        KeyboardLayout kbdLayout = kbdProperties.getKeyboardLayout();
        kbdLayoutCheck.setChecked(kbdLayout != null);
        if (kbdLayout != null) {
            kbdLayoutSpinner.setSelection(kbdLayoutAdapter.getPosition(kbdLayout));
        }

        KeypressMode keypressMode = kbdProperties.getKeypressMode();
        keypressModeCheck.setChecked(keypressMode != null);
        if (keypressMode != null) {
            keypressModeSpinner.setSelection(keypressModeAdapter.getPosition(keypressMode));
        }

        Vector<String> charListValue = kbdProperties.getLimitedCharacterList();
        charListCheck.setChecked(charListValue != null);
        if (charListValue != null) {
            charList.setText(StringUtils.joinStrings(charListValue));
        }

        String autocompleteTextValue = kbdProperties.getAutoCompleteText();
        autocompleteTextCheck.setChecked(autocompleteTextValue != null);
        if (autocompleteTextValue != null) {
            autocompleteText.setText(autocompleteTextValue);
        }
    }

    private void setupUI() {
        languageCheck = (CheckBox) findViewById(R.id.keyboardproperties_useLanguage);
        languageSpinner = (Spinner) findViewById(R.id.keyboardproperties_language);
        kbdLayoutCheck = (CheckBox) findViewById(R.id.keyboardproperties_useKbdLayout);
        kbdLayoutSpinner = (Spinner) findViewById(R.id.keyboardproperties_kbdLayout);
        keypressModeCheck = (CheckBox) findViewById(R.id.keyboardproperties_useKeypressMode);
        keypressModeSpinner = (Spinner) findViewById(R.id.keyboardproperties_keypressMode);
        charListCheck = (CheckBox) findViewById(R.id.keyboardproperties_useLimitedCharacterList);
        charList = (EditText) findViewById(R.id.keyboardproperties_limitedCharacterList);
        autocompleteTextCheck = (CheckBox) findViewById(
                R.id.keyboardproperties_useAutoCompleteText);
        autocompleteText = (EditText) findViewById(R.id.keyboardproperties_autoCompleteText);

        Button btnOk = ((Button) findViewById(R.id.keyboardproperties_ok));
        btnOk.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String key = Const.INTENTHELPER_KEY_KEYBOARDPROPERTIES;
                KeyboardProperties keyboardProperties = getKbdProperties();
                if (keyboardProperties == null) {
                    keyboardProperties = new KeyboardProperties();
                    key = Const.INTENTHELPER_KEY_KEYBOARDPROPERTIES_EMPTY;
                }
                IntentHelper.addObjectForKey(keyboardProperties, key);
                setResult(RESULT_OK);
                finish();
            }
        });

        Button btnCancel = ((Button) findViewById(R.id.keyboardproperties_cancel));
        btnCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                setResult(RESULT_CANCELED);
                finish();
            }
        });
    }

    /**
     * @return {@link com.ford.syncV4.proxy.rpc.KeyboardProperties} object filled with selected
     * values, or <b>null</b> if none of the properties selected
     */
    private KeyboardProperties getKbdProperties() {

        boolean isLanguageCheck = languageCheck.isChecked();
        boolean isKbdLayoutCheck = kbdLayoutCheck.isChecked();
        boolean isKeyPressModeCheck = keypressModeCheck.isChecked();
        boolean isCharListCheck = charListCheck.isChecked();
        boolean isAutoCompleteTextCheck = autocompleteTextCheck.isChecked();

        if (!isLanguageCheck && !isKbdLayoutCheck && !isKeyPressModeCheck && !isCharListCheck &&
                !isAutoCompleteTextCheck) {
            return null;
        }

        KeyboardProperties keyboardProperties = new KeyboardProperties();

        if (isLanguageCheck) {
            Language language = languageAdapter.getItem(languageSpinner.getSelectedItemPosition());
            keyboardProperties.setLanguage(language);
        }

        if (isKbdLayoutCheck) {
            KeyboardLayout kbdLayout = kbdLayoutAdapter
                    .getItem(kbdLayoutSpinner.getSelectedItemPosition());
            keyboardProperties.setKeyboardLayout(kbdLayout);
        }

        if (isKeyPressModeCheck) {
            KeypressMode keypressMode = keypressModeAdapter
                    .getItem(keypressModeSpinner.getSelectedItemPosition());
            keyboardProperties.setKeypressMode(keypressMode);
        }

        if (isCharListCheck) {
            Vector<String> charListValue = new Vector<String>(Arrays.asList(
                    charList.getText().toString().split(StringUtils.DEFAULT_JOIN_STRING)));
            keyboardProperties.setLimitedCharacterList(charListValue);
        }

        if (isAutoCompleteTextCheck) {
            String autocompleteTextValue = autocompleteText.getText().toString();
            keyboardProperties.setAutoCompleteText(autocompleteTextValue);
        }

        return keyboardProperties;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        Logger.i(LOG_TAG + " OnActivityResult, request:" + requestCode + ", result:" + resultCode +
                ", data:" + data);
    }
}