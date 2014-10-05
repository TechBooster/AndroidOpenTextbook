package com.techinstitute.usemylibrary;

import com.techinstitute.mylibrary.LibraryActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;

public class MainActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		findViewById(R.id.button).setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = LibraryActivity.createIntent(MainActivity.this);
				startActivity(intent);
			}
		});
	}
}
