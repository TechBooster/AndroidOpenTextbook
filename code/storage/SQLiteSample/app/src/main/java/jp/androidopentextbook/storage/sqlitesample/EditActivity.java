package jp.androidopentextbook.storage.sqlitesample;

import android.app.Activity;
import android.content.ContentValues;
import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.util.Log;
import android.widget.EditText;

import butterknife.ButterKnife;
import butterknife.InjectView;
import butterknife.OnClick;
import jp.androidopentextbook.storage.sqlitesample.memo.MemoDB;

/**
 * Activity for Edit memo
 */
public class EditActivity extends Activity {
    public static final String ARGS_ID = "id";
    public static final String ARGS_MEMO = "memo";

    private int mId;
    private MemoDB mMemoDB;

    @InjectView(R.id.edit_id)
    EditText mIdEdit;

    @InjectView(R.id.edit_memo)
    EditText mMemoEdit;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit);

        ButterKnife.inject(this, this);

        mMemoDB = new MemoDB(this);

        if (savedInstanceState == null) {
            Intent it = getIntent();
            if (it == null) {
                finish();
                return;
            }
            mId = it.getIntExtra(ARGS_ID, 0);
            String memo = it.getStringExtra(ARGS_MEMO);

            mIdEdit.setText(String.valueOf(mId));
            mMemoEdit.setText(memo);
        }
    }

    // UI event
    @OnClick(R.id.button_update)
    void updateClicked() {
        String memo = mMemoEdit.getText().toString();
        updateMemo(mId, memo);
        finish();
    }

    private void updateMemo(int id, String memo) {
        SQLiteDatabase db = mMemoDB.getWritableDatabase();

        ContentValues values = new ContentValues();
        values.put(MemoDB.Columns.MEMO, memo);
        values.put(MemoDB.Columns.UPDATE_TIME, System.currentTimeMillis());

        String where = MemoDB.Columns._ID + "=?";
        String[] args = { String.valueOf(id) };

        int count = db.update(MemoDB.TABLE_NAME, values, where, args);
        if (count == 0) {
            Log.v("Edit", "Failed to update");
        }

        db.close();
    }

    @OnClick(R.id.button_delete)
    void deleteClicked() {
        deleteMemo(mId);
        finish();
    }

    private void deleteMemo(int id) {
        SQLiteDatabase db = mMemoDB.getWritableDatabase();

        String where = MemoDB.Columns._ID + "=?";
        String[] args = { String.valueOf(id) };

        int count = db.delete(MemoDB.TABLE_NAME, where, args);
        if (count == 0) {
            Log.v("Edit", "Failed to delete");
        }

        db.close();
    }
}
