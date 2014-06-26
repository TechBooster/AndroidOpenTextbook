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
リファレンスによると、現在のAPI Level 19で使用可能なセンサーの一覧は以下のようになります。
//image[sensor-01-list][センサー一覧]{
//}


== センサーの取得の流れ
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
//list[listener][イベントコールバック]{
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
 * onResume()でSensorManager#registerListner()で有効化
 * onPause()でSensorManager#unregisterListener()で無効化

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

@<list>{register}の"SensorManager.SENSOR_DELAY_NORMAL"はセンサーの取得の遅延設定値です。
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

加速度なので、そのままx,y,xのそれぞれの加速度が取得できます。これを
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

=== 傾きセンサーの取得

API Level 8移行では傾きセンサー(TYPE_ORIENTATION)は非推奨となっており、
代わりにSensorManager#getOrientationメソッドの仕様が推奨されています。

#@# TODO: 傾きセンサー

=== 複数のセンサーを取得する場合

複数のセンサーを利用したい場合はどうなるでしょうか。その場合は
センサーを複数設定することで可能です。

//list[multi-cretae][複数のセンサーを使用 onCreate()]{
  mAcceleration = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
  mProximity = mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
//}

//list[multi-resume][複数のセンサーを使用 onResume]{
@Override
protected void onResume() {
    super.onResume();
    // センサーを有効にする
    mSensorManager.registerListener(this, mAcceleration, SensorManager.SENSOR_DELAY_NORMAL);
    mSensorManager.registerListener(this, mProximity, SensorManager.SENSOR_DELAY_NORMAL);
}
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

複数のセンサーを使用する例として、位置情報を利用したARアプリなどがあります。
GPSで位置情報を取得し、その場所でどっちの方向を見ているか（地磁気センサー）、
そしてどんな仰角で端末を掲げているか（傾きセンサー）というのを組み合わせる
ことで、特定の位置に「何かが見える」などの実装が可能です。

== 各センサーの紹介

API-Level 19で規定されているセンサーをざっとですが整理してみます。

=== 加速度センサー (Acceleration sensor)

x軸、y軸、z軸のそれぞれの加速度を表します。単位は(m/s2^)
#@# TODO 図を入れる

=== 周囲温度センサー (Temperature Sensor)

端末の周囲の温度を表します単位は(℃)

=== 地磁気センサー (Geomagnetic field sensor)

x軸、y軸、z軸方向の磁気の強さを表します。単位は(μT)

=== ジャイロスコープ (Gyroscope)

x軸、y軸、z軸の回転の速度、角速度を表します。単位は(rad/s)
#@# TODO 図を入れる

=== 照度センサー (Light)

周囲の明るさを表します。単位は(lx)

=== 近接センサー (Proximity Sensor)

端末の前面との距離を表します。単位は(cm)。
ただし"near"と"far"の2値しか返さない端末もあります。

=== 気圧センサー (Pressure)

周囲の気圧を表します。単位は(hPa)

=== 相対湿度センサー (Humidity Sensor)

周囲の湿度を表します。単位は(%)

=== 回転ベクトルセンサー（地磁気影響を除外） (Game Rotation Vector Sensor)

回転ベクトルセンサーから地磁気の影響を除外したものを表します。ゲームなどに利用します。

=== 地磁気回転ベクトルセンサー (Geomagnetic Rotation Vector Sensor)

回転ベクトルセンサーとほぼ同じものですが、ジャイロスコープの代わりに地磁気センサーを使用しています。
回転ベクトルセンサーよりも精度は落ちますが低消費電力です。バックグラウンドでの動作に使われます。

=== 重力センサー (Gravity sensor)

重力加速度を表します。単位は(m/s^2)

=== 回転ベクトルセンサー (Rotation Vector Sensor)

=== ジャイロスコープ（生データ） (Uncalibrated Gyroscope)

x軸、y軸、z軸の回転の速度、角速度を温度ドリフトなどを補正しない、生データを表します。単位は(rad/s)

=== 地磁気センサー（生データ）(Uncalibrated Magnetometer)

x軸、y軸、z軸方向の磁気の強さをキャリブレーション無しの生データを表します。単位は(μT)

=== 加速度センサー（重力を除外） (Linear acceleration sensor)

