package com.ford.syncV4.android.activity;

import java.util.List;
import java.util.Vector;

import android.app.ListActivity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageButton;
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
	/** Index of the softButton being edited. */
	private int currentSoftButtonIndex = -1;
	private ArrayAdapter<SoftButton> adapter;

	class SoftButtonsAdapter extends ArrayAdapter<SoftButton> {
		private static final int ITEMTYPE_REGULAR = 0;
		private static final int ITEMTYPE_ADD = 1;

		public SoftButtonsAdapter(Context context, List<SoftButton> objects) {
			super(context, 0, objects);
		}

		@Override
		public int getCount() {
			// return the number of soft buttons + 1 to add row
			return softButtons.size() + 1;
		}

		@Override
		public int getViewTypeCount() {
			// 1 is regular soft button row
			// 2 is add soft button row
			return 2;
		}

		@Override
		public int getItemViewType(int position) {
			if (position == softButtons.size()) {
				return ITEMTYPE_ADD;
			} else {
				return ITEMTYPE_REGULAR;
			}
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			switch (getItemViewType(position)) {
			case ITEMTYPE_REGULAR: {
				TwoLineListItem item = (TwoLineListItem) convertView;
				if (item == null) {
					LayoutInflater inflater = (LayoutInflater) getContext()
							.getSystemService(LAYOUT_INFLATER_SERVICE);
					item = (TwoLineListItem) inflater.inflate(
							R.layout.softbutton_row, null);
				}

				final SoftButton softButton = getItem(position);
				TextView text1 = item.getText1();
				TextView text2 = item.getText2();

				ImageButton btnDelete = (ImageButton) item
						.findViewById(R.id.softbuttonrow_deleteButton);
				btnDelete.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						softButtons.remove(softButton);
						notifyDataSetChanged();
					}
				});

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

			case ITEMTYPE_ADD: {
				TextView text = (TextView) convertView;
				if (text == null) {
					LayoutInflater inflater = (LayoutInflater) getContext()
							.getSystemService(LAYOUT_INFLATER_SERVICE);
					text = (TextView) inflater.inflate(
							android.R.layout.simple_list_item_1, null);
				}

				text.setGravity(Gravity.CENTER);
				text.setText("Add soft button");

				return text;
			}

			default:
				Log.w(LOG_TAG, "Unknown item view type: "
						+ getItemViewType(position));
				return null;
			}
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.softbuttons);

		Button btnOk = ((Button) findViewById(R.id.softbuttons_ok));
		btnOk.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				IntentHelper.addObjectForKey(softButtons,
						Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
				setResult(RESULT_OK);
				finish();
			}
		});

		Button btnCancel = ((Button) findViewById(R.id.softbuttons_cancel));
		btnCancel.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				setResult(RESULT_CANCELED);
				finish();
			}
		});

		softButtons = (Vector<SoftButton>) IntentHelper
				.getObjectForKey(Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
		adapter = new SoftButtonsAdapter(this, softButtons);
		setListAdapter(adapter);
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		if (position == softButtons.size()) {
			// create and add default soft button
			Image img = new Image();
			img.setValue("imageFilename");
			img.setImageType(ImageType.STATIC);

			SoftButton sb = new SoftButton();
			sb.setSoftButtonID(5402);
			sb.setText("Close");
			sb.setType(SoftButtonType.SBT_BOTH);
			sb.setImage(img);
			sb.setIsHighlighted(true);
			sb.setSystemAction(SystemAction.DEFAULT_ACTION);

			softButtons.add(sb);
			adapter.notifyDataSetChanged();
		} else {
			currentSoftButtonIndex = position;
			SoftButton softButton = softButtons.get(position);
			IntentHelper.addObjectForKey(softButton,
					Const.INTENTHELPER_KEY_SOFTBUTTON);
			startActivityForResult(new Intent(this,
					SoftButtonEditActivity.class),
					Const.REQUEST_EDIT_SOFTBUTTON);
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch (requestCode) {
		case Const.REQUEST_EDIT_SOFTBUTTON:
			if (resultCode == RESULT_OK) {
				SoftButton result = (SoftButton) IntentHelper
						.getObjectForKey(Const.INTENTHELPER_KEY_SOFTBUTTON);
				softButtons.set(currentSoftButtonIndex, result);
				adapter.notifyDataSetChanged();
			}
			currentSoftButtonIndex = -1;
			IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_SOFTBUTTON);
			break;
		default:
			Log.i(LOG_TAG, "Unknown request code: " + requestCode);
			break;
		}
	}
}
