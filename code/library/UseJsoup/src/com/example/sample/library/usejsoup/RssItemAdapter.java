package com.example.sample.library.usejsoup;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

public class RssItemAdapter extends ArrayAdapter<Item> {

	public RssItemAdapter(Context context) {
		super(context, -1);
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		Item item = getItem(position);
		ViewHolder holder = null;
		if (convertView == null) {
			convertView = View.inflate(getContext(),
					R.layout.listitem_rss_item, null);
			convertView.setTag(new ViewHolder(convertView));
		}
		holder = (ViewHolder) convertView.getTag();

		holder.title.setText(item.getTitle());
		holder.url.setText(item.getUrl());

		return convertView;
	}

	static class ViewHolder {
		TextView title;
		TextView url;

		public ViewHolder(View root) {
			title = (TextView) root.findViewById(R.id.title);
			url = (TextView) root.findViewById(R.id.url);
		}
	}
}