x軸、y軸、z軸のそれぞれの加速度を重力加速度を差し引いて表します。単位は(m/s2^)

=== 動き検知 (Significant Motion Sensor)

端末を持ったユーザが動いたことを検知します。例えば、歩く、自転車、座る、車で移動など。
このセンサーはワンショットのトリガ起動なので、"TriggerEventListener"を使用します。

=== 歩数計 (Step Counter Sensor)

端末がリブート起動してからの歩数を表します。

=== 歩行検知 (Step Detecter Sensor)

端末を持ったユーザが歩行中であることを検出します。

=== 傾きセンサー (Orientation Sensor)

端末の傾きを検出します。単位は(度)



== センサーのハードウェア情報の取得

端末に搭載されているセンサーをすべて列挙して取得し、センサーのハードウェア情報を取得してみます。
センサーから取得できるハードウェア情報は以下になります。

//image[sensor-01-hard][センサーのハードウェ情報]{
//}

全センサーの取得には"Sensor.TYPE_ALL"を指定すると、センサーのリストとして取得できます。
ログとして表示するようにしてみたのが下記になります。
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
例えば、"Sensor.TYPE_STEP_COUNTER"を使ったアプリを作りたい場合、
このセンサーを搭載していない機種には対応ができません。そういった場合には
対応する機種を制限することが出来ます。AndroidManifest.xmlファイルに使用
するハードウェアを設定することで、未搭載の端末に対してはGoogle Playからの
ダウンロードをできないようにすることができます。

例えば、歩数計のセンサーを必須とする場合はAndroidManifest.xmlに以下の
ように追加します。
//list[featues][マニフェストへの追記]{
  <uses-feature android:name="android.hardware.sensor.stepdetector" android:required="true" />
//}


=== 機種によってセンサーのハードウェアが異なる

これは実装の段階ではあまり気にすることは実はありません。ただしiPhoneと異なり、
実際に搭載されているハードウェアは機種によって違うので、センサーの感度や精度が
大きく違う場合があります。

=== センサーの数値の上限下限が機種によって異なる

センサーのハードウェアが違えば、精度や上限・下限も異なる場合があります。センサー
の値の範囲を気にする必要がある場合もあります。例えば先程のNexus5と中華系の端末
を近接センサーで比較してみると、以下のようになっていたりします。

#@# TODO スクリーンショット

このような場合は、機種ごとに対応したり、値を正規化して相対値として
使用できるか検討する必要があります。

== センサーの種類と系統

センサーの種類は多く、APIレベルが上がるにつれて増えてきています。
ここではセンサーの種類と系統をまとめておきます。

センサーの種類をハードウェアでカテゴリで分けると

 * 基本
 * 状態
 * 生データ
 * 動き

基本は実際に搭載されているセンサを意味していて、それぞれのセンサーデータを取得します。
それ以外のセンサー値については、基本を組み合わせて生成したものと、一部生データを提供
するものとになります（もちろんハードウェアとして実装されていても構わないはずです）。
これを相関図として整理すると以下のようになります@<fn>{sensor_category}。

//image[sensor-01-category][センサーのカテゴリ分け]{
//}

こうして整理してみることで、それぞれのセンサー値がどのように合成されているかわかります。
ただし、ここにある基本のセンサーでも搭載されていない機種があるので、基本だからといって
センサー値が取得できるとは限りません。

//footnote[sensor_category][https://source.android.com/devices/sensors/index.html]

== センサーのバッチモード

センサーは非常に便利ですが、使うと消費電力が大きいのがデメリットです。そこでAndroid
では、センサーのデータを複数回分貯めこんでおき、バッチ処理で通知する仕組みがあります@<fn>{sensor_batch}。

バッチ処理が実行されるまでは、センサーは低消費電力な状態を維持できるので電池の消費を
抑えることができます。このバッチ処理の間隔は設定ができるます。

バッチモードは、スクリーンがオフの状態やシステムがスリープ中でも動作するため、
フィットネスや位置情報の追跡や監視などに有効な手段となります。Android 4.4からは
歩行検出センサーと、歩数計センサーが使用可能になっており、特にこの辺りのセンサー
を使用する場合に有効な手段となります。

//footnote[sensor_batch][https://source.android.com/devices/sensors/batching.html]

