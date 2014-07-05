package com.example.sample.network;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import org.apache.http.Header;
import org.apache.http.HttpResponse;
import org.apache.http.StatusLine;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;

public class HttpClientActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_http_client);
        initDownloadButton();
    }

    private void initDownloadButton() {
        Button button = (Button) findViewById(R.id.download_button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new DownloadTask().execute();
            }
        });
    }

    private static class DownloadTask extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... params) {
            try {
                HttpGet httpGet = new HttpGet("http://tomorrowkey.github.io");
                httpGet.addHeader("Host", "tomorrowkey.github.io");
                HttpClient httpClient = new DefaultHttpClient();

                HttpResponse httpResponse = httpClient.execute(httpGet);

                StatusLine statusLine = httpResponse.getStatusLine();
                Log.d("TEST", "Status-Code=" + statusLine.getStatusCode());

                Header contentLengthHeader = httpResponse.getFirstHeader("Content-Length");
                Log.d("TEST", "Content-Length=" + contentLengthHeader.getValue());
                Header contentTypeHeader = httpResponse.getFirstHeader("Content-Type");
                Log.d("TEST", "Content-Type=" + contentTypeHeader.getValue());

                InputStream inputStream = httpResponse.getEntity().getContent();
                String body = readToEnd(inputStream);
                Log.d("TEST", body);
                inputStream.close();
            } catch (MalformedURLException e) {
                throw new RuntimeException(e);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

            return null;
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
