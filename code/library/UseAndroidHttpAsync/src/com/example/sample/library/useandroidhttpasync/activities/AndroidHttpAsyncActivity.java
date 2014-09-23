package com.example.sample.library.useandroidhttpasync.activities;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.example.sample.library.useandroidhttpasync.R;
import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;

import org.apache.http.Header;

public class AndroidHttpAsyncActivity extends Activity {

    public static Intent createIntent(Context context){
        return new Intent(context, AndroidHttpAsyncActivity.class);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_android_http_async);

        loadAsync();
    }

    private void loadAsync() {
        AsyncHttpClient client = new AsyncHttpClient();
        client.get("http://tomorrowkey.github.io",
                new AsyncHttpResponseHandler() {
                    @Override
                    public void onSuccess(int responseCode, Header[] headers, byte[] response) {
                        String body = new String(response);
                        Log.d("TEST", "body=" + body);
                        showBody(body);
                    }

                    @Override
                    public void onFailure(int responseCode, Header[] headers, byte[] response, Throwable e) {
                        //省略
                    }
                });
    }

    private void showBody(String body){
        TextView textView = (TextView) findViewById(R.id.text);
        textView.setText(body);
    }
}
