package com.techinstitute.mylibrary;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import com.techinstitute.mylibrary.R;

public class LibraryActivity extends Activity {
	public static Intent createIntent(Context context) {
		return new Intent(context, LibraryActivity.class);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_library);
	}
}
