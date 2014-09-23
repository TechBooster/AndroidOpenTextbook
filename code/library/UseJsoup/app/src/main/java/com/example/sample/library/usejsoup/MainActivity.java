package com.example.sample.library.usejsoup;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;

import org.apache.http.Header;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.parser.Parser;
import org.jsoup.select.Elements;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        loadAsync("http://rss.dailynews.yahoo.co.jp/fc/computer/rss.xml");
    }

    private void loadAsync(String url) {
        AsyncHttpClient client = new AsyncHttpClient();
        client.get(url,
                new AsyncHttpResponseHandler() {
                    @Override
                    public void onSuccess(int responseCode, Header[] headers, byte[] response) {
                        String body = new String(response);
                        Log.d("TEST", "body=" + body);
                        parseRss(body);
                    }

                    @Override
                    public void onFailure(int responseCode, Header[] headers, byte[] response, Throwable e) {
                        //省略
                    }
                });
    }

    private void parseRss(String rssBody) {
        Document document = Jsoup.parse(rssBody, "", Parser.xmlParser());
        List<String> titles = new ArrayList<String>();

        Elements items = document.select("item");
        for (Element item : items) {
            titles.add(item.select("title").first().text());
        }

        showRss(titles);
    }

    private void showRss(List<String> titles) {
        ListView listView = (ListView) findViewById(R.id.list_view);

        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        for(String title : titles){
            adapter.add(title);
        }

        listView.setAdapter(adapter);
    }
}
