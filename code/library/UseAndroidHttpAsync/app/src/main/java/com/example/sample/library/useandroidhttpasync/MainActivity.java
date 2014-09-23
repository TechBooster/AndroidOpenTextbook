package com.example.sample.library.useandroidhttpasync;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.example.sample.library.useandroidhttpasync.activities.AndroidHttpAsyncActivity;
import com.example.sample.library.useandroidhttpasync.activities.HttpUrlConnectionActivity;

public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setupUi();
    }

    private void setupUi(){
        ListView listView = (ListView)findViewById(R.id.list_view);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        adapter.add("HttpUrlConnectionを使う場合");
        adapter.add("android-async-httpを使う場合");

        listView.setAdapter(adapter);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                showActivity(position);
            }
        });
    }

    private void showActivity(int position){
        switch(position){
            case 0:
                startActivity(HttpUrlConnectionActivity.createIntent(this));
                break;
            case 1:
                startActivity(AndroidHttpAsyncActivity.createIntent(this));
                break;
        }
    }
}
