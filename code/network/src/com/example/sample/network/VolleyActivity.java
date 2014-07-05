package com.example.sample.network;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONObject;

public class VolleyActivity extends Activity {

    private RequestQueue mRequestQueue;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_volley);

        initVolley();
        initDownloadButton();
    }

    private void initVolley() {
        mRequestQueue = Volley.newRequestQueue(getApplicationContext());
    }

    private void initDownloadButton() {
        Button button = (Button) findViewById(R.id.download_button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                download();
            }
        });
    }

    private void download() {
        int method = Request.Method.GET;
        String url = "https://raw.githubusercontent.com/TechBooster/AndroidOpenTextbook/master/code/network/assets/sample.json";
        JSONObject requestBody = null;
        Response.Listener<JSONObject> listener = new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject jsonObject) {
                Log.d("TEST", jsonObject.toString());
            }
        };
        Response.ErrorListener errorListener = new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError volleyError) {
                NetworkResponse networkResponse = volleyError.networkResponse;
                int statusCode = networkResponse.statusCode;
                Log.d("TEST", "Status-Code=" + statusCode);

                String contentLength = networkResponse.headers.get("Content-Length");
                Log.d("TEST", "Content-Length=" + contentLength);

                String body = new String(networkResponse.data);
                Log.d("TEST", body);
            }
        };

        mRequestQueue.add(new JsonObjectRequest(method, url, requestBody, listener, errorListener));
    }

}
