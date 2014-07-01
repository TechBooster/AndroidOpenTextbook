package com.techbooster.location;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesClient.ConnectionCallbacks;
import com.google.android.gms.common.GooglePlayServicesClient.OnConnectionFailedListener;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.location.LocationClient;

import android.app.Activity;
import android.content.Intent;
import android.location.Location;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity implements ConnectionCallbacks, OnConnectionFailedListener {

	private LocationClient mLocationClient;
	private Location mLoc;
    private TextView[] mLocation = new TextView[3];
    private Button mMapBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        // Google Play ServiceKが実装されているか確認
        final int result = GooglePlayServicesUtil.isGooglePlayServicesAvailable(this);
        if (result != ConnectionResult.SUCCESS) {
            Toast.makeText(this, "Google Play service is not available (status=" + result + ")", Toast.LENGTH_LONG).show();
            finish();
        }

        mLocationClient = new LocationClient(this, this, this);  
        mLocation[0] = (TextView) findViewById(R.id.lat_text);
        mLocation[1] = (TextView) findViewById(R.id.lon_text);
        mMapBtn = (Button) findViewById(R.id.map);
        
        mMapBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// IntentでGoogle Mapを呼び出す
				Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse("geo:" + mLoc.getLatitude() + "," + mLoc.getLongitude()));
				startActivity(intent);
			}
		});
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
		// Google Play Servicesとの切断
		if (mLocationClient != null) {
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

        // 位置情報の取得
        mLoc = mLocationClient.getLastLocation();
        Log.d("LOCATION", "LAT: " + mLoc.getLatitude());
        Log.d("LOCATION", "LON: " + mLoc.getLongitude());
        
        mLocation[0].setText(String.valueOf(mLoc.getLatitude()));
        mLocation[1].setText(String.valueOf(mLoc.getLongitude()));
    }

	@Override
	public void onDisconnected() {
		Toast.makeText(this, "Disconnected", Toast.LENGTH_LONG).show();
	}
}