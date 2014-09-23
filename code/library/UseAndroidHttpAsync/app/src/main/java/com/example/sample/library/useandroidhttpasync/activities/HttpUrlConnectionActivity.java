package com.example.sample.library.useandroidhttpasync.activities;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;

import com.example.sample.library.useandroidhttpasync.R;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class HttpUrlConnectionActivity extends Activity {

    public static Intent createIntent(Context context) {
        return new Intent(context, HttpUrlConnectionActivity.class);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_http_url_connection);

        new DownloadTask() {
            @Override
            protected void onPostExecute(String body) {
                showBody(body);
            }
        }.execute();
    }

    private void showBody(String body){
        TextView textView = (TextView) findViewById(R.id.text);
        textView.setText(body);
    }

    private static class DownloadTask extends AsyncTask<Void, Void, String> {
        @Override
        protected String doInBackground(Void... params) {
            try {
                URL url = new URL("http://tomorrowkey.github.io");
                HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                connection.setRequestMethod("GET");
                connection.setRequestProperty("Host", "tomorrowkey.github.io");
                connection.connect();

                int responseCode = connection.getResponseCode();
                Log.d("TEST", "Response-Code=" + responseCode);
                String contentLength = connection.getHeaderField("Content-Length");
                Log.d("TEST", "Content-Length=" + contentLength);
                String contentType = connection.getHeaderField("Content-Type");
                Log.d("TEST", "Content-Type=" + contentType);

                InputStream inputStream = connection.getInputStream();
                String body = readToEnd(inputStream);
                Log.d("TEST", "body=" + body);
                inputStream.close();

                return body;
            } catch (MalformedURLException e) {
                throw new RuntimeException(e);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        private String readToEnd(InputStream inputStream) throws IOException {
            StringBuilder sb = new StringBuilder();
            int length;
            byte[] buffer = new byte[1024];
            while ((length = inputStream.read(buffer)) != -1) {
                sb.append(new String(buffer, 0, length));
            }
            return sb.toString();
        }
    }
}
