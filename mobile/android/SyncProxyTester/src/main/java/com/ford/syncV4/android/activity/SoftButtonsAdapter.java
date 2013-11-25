package com.ford.syncV4.android.activity;

import android.content.Context;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.TwoLineListItem;

import com.ford.syncV4.android.R;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;

import java.util.Vector;

class SoftButtonsAdapter extends ArrayAdapter<SoftButton> {
    private static final int ITEMTYPE_REGULAR = 0;
    private static final int ITEMTYPE_ADD = 1;
    private static final String LOG_TAG =
            SoftButtonsAdapter.class.getSimpleName();
    Vector<SoftButton> softButtons = null;
    private int maxSoftButtonsNumber;

    public SoftButtonsAdapter(Context context, Vector<SoftButton> objects,
                              int maxSoftButtonsNumber) {
        super(context, 0, objects);
        this.softButtons = objects;
        this.maxSoftButtonsNumber = maxSoftButtonsNumber;
    }

    /**
     * Returns true if the soft buttons list contains maximum number of items or
     * more.
     */
    boolean isMaxReached() {
        return softButtons.size() >= maxSoftButtonsNumber;
    }

    @Override
    public int getCount() {
        // return the number of soft buttons
        // + 1 to add row if maximum is not reached
        return softButtons.size() + (isMaxReached() ? 0 : 1);
    }

    @Override
    public int getViewTypeCount() {
        // 1 is regular soft button row
        // 2 is add soft button row
        return 2;
    }

    @Override
    public int getItemViewType(int position) {
        if ((position == softButtons.size()) && !isMaxReached()) {
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
                    LayoutInflater inflater =
                            (LayoutInflater) getContext().getSystemService(
                                    Context.LAYOUT_INFLATER_SERVICE);
                    item = (TwoLineListItem) inflater.inflate(
                            R.layout.softbutton_row, null);
                }

                final SoftButton softButton = getItem(position);
                TextView text1 = item.getText1();
                TextView text2 = item.getText2();

                ImageButton btnDelete = (ImageButton) item.findViewById(
                        R.id.softbuttonrow_deleteButton);
                btnDelete.setOnClickListener(new View.OnClickListener() {
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
                        b.append("\"" + softButton.getImage().getValue() +
                                "\" [" +
                                softButton.getImage().getImageType().name() +
                                "]");
                        break;
                    case SBT_BOTH:
                        b.append("\"" + softButton.getText() + "\", \"" +
                                softButton.getImage().getValue() + "\" [" +
                                softButton.getImage().getImageType().name() +
                                "]");
                        break;
                }
                String line1 = b.toString();
                final SystemAction systemAction = softButton.getSystemAction();
                final StringBuilder line2Builder = new StringBuilder();
                if (systemAction != null) {
                    line2Builder.append(systemAction.name());
                    line2Builder.append(", ");
                }
                line2Builder.append(
                        softButton.getIsHighlighted() ? "" : "non-");
                line2Builder.append("highlighted, id=");
                line2Builder.append(softButton.getSoftButtonID());
                text1.setText(line1);
                text2.setText(line2Builder.toString());

                return item;
            }

            case ITEMTYPE_ADD: {
                TextView text = (TextView) convertView;
                if (text == null) {
                    LayoutInflater inflater =
                            (LayoutInflater) getContext().getSystemService(
                                    Context.LAYOUT_INFLATER_SERVICE);
                    text = (TextView) inflater.inflate(
                            android.R.layout.simple_list_item_1, null);
                }

                text.setGravity(Gravity.CENTER);
                text.setText("Add soft button");

                return text;
            }

            default:
                Log.w(LOG_TAG,
                        "Unknown item view type: " + getItemViewType(position));
                return null;
        }
    }
}
