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
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;

public class SoftButtonEditActivity extends Activity {
	private final static String LOG_TAG = SoftButtonEditActivity.class
			.getSimpleName();

	private SoftButton softButton = null;

	private EditText editId;
	private EditText editText;
	private EditText editImage;
	private CheckBox checkBoxHighlighted;
	private Spinner spinnerType;
	private Spinner spinnerImageType;
	private Spinner spinnerSystemAction;

	private ArrayAdapter<SoftButtonType> typeAdapter;
	private ArrayAdapter<ImageType> imageTypeAdapter;
	private ArrayAdapter<SystemAction> systemActionAdapter;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.softbutton);

		softButton = (SoftButton) IntentHelper
				.getObjectForKey(Const.INTENTHELPER_KEY_SOFTBUTTON);

		editId = (EditText) findViewById(R.id.softbutton_id);
		editText = (EditText) findViewById(R.id.softbutton_text);
		editImage = (EditText) findViewById(R.id.softbutton_image);
		checkBoxHighlighted = (CheckBox) findViewById(R.id.softbutton_isHighlighted);
		spinnerType = (Spinner) findViewById(R.id.softbutton_type);
		spinnerImageType = (Spinner) findViewById(R.id.softbutton_imageType);
		spinnerSystemAction = (Spinner) findViewById(R.id.softbutton_systemAction);

		Button btnOk = ((Button) findViewById(R.id.softbutton_ok));
		btnOk.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				SoftButton result = new SoftButton();
				result.setType((SoftButtonType) spinnerType.getSelectedItem());
				switch (result.getType()) {
				case SBT_TEXT:
					setTextToSoftButton(result);
					break;
				case SBT_IMAGE:
					setImageToSoftButton(result);
					break;
				case SBT_BOTH:
					setTextToSoftButton(result);
					setImageToSoftButton(result);
					break;
				}
				result.setIsHighlighted(checkBoxHighlighted.isChecked());
				result.setSystemAction((SystemAction) spinnerSystemAction
						.getSelectedItem());
				try {
					result.setSoftButtonID(Integer.parseInt(editId.getText()
							.toString()));
				} catch (NumberFormatException e) {
					result.setSoftButtonID(5555);
				}

				IntentHelper.addObjectForKey(result,
						Const.INTENTHELPER_KEY_SOFTBUTTON);
				setResult(RESULT_OK);
				finish();
			}
		});

		Button btnCancel = ((Button) findViewById(R.id.softbutton_cancel));
		btnCancel.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				setResult(RESULT_CANCELED);
				finish();
			}
		});

		editId.setText(String.valueOf(softButton.getSoftButtonID()));

		// setup adapters
		typeAdapter = new ArrayAdapter<SoftButtonType>(this,
				android.R.layout.simple_spinner_item, SoftButtonType.values());
		typeAdapter
				.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		spinnerType.setAdapter(typeAdapter);

		imageTypeAdapter = new ArrayAdapter<ImageType>(this,
				android.R.layout.simple_spinner_item, ImageType.values());
		imageTypeAdapter
				.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		spinnerImageType.setAdapter(imageTypeAdapter);

		systemActionAdapter = new ArrayAdapter<SystemAction>(this,
				android.R.layout.simple_spinner_item, SystemAction.values());
		systemActionAdapter
				.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		spinnerSystemAction.setAdapter(systemActionAdapter);

		spinnerType.setSelection(typeAdapter.getPosition(softButton.getType()));
		switch (softButton.getType()) {
		case SBT_TEXT:
			setTextFromSoftButton();
			break;
		case SBT_IMAGE:
			setImageFromSoftButton();
			break;
		case SBT_BOTH:
			setTextFromSoftButton();
			setImageFromSoftButton();
			break;
		}
		checkBoxHighlighted.setChecked(softButton.getIsHighlighted());
		spinnerSystemAction.setSelection(systemActionAdapter
				.getPosition(softButton.getSystemAction()));
	}

	private void setImageFromSoftButton() {
		editImage.setText(softButton.getImage().getValue());
		spinnerImageType.setSelection(imageTypeAdapter.getPosition(softButton
				.getImage().getImageType()));
	}

	private void setTextFromSoftButton() {
		editText.setText(softButton.getText());
	}

	private void setTextToSoftButton(SoftButton softButton) {
		softButton.setText(editText.getText().toString());
	}

	private void setImageToSoftButton(SoftButton softButton) {
		Image image = new Image();
		image.setValue(editImage.getText().toString());
		image.setImageType((ImageType) spinnerImageType.getSelectedItem());
		softButton.setImage(image);
	}

}
