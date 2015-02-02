package jp.androidopentextbook.storage.sharedpreferences;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;


public class MainActivity extends ActionBarActivity {

    private static final String PREF_NAME = "myPref";
    private static final String KEY_NAME = "name";
    private static final String DEFAULT_NAME = "";

    private EditText mNameEdit;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mNameEdit = (EditText) findViewById(R.id.edit_name);
        String name = loadName();
        mNameEdit.setText(name);

        Button saveButton = (Button) findViewById(R.id.button_save);
        saveButton.setOnClickListener(mClickListener);
    }

    private final View.OnClickListener mClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            saveName();
            Toast.makeText(MainActivity.this, R.string.save_done, Toast.LENGTH_SHORT).show();
        }
    };

    private void saveName() {
        // Get name from EditText
        String name = mNameEdit.getText().toString();

        SharedPreferences pref = getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
        SharedPreferences.Editor edit = pref.edit();
        edit.putString(KEY_NAME, name);
        edit.apply();
        // If you want to sync preference file at this time,
        // user edit.commit() instead of edit.apply()
    }

    private String loadName() {
        SharedPreferences pref = getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
        return pref.getString(KEY_NAME, DEFAULT_NAME);
    }
}
