
package com.techbooster.sensor;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.techbooster.sensoractivity.AccelerometerActivity;
import com.techbooster.sensoractivity.GravityActivity;
import com.techbooster.sensoractivity.GyroscopeActivity;
import com.techbooster.sensoractivity.LightActivity;
import com.techbooster.sensoractivity.MagneticActivity;
import com.techbooster.sensoractivity.PressureActivity;
import com.techbooster.sensoractivity.ProximityActivity;
import com.techbooster.sensoractivity.RotationVectorActivity;

public class MainActivity extends Activity {
    private static final String TAG = DeviceViewActivity.class.getSimpleName();

    private SensorManager mSensorManager;
    private List<Sensor> mSensors;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // SensorManagerのインスタンスを取得する
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
    }

    @Override
    protected void onResume() {
        super.onResume();

        // センサーのオブジェクトリストを取得する
        mSensors = mSensorManager.getSensorList(Sensor.TYPE_ALL);
        // ListViewで表示するために登録
        ArrayList<String> list = new ArrayList<String>();
        for (Sensor sensor : mSensors) {
            list.add(getTypeString(sensor.getType()));
        }

        ListView listView = (ListView) findViewById(R.id.ListView);
        // android.R.layout.simple_list_item_1はAndroidで既に定義されているリストアイテムのレイアウト
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1, list);

        listView.setAdapter(adapter);

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Sensor sensor = mSensors.get(position);
                startActivity(setSensorInfo(sensor));
            }
        });
    }

    private Intent setSensorInfo(Sensor sensor) {
        Intent intent = new Intent();
        intent.setClass(this, DeviceViewActivity.class);

        SensorInfo sensorInfo = new SensorInfo();
        sensorInfo.setFifoMaxEventCount(sensor.getFifoMaxEventCount());
        sensorInfo.setFifoReservedEventCount(sensor.getFifoReservedEventCount());
        sensorInfo.setMaximumRange(sensor.getMaximumRange());
        sensorInfo.setMinDelay(sensor.getMinDelay());
        sensorInfo.setName(sensor.getName());
        sensorInfo.setPower(sensor.getPower());
        sensorInfo.setResolution(sensor.getResolution());
        sensorInfo.setType(sensor.getType());
        sensorInfo.setToString(sensor.toString());
        sensorInfo.setVendor(sensor.getVendor());
        sensorInfo.setVersion(sensor.getVersion());
        sensorInfo.setActivityName(getActivityName(sensor.getType()));
        intent.putExtra(SENSOR_INFO, sensorInfo);
        return intent;
    }

    private String getTypeString(final int type) {
        switch (type) {
            case Sensor.TYPE_ACCELEROMETER:
                return TYPE_ACCELEROMETER;
            case Sensor.TYPE_AMBIENT_TEMPERATURE:
                return TYPE_AMBIENT_TEMPERATURE;
            case Sensor.TYPE_GAME_ROTATION_VECTOR:
                return TYPE_GAME_ROTATION_VECTOR;
            case Sensor.TYPE_GEOMAGNETIC_ROTATION_VECTOR:
                return TYPE_GEOMAGNETIC_ROTATION_VECTOR;
            case Sensor.TYPE_GRAVITY:
                return TYPE_GRAVITY;
            case Sensor.TYPE_GYROSCOPE:
                return TYPE_GYROSCOPE;
            case Sensor.TYPE_GYROSCOPE_UNCALIBRATED:
                return TYPE_GYROSCOPE_UNCALIBRATED;
            case Sensor.TYPE_LIGHT:
                return TYPE_LIGHT;
            case Sensor.TYPE_LINEAR_ACCELERATION:
                return TYPE_LINEAR_ACCELERATION;
            case Sensor.TYPE_MAGNETIC_FIELD:
                return TYPE_MAGNETIC_FIELD;
            case Sensor.TYPE_MAGNETIC_FIELD_UNCALIBRATED:
                return TYPE_MAGNETIC_FIELD_UNCALIBRATED;
            case Sensor.TYPE_ORIENTATION:
                return TYPE_ORIENTATION;
            case Sensor.TYPE_PRESSURE:
                return TYPE_PRESSURE;
            case Sensor.TYPE_PROXIMITY:
                return TYPE_PROXIMITY;
            case Sensor.TYPE_RELATIVE_HUMIDITY:
                return TYPE_RELATIVE_HUMIDITY;
            case Sensor.TYPE_ROTATION_VECTOR:
                return TYPE_ROTATION_VECTOR;
            case Sensor.TYPE_SIGNIFICANT_MOTION:
                return TYPE_SIGNIFICANT_MOTION;
            case Sensor.TYPE_STEP_COUNTER:
                return TYPE_STEP_COUNTER;
            case Sensor.TYPE_STEP_DETECTOR:
                return TYPE_STEP_DETECTOR;
            case Sensor.TYPE_TEMPERATURE:
                return TYPE_TEMPERATURE;
            default:
                return TYPE_UNKNOWN;
        }
    }

    private String getActivityName(int type) {
        switch (type) {
            case Sensor.TYPE_ACCELEROMETER:
                return AccelerometerActivity.class.getCanonicalName();
            case Sensor.TYPE_AMBIENT_TEMPERATURE:
                return null;
            case Sensor.TYPE_GAME_ROTATION_VECTOR:
                return null;
            case Sensor.TYPE_GEOMAGNETIC_ROTATION_VECTOR:
                return null;
            case Sensor.TYPE_GRAVITY:
                return GravityActivity.class.getCanonicalName();
            case Sensor.TYPE_GYROSCOPE:
                return GyroscopeActivity.class.getCanonicalName();
            case Sensor.TYPE_GYROSCOPE_UNCALIBRATED:
                return null;
            case Sensor.TYPE_LIGHT:
                return LightActivity.class.getCanonicalName();
            case Sensor.TYPE_LINEAR_ACCELERATION:
                return null;
            case Sensor.TYPE_MAGNETIC_FIELD:
                return MagneticActivity.class.getCanonicalName();
            case Sensor.TYPE_MAGNETIC_FIELD_UNCALIBRATED:
                return null;
            case Sensor.TYPE_ORIENTATION:
                return null;
            case Sensor.TYPE_PRESSURE:
                return PressureActivity.class.getCanonicalName();
            case Sensor.TYPE_PROXIMITY:
                return ProximityActivity.class.getCanonicalName();
            case Sensor.TYPE_RELATIVE_HUMIDITY:
                return null;
            case Sensor.TYPE_ROTATION_VECTOR:
                return RotationVectorActivity.class.getCanonicalName();
            case Sensor.TYPE_SIGNIFICANT_MOTION:
                return null;
            case Sensor.TYPE_STEP_COUNTER:
                return null;
            case Sensor.TYPE_STEP_DETECTOR:
                return null;
            case Sensor.TYPE_TEMPERATURE:
                return null;
            default:
                return null;
        }
    }

    private static final String TYPE_ACCELEROMETER = "TYPE_ACCELEROMETER";
    private static final String TYPE_AMBIENT_TEMPERATURE = "TYPE_AMBIENT_TEMPERATURE";
    private static final String TYPE_GAME_ROTATION_VECTOR = "TYPE_GAME_ROTATION_VECTOR";
    private static final String TYPE_GEOMAGNETIC_ROTATION_VECTOR = "TYPE_GEOMAGNETIC_ROTATION_VECTOR";
    private static final String TYPE_GRAVITY = "TYPE_GRAVITY";
    private static final String TYPE_GYROSCOPE = "TYPE_GYROSCOPE";
    private static final String TYPE_GYROSCOPE_UNCALIBRATED = "TYPE_GYROSCOPE_UNCALIBRATED";
    private static final String TYPE_LIGHT = "TYPE_LIGHT";
    private static final String TYPE_LINEAR_ACCELERATION = "TYPE_LINEAR_ACCELERATION";
    private static final String TYPE_MAGNETIC_FIELD = "TYPE_MAGNETIC_FIELD";
    private static final String TYPE_MAGNETIC_FIELD_UNCALIBRATED = "TYPE_MAGNETIC_FIELD_UNCALIBRATED";
    private static final String TYPE_ORIENTATION = "TYPE_ORIENTATION";
    private static final String TYPE_PRESSURE = "TYPE_PRESSURE";
    private static final String TYPE_PROXIMITY = "TYPE_PROXIMITY";
    private static final String TYPE_RELATIVE_HUMIDITY = "TYPE_RELATIVE_HUMIDITY";
    private static final String TYPE_ROTATION_VECTOR = "TYPE_ROTATION_VECTOR";
    private static final String TYPE_SIGNIFICANT_MOTION = "TYPE_SIGNIFICANT_MOTION";
    private static final String TYPE_STEP_COUNTER = "TYPE_STEP_COUNTER";
    private static final String TYPE_STEP_DETECTOR = "TYPE_STEP_DETECTOR";
    private static final String TYPE_TEMPERATURE = "TYPE_TEMPERATURE";
    private static final String TYPE_UNKNOWN = "TYPE_UNKNOWN";

    public static final String SENSOR_INFO = "SENSOR_INFO";
}
