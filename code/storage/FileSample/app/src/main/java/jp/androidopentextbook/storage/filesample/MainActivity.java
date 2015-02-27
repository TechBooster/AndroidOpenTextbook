package jp.androidopentextbook.storage.filesample;

import android.content.Context;
import android.os.Environment;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;

import butterknife.ButterKnife;
import butterknife.InjectView;
import butterknife.OnClick;


public class MainActivity extends ActionBarActivity {

    @InjectView(R.id.edit)
    EditText mEdit;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ButterKnife.inject(this, this);
    }

    @OnClick(R.id.button_internal_load)
    void internalLoadClicked() {
        InputStream in = null;
        InputStreamReader sr = null;
        BufferedReader br = null;
        try {
            in = openFileInput("myText.txt");
            sr = new InputStreamReader(in);
            br = new BufferedReader(sr);

            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = br.readLine()) != null) {
                sb.append(line).append("\n");
            }
            mEdit.setText(sb.toString());

            Toast.makeText(this, R.string.load_done, Toast.LENGTH_SHORT).show();
        } catch (IOException e) {
            Log.e("Internal", "IO Exception " + e.getMessage(), e);
        } finally {
            try {
                if (br != null) { br.close(); }
                if (sr != null) { sr.close(); }
                if (in != null) { in.close(); }
            } catch (IOException e) {
                Log.e("Internal", "IO Exception " + e.getMessage(), e);
            }
        }
    }

    @OnClick(R.id.button_internal_save)
    void internalSaveClicked() {
        OutputStream out = null;
        OutputStreamWriter writer = null;
        BufferedWriter bw = null;
        try {
            out = openFileOutput("myText.txt", Context.MODE_PRIVATE);
            writer = new OutputStreamWriter(out);
            bw = new BufferedWriter(writer);

            String text = mEdit.getText().toString();
            bw.write(text);

            Toast.makeText(this, R.string.save_done, Toast.LENGTH_SHORT).show();

        } catch (IOException e) {
            Log.e("Internal", "IO Exception " + e.getMessage(), e);
        } finally {
            try {
                if (bw != null) { bw.close(); }
                if (writer != null) { writer.close(); }
                if (out != null) { out.close(); }
            } catch (IOException e) {
                Log.e("Internal", "IO Exception " + e.getMessage(), e);
            }
        }
    }

    @OnClick(R.id.button_external_load)
    void externalLoadClicked() {
        InputStream in = null;
        InputStreamReader sr = null;
        BufferedReader br = null;
        try {
            File foler = Environment.getExternalStoragePublicDirectory("MyDocuments");
            File file = new File(foler, "myText.txt");
            in = new FileInputStream(file);
            sr = new InputStreamReader(in);
            br = new BufferedReader(sr);

            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = br.readLine()) != null) {
                sb.append(line).append("\n");
            }
            mEdit.setText(sb.toString());

            Toast.makeText(this, R.string.load_done, Toast.LENGTH_SHORT).show();
        } catch (IOException e) {
            Log.e("External", "IO Exception " + e.getMessage(), e);
        } finally {
            try {
                if (br != null) { br.close(); }
                if (sr != null) { sr.close(); }
                if (in != null) { in.close(); }
            } catch (IOException e) {
                Log.e("External", "IO Exception " + e.getMessage(), e);
            }
        }
    }

    @OnClick(R.id.button_external_save)
    void externalSaveClicked() {
        OutputStream out = null;
        OutputStreamWriter writer = null;
        BufferedWriter bw = null;
        try {
            File foler = Environment.getExternalStoragePublicDirectory("MyDocuments");
            if (!foler.exists()) {
                boolean result = foler.mkdir();
                if (!result) {
                    return;
                }
            }
            File file = new File(foler, "myText.txt");
            out = new FileOutputStream(file);
            writer = new OutputStreamWriter(out);
            bw = new BufferedWriter(writer);

            String text = mEdit.getText().toString();
            bw.write(text);

            Toast.makeText(this, R.string.save_done, Toast.LENGTH_SHORT).show();
        } catch (IOException e) {
            Log.e("External", "IO Exception " + e.getMessage(), e);
        } finally {
            try {
                if (bw != null) { bw.close(); }
                if (writer != null) { writer.close(); }
                if (out != null) { out.close(); }
            } catch (IOException e) {
                Log.e("External", "IO Exception " + e.getMessage(), e);
            }
        }
    }

}
