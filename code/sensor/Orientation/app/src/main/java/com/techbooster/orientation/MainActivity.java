
package com.techbooster.orientation;

import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends Activity implements SensorEventListener {

    private SensorManager mSensorManager;
    private TextView[] mSensor = new TextView[3];

    // センサー値
    private static final int AXIS_NUM = 3;
    float[] mOrientation = new float[AXIS_NUM];
    float[] mGeomagnetic = new float[AXIS_NUM];
    float[] mAcceleration = new float[AXIS_NUM];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mSensor[0] = (TextView) findViewById(R.id.sensor_0_text);
        mSensor[1] = (TextView) findViewById(R.id.sensor_1_text);
        mSensor[2] = (TextView) findViewById(R.id.sensor_2_text);

        // SensorManagerのインスタンスを取得する
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
    }

    @Override
    protected void onResume() {
        super.onResume();

        // センサの取得
        List<Sensor> sensors = mSensorManager.getSensorList(Sensor.TYPE_ALL);

        // センサマネージャへリスナーを登録
        for (Sensor sensor : sensors) {
            if (sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD) {
                mSensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_UI);
            }

            if (sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
                mSensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_UI);
            }
        }
    }

    @Override
    protected void onPause() {
        super.onPause();

        // センサーマネージャのリスナ登録破棄
        if (mSensorManager != null) {
            mSensorManager.unregisterListener(this);
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // TODO Auto-generated method stub

    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.accuracy == SensorManager.SENSOR_STATUS_UNRELIABLE)
            return;

        switch (event.sensor.getType()) {
            case Sensor.TYPE_MAGNETIC_FIELD:
                mGeomagnetic = event.values.clone();
                break;
            case Sensor.TYPE_ACCELEROMETER:
                mAcceleration = event.values.clone();
                break;
        }

        if (mGeomagnetic != null && mAcceleration != null) {

            // 回転行列
            final int MATRIX_SIZE = 16;
            float[] inR = new float[MATRIX_SIZE];
            float[] outR = new float[MATRIX_SIZE];
            float[] I = new float[MATRIX_SIZE];

            SensorManager.getRotationMatrix(inR, I, mAcceleration, mGeomagnetic);
            // Activityの表示が縦固定で、端末表面が自分を向いてる場合
            SensorManager.remapCoordinateSystem(inR, SensorManager.AXIS_X, SensorManager.AXIS_Z,
                    outR);
            SensorManager.getOrientation(outR, mOrientation);

//@formatter:off
//            Log.v("Orientation", String.valueOf(radianToDegree(mOrientation[0])) + ", " + // Z軸方向,azimuth
//                    String.valueOf(radianToDegree(mOrientation[1])) + ", " + // X軸方向,pitch
//                    String.valueOf(radianToDegree(mOrientation[2]))); // Y軸方向,roll
//@formatter:on
            mSensor[0].setText(String.valueOf(radianToDegree(mOrientation[0])));
            mSensor[1].setText(String.valueOf(radianToDegree(mOrientation[1])));
            mSensor[2].setText(String.valueOf(radianToDegree(mOrientation[2])));
        }
    }

    private int radianToDegree(float rad) {
        return (int) Math.floor(Math.toDegrees(rad));
    }
}
