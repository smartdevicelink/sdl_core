package com.ford.syncV4.android.activity;

import android.content.Context;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.TwoLineListItem;

import com.ford.syncV4.android.R;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;

import java.util.Vector;

class SoftButtonsAdapter extends RPCStructAdapter<SoftButton> {
    public SoftButtonsAdapter(Context context, Vector<SoftButton> objects,
                              int maxSoftButtonsNumber) {
        super(context, objects, maxSoftButtonsNumber);
    }

    @Override
    protected void fillItem(TwoLineListItem item, int position) {
        final SoftButton softButton = getItem(position);
        TextView text1 = item.getText1();
        TextView text2 = item.getText2();

        ImageButton btnDelete =
                (ImageButton) item.findViewById(R.id.rpcstructrow_deleteButton);
        btnDelete.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                objects.remove(softButton);
                notifyDataSetChanged();
            }
        });

        StringBuilder b = new StringBuilder();
        b.append("[").append(softButton.getType().name()).append("], ");
        switch (softButton.getType()) {
            case SBT_TEXT:
                b.append("\"").append(softButton.getText()).append("\"");
                break;
            case SBT_IMAGE:
                b.append("\"")
                 .append(softButton.getImage().getValue())
                 .append("\" [")
                 .append(softButton.getImage().getImageType().name())
                 .append("]");
                break;
            case SBT_BOTH:
                b.append("\"")
                 .append(softButton.getText())
                 .append("\", \"")
                 .append(softButton.getImage().getValue())
                 .append("\" [")
                 .append(softButton.getImage().getImageType().name())
                 .append("]");
                break;
        }

        String line1 = b.toString();
        final SystemAction systemAction = softButton.getSystemAction();
        final StringBuilder line2Builder = new StringBuilder();
        if (systemAction != null) {
            line2Builder.append(systemAction.name());
            line2Builder.append(", ");
        }

        line2Builder.append(softButton.getIsHighlighted() ? "" : "non-");
        line2Builder.append("highlighted, id=");
        line2Builder.append(softButton.getSoftButtonID());
        text1.setText(line1);
        text2.setText(line2Builder.toString());
    }
}