=== バッチモードの使用

バッチモードを使用するには、LocationManager#registerListenerメソッドで指定します。
通常のセンサーのメソッドにバッチ処理時間が引数で増えています。下記の例はバッチ処理として
1000 * 1000 * 10 μs = 10sに設定した場合です。

//list[sensor_batch][バッチモード設定]{
private Sensor mStepDetector;
...
boolean status = mSensorManager.registerListener(this, mStepDetector, SensorManager.SENSOR_DELAY_NORMAL, 10000);
//}
ただし、バッチ処理が可能かどうかはセンサーの対応によるので、対応している場合のみ
返り値statusにtrueが返ります。

注意点としては、バッチモードで使用されるデータバッファのFIFO(First in First Out)
はすべてのアプリで共通になるため、他のアプリで使用されてしまうと自分のアプリで思った
ように動かない。という場合があります。


== GPSセンサー概要

重要なデバイスとしてGPSセンサーがあります。GPSは"Global Positioning System"
の略で、アメリカが上げた軍事用衛星の複数からの電波を受信することで、現在位置を正確
に知ることができます。正確にはセンサーというよりはGPS受信機になります。日本で受信
可能な衛星の数は6〜10個程度ですが、位置情報は3つ以上の衛星の電波を捕捉するすること
で3次元測位が可能となります。

GPSは受信精度が高ければ、正確な位置を10m程度の誤差で測位できますが、いくつかの弱点もあり
ますので留意して下さい。

 * 高層ビルの間など空がひらけていない場合、十分な衛星を捕捉できない、あるいはマルチパス
 の影響を受けるため、都心部ではむしろ位置精度が落ちることがある
 * 衛星の捕捉と位置検出には、60秒以上の時間が掛かる場合がある

#@# コラム的な感じにする？
なお、日本では独自GPS衛星として「みちびき」@<fn>{mitibiki}の打ち上げに成功し、今後4機
での本格運用が計画されています。これは「準天頂衛星システム」といい、従来のGPSを補完する
ため、常に1機は日本上空にあって、マルチパスの影響を受けず正確な位置を測位することが可能に
なります。

//footnote[mitibiki][http://www.jaxa.jp/projects/sat/qzss/index_j.html]

== GPSを利用した位置情報取得

GPSを利用して位置情報を取得しますが、これは通常のセンサーと異なり"Google Play Services"
ライブラリを導入して、この中にあるLocationClientを使って取得します。

=== Google Play Services Libraryの導入

"Android SDK Manager"から@<b>{Google Play Services}を選択して、インストールします。
//image[sensor-01-play_service-01][Google Play Servicesのインストール 1]{
//}

"Existing Android Code into Worksapce"を選択します。
//image[sensor-01-play_service-02][Google Play Servicesのインストール 2]{
//}

#@# TODO: パスをWindowsの場合について書く
"Browse"から@<b>{google-play-services_lib}を選択し、@<b>{Copy projects into workspaceにチェック}
を入れて"Finish"ボタンを押します。
//image[sensor-01-play_service-03][Google Play Servicesのインストール 3]{
//}

正常にビルドしてエラーが無いことを確認しておきましょう。
//image[sensor-01-play_service-04][Google Play Servicesのインストール 4]{
//}

アプリを作成時のひな形作成した後、Google Play ServicesライブラリをAndroid
ライブラリとして指定することで使用が可能となります。
#@warn Google API必要だっけ？要確認
//image[sensor-01-play_service-05][Google Play Servicesのインストール 5]{
//}

これでGoogle Play Servicesライブラリの導入は完了です。


== GPSの取得の流れ

GPSを利用した位置情報取得にもフレームワークが用意されているのでそれを使います。
使用するクラスは以下の通りです。

 * LocationClient
 * Location

使い方としては、LocationClientを生成しておきます。LocationClientへの接続が成功
すると、位置情報はバックグラウンドで取得され、LocationClient#getLastLocationに
現在の位置情報が非同期に更新されるので、この値を使用することができます。

Google Play Servicesを使わずにGPSやWiFiを使って位置情報の取得も可能ですが、
GPSやWiFiのON/OFFを検出したり、GPSとWiFiのステータスを管理しなければならず、
かなり面倒なのが実際です。Google Play ServicesのLocationClientを使うこと
「一番良い位置情報をクレ」ということができ、GPS/WiFiの区別を実際には意識する
ことはありません。

