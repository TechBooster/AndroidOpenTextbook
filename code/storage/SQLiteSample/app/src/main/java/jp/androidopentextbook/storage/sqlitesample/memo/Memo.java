package jp.androidopentextbook.storage.sqlitesample.memo;

import android.text.format.DateFormat;

/**
 * Describes Memo record
 */
public class Memo {
    public int mId;
    public String mMemo;
    public long mCreateTime;
    public long mUpdateTime;

    @Override
    public String toString() {
        return mMemo + DateFormat.format("(yyyy/MM/dd HH:mm:ss)", mCreateTime);
    }
}
