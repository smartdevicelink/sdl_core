package com.ford.syncV4.android.activity;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Spinner;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.utils.StringUtils;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.enums.ImageType;

import java.util.Arrays;
import java.util.Vector;

public class ChoiceEditActivity extends Activity {
    private final static String LOG_TAG =
            ChoiceEditActivity.class.getSimpleName();
    //
    private Choice choice = null;
    //
    private CheckBox useChoiceID;
    private EditText choiceID;
    private CheckBox useMenuName;
    private EditText menuName;
    private CheckBox useVRCommands;
    private EditText vrCommands;
    private CheckBox useImage;
    private EditText image;
    private Spinner imageType;
    private CheckBox useSecondaryText;
    private EditText secondaryText;
    private CheckBox useTertiaryText;
    private EditText tertiaryText;
    private CheckBox useSecondaryImage;
    private EditText secondaryImage;
    private Spinner secondaryImageType;
    //
    private ArrayAdapter<ImageType> imageTypeAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.choice);

        setupUI();

        choice = (Choice) IntentHelper.getObjectForKey(
                Const.INTENTHELPER_KEY_OBJECT);

        fillUI(choice);
    }

    private void fillUI(Choice choice) {
        assert choice != null;

        final Integer choiceIDValue = choice.getChoiceID();
        useChoiceID.setChecked(choiceIDValue != null);
        if (choiceIDValue != null) {
            choiceID.setText(String.valueOf(choiceIDValue));
        }

        final String menuNameValue = choice.getMenuName();
        useMenuName.setChecked(menuNameValue != null);
        if (menuNameValue != null) {
            menuName.setText(menuNameValue);
        }

        final Vector<String> vrCommandsValue = choice.getVrCommands();
        useVRCommands.setChecked(vrCommandsValue != null);
        if (vrCommandsValue != null) {
            vrCommands.setText(StringUtils.joinStrings(vrCommandsValue));
        }

        final Image imageValue = choice.getImage();
        useImage.setChecked(imageValue != null);
        if (imageValue != null) {
            final String imageValueValue = imageValue.getValue();
            if (imageValueValue != null) {
                image.setText(imageValueValue);
            }

            final ImageType imageValueType = imageValue.getImageType();
            if (imageValueType != null) {
                imageType.setSelection(
                        imageTypeAdapter.getPosition(imageValueType));
            }
        }

        final String secondaryTextValue = choice.getSecondaryText();
        useSecondaryText.setChecked(secondaryTextValue != null);
        if (secondaryTextValue != null) {
            secondaryText.setText(secondaryTextValue);
        }

        final String tertiaryTextValue = choice.getTertiaryText();
        useTertiaryText.setChecked(tertiaryTextValue != null);
        if (tertiaryTextValue != null) {
            tertiaryText.setText(tertiaryTextValue);
        }

        final Image secondaryImageValue = choice.getSecondaryImage();
        useSecondaryImage.setChecked(secondaryImageValue != null);
        if (secondaryImageValue != null) {
            final String imageValueValue = secondaryImageValue.getValue();
            if (imageValueValue != null) {
                secondaryImage.setText(imageValueValue);
            }

            final ImageType imageValueType = secondaryImageValue.getImageType();
            if (imageValueType != null) {
                secondaryImageType.setSelection(
                        imageTypeAdapter.getPosition(imageValueType));
            }
        }
    }

    private void setupUI() {
        useChoiceID = (CheckBox) findViewById(R.id.choice_useChoiceID);
        choiceID = (EditText) findViewById(R.id.choice_choiceID);
        useMenuName = (CheckBox) findViewById(R.id.choice_useMenuName);
        menuName = (EditText) findViewById(R.id.choice_menuName);
        useVRCommands = (CheckBox) findViewById(R.id.choice_useVRCommands);
        vrCommands = (EditText) findViewById(R.id.choice_vrCommands);
        useImage = (CheckBox) findViewById(R.id.choice_useImage);
        image = (EditText) findViewById(R.id.choice_image);
        imageType = (Spinner) findViewById(R.id.choice_imageType);
        useSecondaryText =
                (CheckBox) findViewById(R.id.choice_useSecondaryText);
        secondaryText = (EditText) findViewById(R.id.choice_secondaryText);
        useTertiaryText = (CheckBox) findViewById(R.id.choice_useTertiaryText);
        tertiaryText = (EditText) findViewById(R.id.choice_tertiaryText);
        useSecondaryImage =
                (CheckBox) findViewById(R.id.choice_useSecondaryImage);
        secondaryImage = (EditText) findViewById(R.id.choice_secondaryImage);
        secondaryImageType =
                (Spinner) findViewById(R.id.choice_secondaryImageType);

        Button btnOk = ((Button) findViewById(R.id.choice_ok));
        btnOk.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                Choice result = new Choice();
                fillChoiceFromUI(result);

                IntentHelper.addObjectForKey(result,
                        Const.INTENTHELPER_KEY_OBJECT);
                setResult(RESULT_OK);
                finish();
            }
        });

        Button btnCancel = ((Button) findViewById(R.id.choice_cancel));
        btnCancel.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                setResult(RESULT_CANCELED);
                finish();
            }
        });

        imageTypeAdapter = new ArrayAdapter<ImageType>(this,
                android.R.layout.simple_spinner_item, ImageType.values());
        imageTypeAdapter.setDropDownViewResource(
                android.R.layout.simple_spinner_dropdown_item);

        imageType.setAdapter(imageTypeAdapter);
        secondaryImageType.setAdapter(imageTypeAdapter);
    }

    private void fillChoiceFromUI(Choice result) {
        assert result != null;

        if (useChoiceID.isChecked()) {
            try {
                result.setChoiceID(
                        Integer.parseInt(choiceID.getText().toString()));
            } catch (NumberFormatException e) {
                result.setChoiceID(4242);
            }
        }

        if (useMenuName.isChecked()) {
            result.setMenuName(menuName.getText().toString());
        }

        if (useVRCommands.isChecked()) {
            result.setVrCommands(new Vector<String>(Arrays.asList(
                    vrCommands.getText()
                              .toString()
                              .split(StringUtils.DEFAULT_JOIN_STRING))));
        }

        if (useImage.isChecked()) {
            Image imageValue = new Image();
            imageValue.setValue(image.getText().toString());
            imageValue.setImageType(imageTypeAdapter.getItem(
                    imageType.getSelectedItemPosition()));
            result.setImage(imageValue);
        }

        if (useSecondaryText.isChecked()) {
            result.setSecondaryText(secondaryText.getText().toString());
        }

        if (useTertiaryText.isChecked()) {
            result.setTertiaryText(tertiaryText.getText().toString());
        }

        if (useSecondaryImage.isChecked()) {
            Image imageValue = new Image();
            imageValue.setValue(secondaryImage.getText().toString());
            imageValue.setImageType(imageTypeAdapter.getItem(
                    secondaryImageType.getSelectedItemPosition()));
            result.setSecondaryImage(imageValue);
        }
    }
}
