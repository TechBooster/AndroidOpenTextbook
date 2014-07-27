
package jp.androidopentextbook.sensoractivity;

import jp.androidopentextbook.sensor.R;
import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.TextView;

public class PressureActivity extends Activity implements SensorEventListener {
    @SuppressWarnings("unused")
    private static final String TAG = PressureActivity.class.getSimpleName();
    private final PressureActivity self = this;

    private SensorManager mSensorManager;
    private TextView mSensor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.pressure_view);
        setTitle("Pressure Sensor");

        // SensorManagerのインスタンスを取得する
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mSensor = (TextView) findViewById(R.id.sensor_0_text);
    }

    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager.registerListener(this,
                mSensorManager.getDefaultSensor(Sensor.TYPE_PRESSURE),
                SensorManager.SENSOR_DELAY_UI);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mSensorManager.unregisterListener(this);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        float data = event.values[0];

        mSensor.setText(String.valueOf(data));
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // TODO Auto-generated method stub

    }
}
