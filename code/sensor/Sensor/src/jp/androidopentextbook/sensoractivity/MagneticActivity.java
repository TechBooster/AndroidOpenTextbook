
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

public class MagneticActivity extends Activity implements SensorEventListener {
    @SuppressWarnings("unused")
    private static final String TAG = MagneticActivity.class.getSimpleName();
    private final MagneticActivity self = this;

    private SensorManager mSensorManager;
    private TextView[] mSensor = new TextView[3];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.accelerometer_view);
        setTitle("Magnetic Sensor");

        // SensorManagerのインスタンスを取得する
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mSensor[0] = (TextView) findViewById(R.id.sensor_0_text);
        mSensor[1] = (TextView) findViewById(R.id.sensor_1_text);
        mSensor[2] = (TextView) findViewById(R.id.sensor_2_text);
    }

    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager.registerListener(this,
                mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD),
                SensorManager.SENSOR_DELAY_UI);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mSensorManager.unregisterListener(this);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        float[] data = new float[3];

        data = event.values.clone();

        mSensor[0].setText(String.valueOf(data[0]));
        mSensor[1].setText(String.valueOf(data[1]));
        mSensor[2].setText(String.valueOf(data[2]));
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // TODO Auto-generated method stub

    }
}
