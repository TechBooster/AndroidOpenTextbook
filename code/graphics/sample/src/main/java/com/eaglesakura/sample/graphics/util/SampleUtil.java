package com.eaglesakura.sample.graphics.util;

import android.app.Activity;
import android.os.Handler;
import android.os.Looper;
import android.widget.TextView;

import com.eaglesakura.sample.graphics.R;


/**
 * サンプルコード用のUtil
 */
public class SampleUtil {
    /**
     * デバッグ用テキストを設定する
     *
     * @param activity
     * @param message
     */
    public static void setDebugText(final Activity activity, final String message) {

        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                TextView view = (TextView) activity.findViewById(R.id.debug_message);
                if (view == null) {
                    return;
                }


                view.setText(message != null ? message : null);
            }
        });
    }
}
