package jp.androidopentextbook.storage.sqlitesample.memo;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.provider.BaseColumns;

/**
 * SQLiteOpenHelper for Memo database
 */
public class MemoDB extends SQLiteOpenHelper {
    private static final String DB_NAME = "memo.db";
    private static final int DB_VERSION = 1;

    public static final String TABLE_NAME = "memo";
    private static final String SQL_CREATE_TABLE =
            "CREATE TABLE " + TABLE_NAME + "(" +
            Columns._ID + " INTEGER primary key autoincrement," +
            Columns.MEMO + " TEXT," +
            Columns.CREATE_TIME + " INTEGER," +
            Columns.UPDATE_TIME + " INTEGER)";

    public interface Columns extends BaseColumns {
        public static final String MEMO = "memo";
        public static final String CREATE_TIME = "create_time";
        public static final String UPDATE_TIME = "update_time";
    }

    public MemoDB(Context context) {
        super(context, DB_NAME, null, DB_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(SQL_CREATE_TABLE);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {

    }
}
