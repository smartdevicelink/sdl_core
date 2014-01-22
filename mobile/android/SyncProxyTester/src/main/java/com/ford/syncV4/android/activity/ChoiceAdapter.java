package com.ford.syncV4.android.activity;

import android.content.Context;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.TwoLineListItem;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.utils.StringUtils;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.enums.ImageType;

import java.util.Vector;

/**
 * Created by enikolsky on 2013-11-25.
 */
public class ChoiceAdapter extends RPCStructAdapter<Choice> {
    public ChoiceAdapter(Context context, Vector<Choice> objects,
                         int maxObjectsNumber) {
        super(context, objects, maxObjectsNumber);
    }

    @Override
    protected String titleForAddRow() {
        return "Add choice";
    }

    @Override
    protected void fillItem(TwoLineListItem item, int position) {
        final Choice choice = getItem(position);
        TextView text1 = item.getText1();
        TextView text2 = item.getText2();

        ImageButton btnDelete =
                (ImageButton) item.findViewById(R.id.rpcstructrow_deleteButton);
        btnDelete.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                objects.remove(choice);
                notifyDataSetChanged();
            }
        });

        StringBuilder sb1 = new StringBuilder();
        sb1.append("#").append(choice.getChoiceID()).append(": ");
        sb1.append("\"").append(choice.getMenuName()).append("\"");

        final Vector<String> vrCommands = choice.getVrCommands();
        if (vrCommands != null) {
            sb1.append(" (")
               .append(StringUtils.joinStrings(vrCommands))
               .append(")");
        }

        sb1.append(", ").append(imageToString(choice.getImage()));
        text1.setText(sb1.toString());

        StringBuilder sb2 = new StringBuilder();
        final String secondaryText = choice.getSecondaryText();
        if (secondaryText != null) {
            sb2.append("\"").append(secondaryText).append("\", ");
        }

        final String tertiaryText = choice.getTertiaryText();
        if (tertiaryText != null) {
            sb2.append("\"").append(tertiaryText).append("\", ");
        }

        sb2.append(", ").append(imageToString(choice.getSecondaryImage()));
        text2.setText(sb2.toString());
    }

    private String imageToString(Image image) {
        if (image == null) {
            return "";
        }

        StringBuilder sb = new StringBuilder();
        sb.append("{");

        final String value = image.getValue();
        if (value != null) {
            sb.append(value).append(", ");
        }

        final ImageType imageType = image.getImageType();
        if (imageType != null) {
            sb.append(imageType.name());
        }

        sb.append("}");
        return sb.toString();
    }
}
