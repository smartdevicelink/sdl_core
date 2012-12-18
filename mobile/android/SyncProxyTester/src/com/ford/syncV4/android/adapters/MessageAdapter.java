package com.ford.syncV4.android.adapters;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.ford.syncV4.android.R;
import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.Result;

public class MessageAdapter extends ArrayAdapter<Object> {
	private LayoutInflater vi;
	private ArrayList<Object> items;
	
	public MessageAdapter(Context context, int textViewResourceId, ArrayList<Object> items) {
        super(context, textViewResourceId, items);
        this.vi = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        this.items = items;
	}
	
	/** Adds the specified message to the items list and notifies of the change. */
	public void addMessage(Object m) {
		items.add(m);
		notifyDataSetChanged();
	}
	
    public View getView(int position, View convertView, ViewGroup parent) {
    	ViewGroup rowView = (ViewGroup)convertView;
        Object rpcObj = getItem(position);
        rowView = (ViewGroup)vi.inflate(R.layout.row, null);
        if (rpcObj != null) {
            TextView lblTop = (TextView) rowView.findViewById(R.id.toptext);
            TextView lblBottom = (TextView) rowView.findViewById(R.id.bottomtext);
            
            if (rpcObj instanceof String) {
            	lblTop.setText((String)rpcObj);
            }
            else if (rpcObj instanceof RPCMessage) {
            	RPCMessage func = (RPCMessage)rpcObj;
            	if (func.getMessageType().equals(Names.request)) {
					lblTop.setTextColor(Color.CYAN);
            	} else if (func.getMessageType().equals(Names.notification)) {
					lblTop.setTextColor(Color.YELLOW);
            	} else if (func.getMessageType().equals(Names.response)) {
					lblTop.setTextColor(Color.argb(255, 32, 161, 32));
            	}
            	
            	lblTop.setText(func.getFunctionName() + " (" + func.getMessageType() + ")");
            	
            	try {
					Method getSuccessMethod = rpcObj.getClass().getDeclaredMethod("getSuccess");
					boolean isSuccess = (Boolean)getSuccessMethod.invoke(func);
					if (isSuccess) {
						lblTop.setTextColor(Color.GREEN);
					}
					else {
						lblTop.setTextColor(Color.RED);
					}
					Method getInfoMethod = rpcObj.getClass().getDeclaredMethod("getInfo");
					Method getResultCodeMethod = rpcObj.getClass().getDeclaredMethod("getResultCode");
					
					String info = (String)getInfoMethod.invoke(rpcObj);
					Result result = (Result)getResultCodeMethod.invoke(rpcObj);
					
					lblBottom.setText(result + ": " + info);
					
				} catch (NoSuchMethodException e) {
					rowView.removeView(lblBottom);
				} catch (SecurityException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IllegalArgumentException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IllegalAccessException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (InvocationTargetException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            }
        }

        return rowView;
    }
}
