
package jp.androidopentextbook.sensor;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

/**
 * Created by tomo on 2014/05/08.
 */
public class DeviceViewActivity extends Activity {
    private static final String TAG = DeviceViewActivity.class.getSimpleName();
    private final DeviceViewActivity self = this;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.device_view);

        Intent intent = getIntent();
        final SensorInfo sensorInfo = intent.getParcelableExtra(MainActivity.SENSOR_INFO);

        // ListViewで表示するために登録
        ArrayList<String> list = new ArrayList<String>();
        list.add("MaxFifo:   " + sensorInfo.getFifoMaxEventCount());
        list.add("ReservedFifo:   " + sensorInfo.getFifoReservedEventCount());
        list.add("MaxRange:   " + sensorInfo.getMaximumRange());
        list.add("MinDelay:   " + sensorInfo.getMinDelay());
        list.add("Name:       " + sensorInfo.getName());
        list.add("Power:      " + sensorInfo.getPower());
        list.add("Resolution: " + sensorInfo.getResolution());
        list.add("Vendor:     " + sensorInfo.getVendor());
        list.add("Version:    " + sensorInfo.getVersion());

        ListView listView = (ListView) findViewById(R.id.ListView);
        findViewById(R.id.start_button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (sensorInfo.getActivityName() == null) {
                    Toast.makeText(self, "Not Implemented...", Toast.LENGTH_SHORT).show();
                    return;
                }

                Intent intent = new Intent();
                intent.setClassName(self, sensorInfo.getActivityName());
                startActivity(intent);
            }
        });

        // android.R.layout.simple_list_item_1はAndroidで既に定義されているリストアイテムのレイアウト
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1, list);

        listView.setAdapter(adapter);
    }
}
