package com.example.sample.network;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {

    private MainActivity self = this;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initUseSocketButton();
        initUseHttpURLConnectionButton();
        initUseHttpClientButton();
        initUseVolleyButton();
        initIMEButton();
    }

    private void initUseSocketButton() {
        Button button = (Button) findViewById(R.id.use_socket_button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(self, SocketActivity.class);
                startActivity(intent);
            }
        });
    }

    private void initUseHttpURLConnectionButton() {
        Button button = (Button) findViewById(R.id.use_http_url_connection_button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(self, HttpURLConnectionActivity.class);
                startActivity(intent);
            }
        });
    }

    private void initUseHttpClientButton() {
        Button button = (Button) findViewById(R.id.use_http_client_button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(self, HttpClientActivity.class);
                startActivity(intent);
            }
        });
    }

    private void initUseVolleyButton() {
        Button button = (Button) findViewById(R.id.use_volley_button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(self, VolleyActivity.class);
                startActivity(intent);
            }
        });
    }

    private void initIMEButton() {
        Button button = (Button) findViewById(R.id.ime_button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(self, SocialIMEActivity.class);
                startActivity(intent);
            }
        });
    }

}
