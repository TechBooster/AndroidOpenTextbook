package com.example.sample.library.useandroidhttpasync.activities;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import com.example.sample.library.useandroidhttpasync.R;
import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;

import org.apache.http.Header;

public class AndroidHttpAsyncActivity extends Activity {

	public interface OnLoadListener {
		public void onLoad(String result);
	}
	
	public static Intent createIntent(Context context) {
		return new Intent(context, AndroidHttpAsyncActivity.class);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_android_http_async);

		loadAsync("http://rss.dailynews.yahoo.co.jp/fc/computer/rss.xml", new OnLoadListener() {
			@Override
			public void onLoad(String result) {
				showBody(result);
			}
		});
	}

	private void loadAsync(String url, final OnLoadListener listener) {
		if (TextUtils.isEmpty(url)) {
			return;
		}
		if(!url.startsWith("http://")){
			Toast.makeText(this, "startsWith", Toast.LENGTH_SHORT).show();
			return;
		}
		AsyncHttpClient client = new AsyncHttpClient();
		client.get(url, new AsyncHttpResponseHandler() {
			@Override
			public void onSuccess(int responseCode, Header[] headers,
					byte[] response) {
				String body = new String(response);
				listener.onLoad(body);
			}

			@Override
			public void onFailure(int responseCode, Header[] headers,
					byte[] response, Throwable e) {
				String error = response != null ? new String(response) : "";
				Log.d("TEST", "status=" + responseCode + ", error=" + error
						+ ", e=" + e.getMessage());
			}
		});
	}

	private void showBody(String body) {
		TextView textView = (TextView) findViewById(R.id.text);
		textView.setText(body);
	}
}
