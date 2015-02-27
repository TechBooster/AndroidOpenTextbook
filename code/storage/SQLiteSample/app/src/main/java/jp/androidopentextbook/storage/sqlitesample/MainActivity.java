package jp.androidopentextbook.storage.sqlitesample;

import android.app.ListActivity;
import android.content.ContentValues;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

import butterknife.ButterKnife;
import butterknife.InjectView;
import butterknife.OnClick;
import jp.androidopentextbook.storage.sqlitesample.memo.Memo;
import jp.androidopentextbook.storage.sqlitesample.memo.MemoDB;


public class MainActivity extends ListActivity {

    @InjectView(R.id.edit_memo)
    EditText mEdit;

    private MemoDB mMemoDB;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ButterKnife.inject(this, this);

        // Create MemoDB object
        mMemoDB = new MemoDB(this);
    }

    @Override
    protected void onResume() {
        super.onResume();

        // load memo
        List<Memo> memoList = loadMemo();
        setListItems(memoList);
    }

    private List<Memo> loadMemo() {
        SQLiteDatabase db = mMemoDB.getReadableDatabase();
        Cursor cursor = db.query(MemoDB.TABLE_NAME, null, null, null, null, null,
                MemoDB.Columns.CREATE_TIME + " ASC");
        if (!cursor.moveToFirst()) {
            cursor.close();
            db.close();
            return new ArrayList<>();
        }
        // get column index
        int idIndex = cursor.getColumnIndex(MemoDB.Columns._ID);
        int memoIndex = cursor.getColumnIndex(MemoDB.Columns.MEMO);
        int createIndex = cursor.getColumnIndex(MemoDB.Columns.CREATE_TIME);
        int updateIndex = cursor.getColumnIndex(MemoDB.Columns.UPDATE_TIME);

        // read rows
        List<Memo> list = new ArrayList<>(cursor.getCount());
        do {
            Memo item = new Memo();
            item.mId = cursor.getInt(idIndex);
            item.mMemo = cursor.getString(memoIndex);
            item.mCreateTime = cursor.getLong(createIndex);
            item.mUpdateTime = cursor.getLong(updateIndex);

            list.add(item);
        } while (cursor.moveToNext());

        cursor.close();
        db.close();

        return list;
    }

    private void setListItems(List<Memo> list) {
        ArrayAdapter<Memo> adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1);
        adapter.addAll(list);
        setListAdapter(adapter);
    }

    // UI event

    @OnClick(R.id.button_create)
    void createClicked() {
        String memo = mEdit.getText().toString();
        int id = insertMemo(memo);
        mEdit.setText("");

        // add memo to list
        Memo item = new Memo();
        item.mId = id;
        item.mMemo = memo;
        item.mCreateTime = System.currentTimeMillis();
        item.mUpdateTime = System.currentTimeMillis();

        ArrayAdapter<Memo> adapter = (ArrayAdapter<Memo>) getListAdapter();
        adapter.add(item);
        adapter.notifyDataSetChanged();
    }

    private int insertMemo(String memo) {
        SQLiteDatabase db = mMemoDB.getWritableDatabase();

        // set row values
        ContentValues values = new ContentValues();
        values.put(MemoDB.Columns.MEMO, memo);
        values.put(MemoDB.Columns.CREATE_TIME, System.currentTimeMillis());
        values.put(MemoDB.Columns.UPDATE_TIME, System.currentTimeMillis());

        // insert method returns _id
        long id = db.insert(MemoDB.TABLE_NAME, null, values);

        db.close();

        return (int)id;
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        super.onListItemClick(l, v, position, id);
        Memo item = (Memo) l.getItemAtPosition(position);

        // Show edit activity
        Intent it = new Intent(this, EditActivity.class);
        it.putExtra(EditActivity.ARGS_ID, item.mId);
        it.putExtra(EditActivity.ARGS_MEMO, item.mMemo);
        startActivity(it);
    }
}
