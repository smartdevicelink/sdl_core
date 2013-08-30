package com.ford.syncV4.android.policies;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.ford.syncV4.android.R;

public class UIMessageAdapter extends ArrayAdapter<Object> {
	private LayoutInflater vi;
	
	public UIMessageAdapter(Context context, int textViewResourceId) {
        super(context, textViewResourceId);
        this.vi = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	}
	
    public View getView(int position, View convertView, ViewGroup parent) {
    	ViewGroup rowView = (ViewGroup)convertView;
        Object rpcObj = getItem(position);
        rowView = (ViewGroup)vi.inflate(R.layout.policiesrow, null);
        
        if (rpcObj != null) {
            TextView lblTop = (TextView) rowView.findViewById(R.id.toptext);
            TextView lblBottom = (TextView) rowView.findViewById(R.id.bottomtext);
            
            if (rpcObj instanceof String) {
            	lblTop.setText((String)rpcObj);
            }
           
        }
        return rowView;
    }
}
