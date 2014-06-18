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


=== センサーの取得
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

基本的な流れは以上になりますが、一連の実装はだいたい以下のようになります。

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
    mSensorManager.unregisterListener(this);
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

@<list>{register}の「SensorManager.SENSOR_DELAY_NORMAL」はセンサーの取得の遅延設定値です。
この設定値は以下を指定することが出来ます。使用する目的によって設定します。
//image[sensor-01-delay][センサーの遅延設定]{
//}


Base sensor
Composite sensor
Batching sensor
Motion Sensors
Position Sensor
Environment Sensors

https://source.android.com/devices/sensors/composite_sensors.html
http://dev.classmethod.jp/smartphone/android/android-google-play-services-location-api-current-location/

GPSはGoogle Play Serviceを使用する体で

== 搭載されたセンサーの取得
 * 実機のセンサーの取得をするサンプルアプリ

=== センサー値を取得してみよう
 * 実測アプリ

== GPSを使う
 * Google Play Services

#@# SIMはない予定・WiMAXはある
#@# Kitkat 4.4
#@# Windows8.1
#@# ですます調
#@# 1人称は使わない方向：筆者