LocationClientの取得は以下のようになります。
//list[gps_client][LocationClientの取得]{
@Override
protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    
    // Google Play Serviceが有効かどうかチェックを行う
    final int result = GooglePlayServicesUtil.isGooglePlayServicesAvailable(this);
    if (result != ConnectionResult.SUCCESS) {
        Toast.makeText(this, "Google Play service is not available (status=" + result + ")", Toast.LENGTH_LONG).show();
        finish();
    }

    // LocationClientの取得
    mLocationClient = new LocationClient(this, this, this);        
} 
//}
 
LocationClientを使用する場合には、2つのコールバックを引数に取るので、リスナーをActivityに設定しておきます。
//list[gps_listener][リスナーの設定]{
public class MainActivity extends Activity implements ConnectionCallbacks, OnConnectionFailedListener
//}

設定されるリスナーは3つで、OnConnectionFailedListenerは接続に失敗した時に呼ばれます。
//list[gps_connectfail][接続失敗時のイベントコールバック]{
@Override
public void onConnectionFailed(ConnectionResult arg0) {
  // TODO Auto-generated method stub
}
//}

ConnectionCallbacksは接続時と切断時に呼ばれます。
//list[gps_connect][接続時・切断時のイベントコールバック]{
@Override
public void onConnected(Bundle arg0) {
  // TODO Auto-generated method stub
}

@Override
public void onDisconnected() {
  // TODO Auto-generated method stub 
}
//}

実際に使用する場合の実装は、LocationAPIへの接続が必要になります。

 * onCreate()もしくはonResume()でLocationClientを取得
 * onResume()でLocationClient#connect()でGoogle Play Servicesへ接続
 * onPause()でLocationClient#disconnect()でGoogle Play Servicesと切断
 * 位置情報はLocationClient#getLastLocation()で取得

また、気をつける点としてLocationClient#connect()は非同期関数で、onConnected()
コールバックが呼ばれてからでないとLocationClient#getLastLocation()を呼ぶことが
できません。

以上をまとめると、このような実装になります。
//list[gps_sample][GPSを使うコード]{
public class MainActivity extends Activity implements ConnectionCallbacks, OnConnectionFailedListener {

    private LocationClient mLocationClient;
    private Location mLoc;

    
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
        Log.d("LOCATION", "LAT: " + loc.getLatitude());
        Log.d("LOCATION", "LON: " + loc.getLongitude());
    }

    @Override
    public void onDisconnected() {
        Toast.makeText(this, "Disconnected", Toast.LENGTH_LONG).show();
    }
}
//}

しかしまだこれでは動作しません。GPSを使うにはパーミッションが必要なのと、
Google Play Servicesライブラリを使うことを宣言する必要があります。

AndroidManifest.xmlにGoogle Play Servicesを使うための宣言をを追加します。
Google Play Servicesの宣言は<application>エレメントの内側に追加してください。
//list[play_permission][Google Play Serviceの使用宣言]{
    <meta-data android:name="com.google.android.gms.version"
       android:value="@integer/google_play_services_version" />
//}

AndroidManifest.xmlにGPSのパーミッションを追加します。
//list[gps_permission][GPSのパーミッション]{
    <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION"/>
//}

位置情報が取得した例として、アプリで表示させると下記のようになります。
//image[sensor-01-location][位置情報取得]{
//}

さて、これだけだといわゆる緯度経度の数値しか見えないので、イマイチ正しいかどうかわかりません。
ちょっと工夫して、位置をMapで表示するようにしてみます。

ボタンを配置して、リスナーをセットします。ボタンがクリックされたらIntentでGoogle Mapsを
起動します。その時に"geo:"のスキーマに緯度経度を設定します。
//list[gps_map][Google Mapで位置を確認]{
mMapBtn.setOnClickListener(new View.OnClickListener() {
    @Override
    public void onClick(View v) {
        // IntentでGoogle Mapを呼び出す
        Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse("geo:" + mLoc.getLatitude() + "," + mLoc.getLongitude()));
        startActivity(intent);
    }
});
//}






#@# ですます調
#@# 1人称は使わない方向：筆者