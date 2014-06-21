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
//list[sensormanager][センサーマネージャの取得]{
  private SensorManager mSensorManager;
    ...
  mSensorManager = (SensorManager)getSystemService(Context.SENSOR_SERVICE);
//}

例として、Acceleration（加速度センサー）を取得します。
//list[type][センサーの取得]{
  mAcceleration = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
//}

次にセンサーを有効にします。
//list[register][センサーの有効化]{
  mSensorManager.registarListener(this, mAcceleration, Sensor.SENSOR_DELAY_NORMAL);
//}

これで、該当するActivityにSensorEventListenerが設定されていれば、イベントリスナー
が呼ばれるようになります。

リスナーは2つ用意します。
//list[listener][イベントリスナー]{
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

センサーの有効化と無効化があるのは、必要な時のみセンサー情報を使用するようにするためです。
センサー情報をずっと使用すると電池を消費してしまうためです。そのため、たいていのアプリで
は、onResume()でセンサーの取得を有効化し、onPause()で無効化するパターンとなります。も
ちろん、これが絶対ではなくServiceでバックグラウンドで取得し続けるなどの使用方法もあるので
アプリに応じて、電池の消費量をできるだけ抑えるように設定することになります。

実装をまとめると以下のようになるでしょう。
//list[complete][コード例]{
public class SensorActivity extends Activity implements SensorEventListener {
    private SensorManager mSensorManager;
    private Sensor mAcceleration;
    private TextView[] mSensor = new TextView[3];

    @Override
    public final void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        // センサーマネージャの取得と使用するセンサーの設定
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mAcceleration = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        mSensor[0] = (TextView) findViewById(R.id.sensor_0_text);
        mSensor[1] = (TextView) findViewById(R.id.sensor_1_text);
        mSensor[2] = (TextView) findViewById(R.id.sensor_2_text);    
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

今回の加速度の例では以下のようになります。valuesの配列サイズは"3"になります。
また型はfloatなので注意して下さい。

