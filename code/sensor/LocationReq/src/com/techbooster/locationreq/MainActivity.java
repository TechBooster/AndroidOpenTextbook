
package com.techbooster.locationreq;

import java.text.SimpleDateFormat;
import java.util.Date;

import android.app.Activity;
import android.location.Location;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesClient.ConnectionCallbacks;
import com.google.android.gms.common.GooglePlayServicesClient.OnConnectionFailedListener;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.location.LocationClient;
import com.google.android.gms.location.LocationListener;
import com.google.android.gms.location.LocationRequest;

public class MainActivity extends Activity implements ConnectionCallbacks,
        OnConnectionFailedListener, LocationListener {

    private LocationClient mLocationClient;
    private TextView[] mLocation = new TextView[7];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Google Play ServiceKが実装されているか確認
        final int result = GooglePlayServicesUtil.isGooglePlayServicesAvailable(this);
        if (result != ConnectionResult.SUCCESS) {
            Toast.makeText(this, "Google Play service is not available (status=" + result + ")",
                    Toast.LENGTH_LONG).show();
            finish();
        }

        mLocationClient = new LocationClient(this, this, this);
        mLocation[0] = (TextView) findViewById(R.id.lat_text);
        mLocation[1] = (TextView) findViewById(R.id.lon_text);
        mLocation[2] = (TextView) findViewById(R.id.altitude_text);
        mLocation[3] = (TextView) findViewById(R.id.speed_text);
        mLocation[4] = (TextView) findViewById(R.id.time_text);
        mLocation[5] = (TextView) findViewById(R.id.accuracy_text);
        mLocation[6] = (TextView) findViewById(R.id.bearing_text);
    }

    @Override
    protected void onResume() {
        super.onResume();
        // Google Play Servicesへの接続
        mLocationClient.connect();
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (mLocationClient != null) {
            // LocationListenerを解除
            mLocationClient.removeLocationUpdates(this);
            // Google Play Servicesとの切断
            mLocationClient.disconnect();
        }
    }

    @Override
    public void onConnectionFailed(ConnectionResult result) {
        // Google Play Servicesへの接続に失敗した
        Toast.makeText(this, "onConnectionFailed", Toast.LENGTH_LONG).show();
        Log.d("ERROR", result.toString());
    }

    @Override
    public void onConnected(Bundle connectionHint) {
        Toast.makeText(this, "Connected", Toast.LENGTH_LONG).show();

        // 位置情報の更新リクエスト
        LocationRequest req = LocationRequest.create();
        req.setFastestInterval(5000);
        req.setInterval(5000);
        req.setSmallestDisplacement(1);
        req.setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY);
        mLocationClient.requestLocationUpdates(req, this);
    }

    @Override
    public void onDisconnected() {
        Toast.makeText(this, "Disconnected", Toast.LENGTH_LONG).show();
    }

    @Override
    public void onLocationChanged(Location loc) {
        Toast.makeText(this, "Get Location", Toast.LENGTH_SHORT).show();

        Date date = new Date(loc.getTime());
        SimpleDateFormat sdf = new SimpleDateFormat("MM/dd: HH:mm:ss");
        mLocation[0].setText(String.valueOf(loc.getLatitude()));
        mLocation[1].setText(String.valueOf(loc.getLongitude()));
        mLocation[2].setText(String.valueOf(loc.getAltitude()));
        mLocation[3].setText(String.valueOf(loc.getSpeed()));
        mLocation[4].setText(sdf.format(date));
        mLocation[5].setText(String.valueOf(loc.getAccuracy()));
        mLocation[6].setText(String.valueOf(loc.getBearing()));
    }
}
