package com.techbooster.axissensor;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;


public class MainActivity extends ActionBarActivity implements SensorEventListener {
    private static final String TAG = MainActivity.class.getSimpleName();

    // テキスト表示部
    private TextView mXaxisValue;
    private TextView mYaxisValue;
    private TextView mZaxisValue;

    // センサーマネージャ
    private SensorManager mSensorManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // テキストビュー取得
        mXaxisValue = (TextView) findViewById(R.id.x_axis_value);
        mYaxisValue = (TextView) findViewById(R.id.y_axis_value);
        mZaxisValue = (TextView) findViewById(R.id.z_axis_value);

        // SensorManagerを取得する
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
    }

    @Override
    protected void onResume() {
        super.onResume();

        // 加速度センサーを取得する
        Sensor accelaration = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        // 加速度センサーを有効にする
        mSensorManager.registerListener(this, accelaration, SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    protected void onPause() {
        super.onPause();
        // 加速度センサーを無効にする
        if (mSensorManager != null) {
            mSensorManager.unregisterListener(this);
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        // センサーの値が変化した場合に呼ばれる
        float[] accell = new float[3];

        // センサーの値を取得
        accell[0] = event.values[0];
        accell[1] = event.values[1];
        accell[2] = event.values[2];

        // デバッグ表示
        Log.i(TAG, "accell[X] " + accell[0]);
        Log.i(TAG, "accell[Y] " + accell[1]);
        Log.i(TAG, "accell[Z] " + accell[2]);

        // 画面に表示する
        mXaxisValue.setText(String.valueOf(accell[0]));
        mYaxisValue.setText(String.valueOf(accell[1]));
        mZaxisValue.setText(String.valueOf(accell[2]));
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
