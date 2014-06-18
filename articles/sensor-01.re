= センサー概要
//lead{
 本章ではAndroidに搭載された各種センサーの使用方法および、
 Google Play Serviceのライブラリを使って位置情報を取得する方法について説明します。
//}

== センサーの種類
AndroidではAPIレベルが上がるたびに使用可能なセンサーの種類が増えています。
現在の対応はリファレンスを参照してください@<fn>{sensor_ref}。
Androidでは多くのセンサー情報が取得できますが、実際のハードウェアがその数の
分だけ搭載されているわけではありません。ハードウェア１つで複数のセンサー
情報に対して提供することが多いです。また位置情報を取得するためのGPSセンサー
については、他のセンサーとは異なりGoogle Play Servicesで提供されるライブラリ
を使用して取得します。まずはGPS以外のセンサーから説明していきます。

//footnote[sensor_ref][http://developer.android.com/reference/android/hardware/Sensor.html]

== 搭載されるセンサーの種類
リファレンスによると、現在のAPI19で使用可能なセンサーの一覧は以下のようになります。
//image[sensor-01-list][センサー一覧]{
//}


=== センサーの取得の流れ
センサーの利用にはフレームワークが用意されているのでそれを使います。
使用するクラスは以下の通りです。

 * Sensor
 * SensorManager
 * SensorEventListener
 * SensorEvent

使い方としては、SensorManagerをシステムから取得し、使用するセンサーを指定します。
センサーイベントを受け取るように設定すると、SensorEventListenerで設定したリスナー
にSensorEventが入ってくるので、これを解析するという流れになります。では具体的に見て
行きます。

まずはSensorManagerをシステムから取得します。
//list[sensormanager][]{
  private SensorManager mSensorManager;
    ...
  mSensorManager = (SensorManager)getSystemService(Context.SENSOR_SERVICE);
//}

例として、Acceleration（加速度センサー）を取得します。
//list[type][]{
  mAcceleration = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
//}

次にセンサーを有効にします。
//list[register][]{
  mSensorManager.registarListener(this, mAcceleration, Sensor.SENSOR_DELAY_NORMAL);
//}

これで、該当するActivityにSensorEventListenerが設定されていれば、イベントリスナー
が呼ばれるようになります。

リスナーは2つ用意します。
//list[listener][]{
  @Override
  public final void onAccuracyChanged(Sensor sensor, int accuracy) {
      // TODO
  }

  @Override
  public final void onSensorChanged(SensorEvent event) {
      // TODO
  }
//}

基本的な流れは以上になりますが、実際に使用する場合の実装はだいたい以下のようになります。

 * onCreate()もしくはonResume()でSensorManagerを取得
 * onResume()でrsgisterListnerで有効化
 * onPause()でunregisterListenerで無効化

実装をまとめると以下のようになるでしょう。
//list[complete][]{
public class SensorActivity extends Activity implements SensorEventListener {
    private SensorManager mSensorManager;
    private Sensor mAcceleration;

    @Override
    public final void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        // センサーマネージャの取得と使用するセンサーの設定
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mAcceleration = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
    }

    @Override
    protected void onResume() {
        super.onResume();
        // センサーを有効にする
        mSensorManager.registerListener(this, mAcceleration, SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    protected void onPause() {
        super.onPause();
        // センサーを無効にする
        if (mSensorManager != null) {
            mSensorManager.unregisterListener(this);          
        }
    }

    @Override
    public final void onAccuracyChanged(Sensor sensor, int accuracy) {
        // センサーの精度が変化した場合に呼ばれる
    }

    @Override
    public final void onSensorChanged(SensorEvent event) {
        // センサーの値が変化した場合に呼ばれる
    }
}
//}

=== センサー取得の遅延設定

@<list>{register}の「SensorManager.SENSOR_DELAY_NORMAL」はセンサーの取得の遅延設定値です。
この設定値は使用する目的によって設定します。
//image[sensor-01-delay][センサーの遅延設定]{
//}

=== 実際のセンサー値を取得する

それでは実際のセンサーの値を取得してみてみます。センサーのデータはリスナー
で取得します。センサーデータはSensorEventに入っています。SensorEventに
は以下のデータが入っています

 * accuray : 精度
 * sensor : センサーオブジェクト
 * timestamp : イベントの発生した時間
 * values : センサーデータ配列

センサーデータはvaluesに入っていますが、配列の数と意味は使用するセンサーによって
異なります。使用するセンサーによるデータの意味はリファレンスを見て下さい@<fn>{sensor_motion}。

//footnote[sensor_motion][http://developer.android.com/guide/topics/sensors/sensors_motion.html]

今回の加速度の例では以下のようになります。また型はfloatなので注意して下さい。

 * SensorEvent.values[0] : x軸加速度
 * SensorEvent.values[1] : y軸加速度
 * SensorEvent.values[0] : z軸加速度

加速度ですから、そのままx,y,xのそれぞれの加速度が取得できます。これを
コードで取得するには以下のようになります。

//list[sensorchange][]{
  @Override
  public final void onSensorChanged(SensorEvent event) {
      // センサーの値が変化した場合に呼ばれる
      float[] accell = new float[3];

      accell[0] = event.values[0];  // x axis
      accell[1] = event.values[1];  // y axis
      accell[2] = event.values[2];  // z axis

      Log.i("ACCEL", "accell[X] " + accell[0]);
      Log.i("ACCEL", "accell[Y] " + accell[1]);
      Log.i("ACCEL", "accell[Z] " + accell[2]);
  }
//}

実際のデータを取得した例としては以下のようにデータが取得できます。
y軸の加速度が重力加速度の約9.8に近い値になっています。実際に動作
させると目まぐるしく値が変化します。これは遅延設定の値によって変化
の速さが変わります。端末を振ったりすれば当然値も変化するので試して
みてください。

//image[sensor-01-accel][加速度センサーの取得]{
//}


=== 複数のセンサーを取得する場合

複数のセンサーを利用したい場合はどうなるでしょうか。その場合は
センサーを複数設定することで可能です。

//list[murti][]{
  mAcceleration = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
  mProximity = mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
//}

ただし、取得するデータを処理する場合は、下記のように場合分けして処理する必要があります。

//list[murti-listener][]{
  @Override
  public void onSensorChanged(SensorEvent event) {
      // 値が変化したセンサーが加速度センサーだった場合
      if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
          // 処理
      }
      // 値が変化したセンサーが近接センサーだった場合
      else if (event.sensor.getType() == Sensor.TYPE_PROXIMITY) {
          // 処理
      }
  }
//}



http://dev.classmethod.jp/smartphone/android/android-google-play-services-location-api-current-location/

GPSはGoogle Play Serviceを使用する体で



== GPSを使う
 * Google Play Services

#@# SIMはない予定・WiMAXはある
#@# Kitkat 4.4
#@# Windows8.1
#@# ですます調
#@# 1人称は使わない方向：筆者