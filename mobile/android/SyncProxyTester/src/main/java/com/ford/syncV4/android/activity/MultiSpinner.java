package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.util.AttributeSet;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

import java.util.ArrayList;
import java.util.List;

/**
 * A Spinner subclass to allow to select multiple items. Based on
 * http://stackoverflow.com/questions/5015686/android-spinner-with-multiple-choice/6022474#6022474
 */
public class MultiSpinner<ItemType> extends Spinner
        implements DialogInterface.OnMultiChoiceClickListener,
        DialogInterface.OnCancelListener {

    private List<ItemType> items;
    private boolean[] selected;
    private String defaultText;
    private MultiSpinnerListener listener;

    public MultiSpinner(Context context) {
        super(context);
    }

    public MultiSpinner(Context arg0, AttributeSet arg1) {
        super(arg0, arg1);
    }

    public MultiSpinner(Context arg0, AttributeSet arg1, int arg2) {
        super(arg0, arg1, arg2);
    }

    @Override
    public void onClick(DialogInterface dialog, int which, boolean isChecked) {
        if (isChecked) {
            selected[which] = true;
        } else {
            selected[which] = false;
        }
    }

    @Override
    public void onCancel(DialogInterface dialog) {
        // refresh text on spinner
        StringBuilder spinnerBuffer = new StringBuilder();
        boolean someUnselected = false;
        for (int i = 0; i < items.size(); i++) {
            if (selected[i]) {
                spinnerBuffer.append(items.get(i));
                spinnerBuffer.append(", ");
            } else {
                someUnselected = true;
            }
        }
        String spinnerText;
        if (someUnselected) {
            spinnerText = spinnerBuffer.toString();
            if (spinnerText.length() > 2) {
                spinnerText =
                        spinnerText.substring(0, spinnerText.length() - 2);
            }
        } else {
            spinnerText = defaultText;
        }
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(getContext(),
                android.R.layout.simple_spinner_item,
                new String[]{ spinnerText });
        setAdapter(adapter);
        if (listener != null) {
            listener.onItemsSelected(selected);
        }
    }

    @Override
    public boolean performClick() {
        AlertDialog.Builder builder = new AlertDialog.Builder(getContext());
        final CharSequence[] array = new CharSequence[items.size()];
        int i = 0;
        for (ItemType item : items) {
            array[i] = item.toString();
            ++i;
        }
        builder.setMultiChoiceItems(array, selected, this);
        builder.setPositiveButton(android.R.string.ok,
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.cancel();
                    }
                });
        builder.setOnCancelListener(this);
        builder.show();

        return true;
    }

    public void setItems(List<ItemType> items, String allText,
                         MultiSpinnerListener listener) {
        this.items = items;
        this.defaultText = allText;
        this.listener = listener;

        // all selected by default
        selected = new boolean[items.size()];
        for (int i = 0; i < selected.length; i++) {
            selected[i] = true;
        }

        // all text on the spinner
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(getContext(),
                android.R.layout.simple_spinner_item, new String[]{ allText });
        setAdapter(adapter);
    }

    public boolean[] getSelected() {
        return selected;
    }

    public List<ItemType> getSelectedItems() {
        final ArrayList<ItemType> res = new ArrayList<ItemType>();
        for (int i = 0; i < selected.length; ++i) {
            if (selected[i]) {
                res.add(items.get(i));
            }
        }
        return res;
    }

    public interface MultiSpinnerListener {
        public void onItemsSelected(boolean[] selected);
    }
}
