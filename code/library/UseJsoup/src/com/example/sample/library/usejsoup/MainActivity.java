package com.example.sample.library.usejsoup;

import java.util.ArrayList;
import java.util.List;

import org.apache.http.Header;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.parser.Parser;
import org.jsoup.select.Elements;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		loadAsync("http://rss.dailynews.yahoo.co.jp/fc/computer/rss.xml");
	}

	private void loadAsync(String url) {
		AsyncHttpClient client = new AsyncHttpClient();
		client.get(url, new AsyncHttpResponseHandler() {
			@Override
			public void onSuccess(int responseCode, Header[] headers,
					byte[] response) {
				String body = new String(response);
				Log.d("TEST", "body=" + body);
				parseRss(body);
			}

			@Override
			public void onFailure(int responseCode, Header[] headers,
					byte[] response, Throwable e) {
				// 省略
			}
		});
	}

	private void parseRss(String rssBody) {
		Document document = Jsoup.parse(rssBody, "", Parser.xmlParser());
		List<Item> itemList = new ArrayList<Item>();

		Elements items = document.select("item");
		for (Element element : items) {
			Item item = new Item();

			item.setTitle(element.select("title").first().text());
			item.setUrl(element.select("link").first().text());

			itemList.add(item);
		}

		showRss(itemList);
	}

	private void showRss(List<Item> itemList) {
		ListView listView = (ListView) findViewById(R.id.list_view);

		final RssItemAdapter adapter = new RssItemAdapter(this);

		for (Item item : itemList) {
			adapter.add(item);
		}

		listView.setAdapter(adapter);
		listView.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				Item item = adapter.getItem(position);
				openLink(item.getUrl());
			}
		});
	}
	
	private void openLink(String link){
		Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(link));
		startActivity(intent);
	}
}
