package com.techbooster.locationnew;

import android.location.Location;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.location.LocationListener;
import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationServices;

import java.text.SimpleDateFormat;
import java.util.Date;


public class MainActivity extends ActionBarActivity implements GoogleApiClient.ConnectionCallbacks,
        GoogleApiClient.OnConnectionFailedListener, LocationListener {

    private final MainActivity self = this;
    private GoogleApiClient mApiClient = null;
    private LocationRequest mLocationRequest;
    private TextView[] mLocation = new TextView[7];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mLocation[0] = (TextView) findViewById(R.id.lat_text);
        mLocation[1] = (TextView) findViewById(R.id.lon_text);
        mLocation[2] = (TextView) findViewById(R.id.altitude_text);
        mLocation[3] = (TextView) findViewById(R.id.speed_text);
        mLocation[4] = (TextView) findViewById(R.id.time_text);
        mLocation[5] = (TextView) findViewById(R.id.accuracy_text);
        mLocation[6] = (TextView) findViewById(R.id.bearing_text);

        // Google Play Service が有効かどうかチェックを行う
        if (!checkPlayServices()) {
            return;
        }

        // Create the Google API Client
        mApiClient = new GoogleApiClient.Builder(this).addConnectionCallbacks(this)
                .addApi(LocationServices.API).addOnConnectionFailedListener(this).build();

        mLocationRequest = new LocationRequest();
        mLocationRequest.setFastestInterval(5000);
        mLocationRequest.setInterval(5000);
        mLocationRequest.setSmallestDisplacement(1);
        mLocationRequest.setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY);
    }

    @Override
    public void onStart() {
        super.onStart();
        if (mApiClient != null && !mApiClient.isConnected()) {
            // Connect the Google API client
            mApiClient.connect();
        }
    }

    @Override
    protected void onStop() {
        if (mApiClient != null) {
            LocationServices.FusedLocationApi.removeLocationUpdates(mApiClient, this);
            mApiClient.disconnect();
        }
        super.onStop();
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

    @Override
    public void onConnected(Bundle bundle) {
        Toast.makeText(this, "Connected", Toast.LENGTH_LONG).show();
        LocationServices.FusedLocationApi
                .requestLocationUpdates(mApiClient, mLocationRequest, this);
    }

    @Override
    public void onConnectionSuspended(int i) {

    }

    @Override
    public void onLocationChanged(Location location) {
        Toast.makeText(this, "Get Location", Toast.LENGTH_SHORT).show();

        Date date = new Date(location.getTime());
        SimpleDateFormat sdf = new SimpleDateFormat("MM/dd: HH:mm:ss");
        mLocation[0].setText(String.valueOf(location.getLatitude()));
        mLocation[1].setText(String.valueOf(location.getLongitude()));
        mLocation[2].setText(String.valueOf(location.getAltitude()));
        mLocation[3].setText(String.valueOf(location.getSpeed()));
        mLocation[4].setText(sdf.format(date));
        mLocation[5].setText(String.valueOf(location.getAccuracy()));
        mLocation[6].setText(String.valueOf(location.getBearing()));
    }

    @Override
    public void onConnectionFailed(ConnectionResult connectionResult) {
        Toast.makeText(this, "Google Play service can't connect (status=" + connectionResult + ")",
                Toast.LENGTH_LONG).show();
        finish();
    }

    /**
     * Play Servicesが利用可能かチェックする
     * @return
     */
    private boolean checkPlayServices() {
        int resultCode = GooglePlayServicesUtil.isGooglePlayServicesAvailable(self);
        if (resultCode != ConnectionResult.SUCCESS) {
            if (GooglePlayServicesUtil.isUserRecoverableError(resultCode)) {
                if (self instanceof ActionBarActivity) {
                    GooglePlayServicesUtil.getErrorDialog(resultCode, self, 0).show();
                } else {
                    String text = GooglePlayServicesUtil.getErrorString(resultCode);
                    Toast.makeText(self, "" + text, Toast.LENGTH_SHORT).show();
                }
            } else {
                Toast.makeText(self, "Play services not available...", Toast.LENGTH_SHORT).show();
                // finish();
            }
            return false;
        }
        return true;
    }
}
