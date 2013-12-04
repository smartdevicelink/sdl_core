package com.ford.syncV4.android.activity;

import android.content.Context;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;
import android.widget.TwoLineListItem;

import com.ford.syncV4.android.R;
import com.ford.syncV4.proxy.RPCStruct;

import java.util.Vector;

/**
 * Base list adapter for different RPCStruct subclasses.
 *
 * Created by enikolsky on 2013-11-25.
 */
abstract class RPCStructAdapter<T extends RPCStruct> extends ArrayAdapter<T> {
    private static final String LOG_TAG =
            RPCStructAdapter.class.getSimpleName();
    protected int maxObjectsNumber;
    protected Vector<T> objects = null;

    public RPCStructAdapter(Context context, Vector<T> objects,
                            int maxObjectsNumber) {
        super(context, 0, objects);
        this.objects = objects;
        this.maxObjectsNumber = maxObjectsNumber;
    }

    /**
     * Returns true if the objects list contains maximum number of items or
     * more.
     */
    protected boolean isMaxReached() {
        return objects.size() >= maxObjectsNumber;
    }

    @Override
    public int getCount() {
        // return the number of soft buttons
        // + 1 to add row if maximum is not reached
        return objects.size() + (isMaxReached() ? 0 : 1);
    }

    @Override
    public int getViewTypeCount() {
        // 1 is regular soft button row
        // 2 is add soft button row
        return ItemType.COUNT;
    }

    @Override
    public int getItemViewType(int position) {
        if ((position == objects.size()) && !isMaxReached()) {
            return ItemType.ADD;
        }
        return ItemType.REGULAR;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        switch (getItemViewType(position)) {
            case ItemType.REGULAR: {
                TwoLineListItem item = (TwoLineListItem) convertView;
                if (item == null) {
                    LayoutInflater inflater =
                            (LayoutInflater) getContext().getSystemService(
                                    Context.LAYOUT_INFLATER_SERVICE);
                    item = (TwoLineListItem) inflater.inflate(
                            R.layout.rpcstruct_row, null);
                }

                fillItem(item, position);
                return item;
            }

            case ItemType.ADD: {
                TextView text = (TextView) convertView;
                if (text == null) {
                    LayoutInflater inflater =
                            (LayoutInflater) getContext().getSystemService(
                                    Context.LAYOUT_INFLATER_SERVICE);
                    text = (TextView) inflater.inflate(
                            android.R.layout.simple_list_item_1, null);
                }

                text.setGravity(Gravity.CENTER);
                text.setText(titleForAddRow());

                return text;
            }

            default:
                Log.w(LOG_TAG,
                        "Unknown item view type: " + getItemViewType(position));
                return null;
        }
    }

    protected abstract String titleForAddRow();

    protected abstract void fillItem(TwoLineListItem item, int position);

    protected static class ItemType {
        static final int REGULAR = 0;
        static final int ADD = 1;
        static final int COUNT = 2;
    }
}