 * SensorEvent.values[0] : x軸加速度
 * SensorEvent.values[1] : y軸加速度
 * SensorEvent.values[2] : z軸加速度

加速度ですから、そのままx,y,xのそれぞれの加速度が取得できます。これを
コードで取得するには以下のように上記のコードに追加すると以下のようになります。

//list[sensorchange][センサー値変化イベントリスナー]{
  @Override
  public final void onSensorChanged(SensorEvent event) {
      // センサーの値が変化した場合に呼ばれる
        float[] accell = new float[3];

        accell[0] = event.values[0];
        accell[1] = event.values[1];
        accell[2] = event.values[2];

        Log.i(TAG, "accell[X] " + accell[0]);
        Log.i(TAG, "accell[Y] " + accell[1]);
        Log.i(TAG, "accell[Z] " + accell[2]);

        mSensor[0].setText(String.valueOf(accell[0]));
        mSensor[1].setText(String.valueOf(accell[1]));
        mSensor[2].setText(String.valueOf(accell[2]));
  }
//}

実際のデータを取得した例としては以下のようにデータが取得できます。端末をほぼ自分に向けた状態では、
y軸の加速度が重力加速度の約9.8に近い値になっています。実際に動作させると目まぐるしく値が変化しま
す。これは遅延設定の値によって変化の速さが変わります。端末を振ったりすれば当然値も変化するので、
遅延設定を変えてみたりして、数値の変化の度合いを確認してみてください。

//image[sensor-01-accel][加速度センサーの取得]{
//}


=== 複数のセンサーを取得する場合

複数のセンサーを利用したい場合はどうなるでしょうか。その場合は
センサーを複数設定することで可能です。

//list[multi][複数のセンサーを使用]{
  mAcceleration = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
  mProximity = mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
//}

ただし、取得するデータを処理する場合は、下記のように場合分けして処理する必要があります。

//list[multi-listener][複数センサー利用時のイベントリスナー]{
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

== センサーのハードウェア情報の取得

端末に搭載されているセンサーをすべて列挙して取得し、センサーのハードウェア情報を取得してみます。
センサーから取得できるハードウェア情報は以下になります。

//table[sensor_hard_info][センサーのハードウェ情報]{
型	メソッド	意味
----------------
int	getFifoMaxEventCount()		センサーイベントに保管できるFIFOの段数
int	getFifoReservedEventCount()	バッチモードで使用した時のFIFO内のデータ数
float	getMaximumRange()		センサー値の最大値
int	getMinDelay()			センサー値の最小値
String	getName()			センサーの名前
float	getPower()			センサー使用時の電流消費量(mA)
float	getResolution()			センサーの解像度
int	getType()			センサーのタイプ
String	getVendor()			センサーモジュールのベンダ名
int	getVersion()			センサーモジュールのバージョン
//}


全センサーの取得には"Sensor.TYPE_ALL"を指定すると、センサーのリストとして取得できます。
ログとして表示するようにしてみます。
//list[sensor-all][センサーリストの取得]{
    // センサーのオブジェクトリストを取得する
    mSensors = mSensorManager.getSensorList(Sensor.TYPE_ALL);
    // センサーリストから個別のセンサーを取得
    ArrayList<String> list = new ArrayList<String>();
    for (Sensor sensor : mSensors) {
        Log.v("SENSOR", sensor.getFifoMaxEventCount());
        Log.v("SENSOR", sensor.getFifoReservedEventCount());
        Log.v("SENSOR", sensor.getMaximumRange());
        Log.v("SENSOR", sensor.getMinDelay());
        Log.v("SENSOR", sensor.getName());
        Log.v("SENSOR", sensor.getPower());
        Log.v("SENSOR", sensor.getResolution());
        Log.v("SENSOR", sensor.getType());
        Log.v("SENSOR", sensor.toString());
        Log.v("SENSOR", sensor.getVendor());
        Log.v("SENSOR", sensor.getVersion());
    }
//}

以下のスクリーンショットは見やすくするため、アプリ画面で表示させています。
これはNexus5でのセンサーの例です。またNexus5では18種類ものセンサーが搭載
されていることもセンサーリストの数を取得するとわかります。
//image[sensor-01-accelerometer][加速度センサー]{
//}

//image[sensor-01-proximity][近接センサー]{
//}

== センサーを使う上で気をつけること

センサー情報を取得するには、非常に簡単に取得できることがわかりました。またセンサー
のハードウェア情報を取得することもできました。しかしセンサーを使う上での注意事項と
して、以下の点に留意する必要があります。

 * 機種によって搭載されるセンサーの種類が異なる
 * 機種によって搭載されるセンサーのハードウェアが異なる
 * センサーの数値の上限下限が機種によって異なる

それぞれに対して、対応を考える必要があります。

=== 機種によってセンサーの種類が異なる
例えば、"Sensor.TYPE_STEP_COUNTER"を使ったアプリを作りたいと言う場合、
このセンサーを搭載していない機種には対応ができません。そういった場合には
対応する機種を制限することが出来ます。AndroidManifest.xmlファイルに使用
するハードウェアを設定することで、未搭載の端末に対してはGoogle Playからの
ダウンロードができないようにすることができます。

例えば、歩数計のセンサーを必須とする場合はAndroidManifest.xmlに以下の
ように追加します。
//list[featues][マニフェストへの追記]{
  <uses-feature android:name="android.hardware.sensor.stepdetector" android:required="true" />
//}


=== 機種によってセンサーのハードウェアが異なる

これは実装の段階ではあまり気にすることは実はありません。ただしiPhoneと異なり、
実際に搭載されているハードウェアは機種によって違うので留意する必要があることは
認識してください。搭載されているセンサーによっては、センサーの感度や精度が大き
く違う場合があります。

=== センサーの数値の上限下限が機種によって異なる





http://dev.classmethod.jp/smartphone/android/android-google-play-services-location-api-current-location/

GPSはGoogle Play Serviceを使用する体で



== GPSを使う
 * Google Play Services

#@# SIMはない予定・WiMAXはある
#@# Kitkat 4.4
#@# Windows8.1
#@# ですます調
#@# 1人称は使わない方向：筆者