package com.eaglesakura.sample.graphics;

import android.app.Fragment;
import android.app.FragmentTransaction;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

/**
 * 全体構成メニュー用
 */
public class MenuFragment extends Fragment {

    /**
     * メニュー一覧
     */
    ListView list;

    /**
     * adapter
     */
    BaseAdapter adapter;

    /**
     * サンプル一覧
     */
    List<SampleData> samples = new ArrayList<SampleData>();

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View result = inflater.inflate(R.layout.fragment_menu, null);
        list = (ListView) result.findViewById(R.id.list);

        initMenuAdapter();
        return result;
    }

    void initMenuAdapter() {
        samples.clear();

        int chapterNumber = 1;
        {
            int sampleNumber = 1;
            samples.add(new SampleData(String.format("%d-%d %s", chapterNumber, sampleNumber++, "Hello OpenGL ES"), Chapter01_01.class));
            samples.add(new SampleData(String.format("%d-%d %s", chapterNumber, sampleNumber++, "三角形を描画する"), Chapter01_02.class));
            samples.add(new SampleData(String.format("%d-%d %s", chapterNumber, sampleNumber++, "四角形を描画する"), Chapter01_03.class));
            samples.add(new SampleData(String.format("%d-%d %s", chapterNumber, sampleNumber++, "テクスチャを描画する"), Chapter01_04.class));

            ++chapterNumber;
        }

        adapter = new BaseAdapter() {
            @Override
            public int getCount() {
                return samples.size();
            }

            @Override
            public Object getItem(int position) {
                return samples.get(position);
            }

            @Override
            public long getItemId(int position) {
                return position;
            }

            @Override
            public View getView(int position, View convertView, ViewGroup parent) {
                if (convertView == null) {
                    convertView = View.inflate(getActivity(), R.layout.menu_item, null);
                }

                final SampleData sample = samples.get(position);
                ((TextView) convertView.findViewById(R.id.sample_title)).setText(sample.title);
                return convertView;
            }
        };
        list.setAdapter(adapter);
        list.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                final SampleData sample = samples.get(position);
                try {
                    FragmentTransaction transaction = getFragmentManager().beginTransaction();
                    Fragment fragment = sample.fragmentClass.newInstance();
                    transaction.replace(R.id.container, fragment);
                    transaction.addToBackStack(null);
                    transaction.commit();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }

    class SampleData {
        /**
         * 表示用タイトル
         */
        String title;

        /**
         * 描画フラグメント用Class
         */
        Class<? extends Fragment> fragmentClass;

        SampleData(String title, Class<? extends Fragment> clazz) {
            this.title = title;
            this.fragmentClass = clazz;
        }
    }
}
