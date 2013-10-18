package com.ford.sdlreverse.activity;

import com.ford.avatar.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class EulaActivity extends Activity {

	private Button btnAgree;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.eula);
		btnAgree=(Button)findViewById(R.id.btnAgree);
		Bundle extras = getIntent().getExtras();
		if (extras != null && !extras.getBoolean("firstStart")){
			btnAgree.setText("Done");
		}
		btnAgree.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent intent = getIntent();
				setResult(Activity.RESULT_OK, intent);
				finish();
			}
		});
	}
	

}
