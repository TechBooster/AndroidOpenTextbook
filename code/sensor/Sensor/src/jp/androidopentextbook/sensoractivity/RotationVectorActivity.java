
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

public class RotationVectorActivity extends Activity implements SensorEventListener {
    @SuppressWarnings("unused")
    private static final String TAG = RotationVectorActivity.class.getSimpleName();
    private final RotationVectorActivity self = this;

    private SensorManager mSensorManager;
    private TextView[] mSensor = new TextView[5];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.rotation_vector_view);
        setTitle("Rotation Vector Sensor");

        // SensorManagerのインスタンスを取得する
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mSensor[0] = (TextView) findViewById(R.id.sensor_0_text);
        mSensor[1] = (TextView) findViewById(R.id.sensor_1_text);
        mSensor[2] = (TextView) findViewById(R.id.sensor_2_text);
        mSensor[3] = (TextView) findViewById(R.id.sensor_3_text);
        mSensor[4] = (TextView) findViewById(R.id.sensor_4_text);
    }

    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager.registerListener(this,
                mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR),
                SensorManager.SENSOR_DELAY_UI);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mSensorManager.unregisterListener(this);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        float[] data = new float[5];

        data = event.values.clone();

        mSensor[0].setText(String.valueOf(data[0]));
        mSensor[1].setText(String.valueOf(data[1]));
        mSensor[2].setText(String.valueOf(data[2]));
        mSensor[3].setText(String.valueOf(data[3]));
        mSensor[4].setText(String.valueOf(data[4]));
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // TODO Auto-generated method stub

    }
}
