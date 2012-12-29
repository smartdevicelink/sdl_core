package com.ford.syncV4.android.activity;

import java.util.List;
import java.util.Vector;

import android.app.ListActivity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.TwoLineListItem;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;

public class SoftButtonsListActivity extends ListActivity {
	private final static String LOG_TAG = SoftButtonsListActivity.class
			.getSimpleName();

	private Vector<SoftButton> softButtons = null;

	class SoftButtonsAdapter extends ArrayAdapter<SoftButton> {

		public SoftButtonsAdapter(Context context, List<SoftButton> objects) {
			super(context, 0, objects);
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			TwoLineListItem item = (TwoLineListItem) convertView;
			if (item == null) {
				LayoutInflater inflater = (LayoutInflater) getContext()
						.getSystemService(LAYOUT_INFLATER_SERVICE);
				item = (TwoLineListItem) inflater.inflate(
						R.layout.softbutton_row, null);
			}

			SoftButton softButton = getItem(position);
			TextView text1 = item.getText1();
			TextView text2 = item.getText2();

			StringBuilder b = new StringBuilder();
			b.append("[" + softButton.getType().name() + "], ");
			switch (softButton.getType()) {
			case SBT_TEXT:
				b.append("\"" + softButton.getText() + "\"");
				break;
			case SBT_IMAGE:
				b.append("\"" + softButton.getImage().getValue() + "\" ["
						+ softButton.getImage().getImageType().name() + "]");
				break;
			case SBT_BOTH:
				b.append("\"" + softButton.getText() + "\", \""
						+ softButton.getImage().getValue() + "\" ["
						+ softButton.getImage().getImageType().name() + "]");
				break;
			}
			String line1 = b.toString();
			String line2 = softButton.getSystemAction().name() + ", "
					+ (softButton.getIsHighlighted() ? "" : "non-")
					+ "highlighted, id=" + softButton.getSoftButtonID();
			text1.setText(line1);
			text2.setText(line2);

			return item;
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		softButtons = new Vector<SoftButton>();

		SoftButton sb1 = new SoftButton();
		sb1.setSoftButtonID(5400);
		sb1.setText("Reply");
		sb1.setType(SoftButtonType.SBT_TEXT);
		sb1.setIsHighlighted(false);
		sb1.setSystemAction(SystemAction.STEAL_FOCUS);
		softButtons.add(sb1);

		Image img = new Image();
		img.setValue("imageFilename");
		img.setImageType(ImageType.STATIC);

		SoftButton sb2 = new SoftButton();
		sb2.setSoftButtonID(5399);
		sb2.setText("Close");
		sb2.setType(SoftButtonType.SBT_BOTH);
		sb2.setImage(img);
		sb2.setIsHighlighted(true);
		sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
		softButtons.add(sb2);

		SoftButton sb3 = new SoftButton();
		sb3.setSoftButtonID(5401);
		sb3.setType(SoftButtonType.SBT_IMAGE);
		sb3.setImage(img);
		sb3.setIsHighlighted(false);
		sb3.setSystemAction(SystemAction.KEEP_CONTEXT);
		softButtons.add(sb3);

		ArrayAdapter<SoftButton> adapter = new SoftButtonsAdapter(this,
				softButtons);
		setListAdapter(adapter);
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		SoftButton softButton = softButtons.get(position);
		Log.w(LOG_TAG, "selected " + softButton);
		
		IntentHelper.addObjectForKey(softButton, Const.INTENTHELPER_KEY_SOFTBUTTON);
		startActivity(new Intent(this, SoftButtonEditActivity.class));
	}

}
