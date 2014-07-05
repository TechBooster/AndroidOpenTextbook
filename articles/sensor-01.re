= センサー概要
//lead{
 本章ではAndroidに搭載された各種センサーの使用方法および、
 Google Play Serviceのライブラリを使って位置情報を取得する方法について説明します。
//}

== センサーとは

一般にセンサーと言うと、加速度センサーやGPSセンサーというのを思い浮かべると思います。
こういったセンサーは身近にも使われていて、車のエアバッグが開くのは加速度センサーである
程度のマイナス加速度が発生した瞬間にエアバッグを膨らませるようになっています。またカー
ナビには当然GPSが搭載されています。暗い廊下などで節電のために蛍光灯などが切ってあって
も、人が通ると自動的に点くようになっているのは、人感センサーを使っています。このように
センサーは意外と身近な所で使用されています。最近のセンサーはMEMS@<fn>{mems}で構成され、
超小型化されているのが特徴です。
//image[sensor-01-mems][半導体プロセスを用いて作成されたギア（左下）とダニ（右上）の電子顕微鏡写真(Wikipediaより)]{
//}

Androidには複数のハードウェアセンサーが搭載されていますが、さらにそれらをまとめた
フュージョンセンサーというソフトウェアセンサーも利用できるようになっています。

//footnote[mems][MEMS(Micro Electro Mechanical Systems)とは、電子回路やセンサー、アクチュエータなどを複合化したデバイスとなっていて、一つのデバイス内部に複数の機能を要することが可能]


== Androidで使用可能なセンサー

AndroidではAPIレベルが上がるたびに使用可能なセンサーの種類が増えています。
現在の対応はリファレンスを参照してください@<fn>{sensor_ref}。
Androidでは多くのセンサー情報が取得できますが、実際のハードウェアがその数の
分だけ搭載されているわけではありません。ハードウェア１つで複数のセンサー
情報に対して提供することが多いです。また位置情報を取得するためのGPSセンサー
については、他のセンサーとは異なりGoogle Play Servicesで提供されるライブラリ
を使用して取得します。まずはGPS以外のセンサーから説明していきます。

//footnote[sensor_ref][@<href>{http://developer.android.com/reference/android/hardware/Sensor.html}]

リファレンスによると、現在のAPI Level 19で使用可能なセンサーの一覧は以下のようになります。
//image[sensor-01-list][センサー一覧]{
//}

== センサー情報の取得

=== センサー情報取得フレームワーク

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
Sensor acceleration = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
//}

次にセンサーを有効にします。
//list[register][センサーの有効化]{
mSensorManager.registarListener(this, acceleration, Sensor.SENSOR_DELAY_NORMAL);
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

 * onCreateもしくはonResumeでSensorManagerを取得
 * onResumeでSensorManager#getDeafauletSensorでセンサーを取得し、SensorManager#registerListnerで有効化
 * onPauseでSensorManager#unregisterListenerで無効化

センサーの有効化と無効化があるのは、必要な時のみセンサー情報を使用するようにするためです。
センサー情報をずっと使用すると電池を消費してしまうためです。そのため、たいていのアプリで
は、onResumeでセンサーの取得を有効化し、onPauseで無効化するパターンとなります。も
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

        // センサーマネージャの取得
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mSensor[0] = (TextView) findViewById(R.id.sensor_0_text);
        mSensor[1] = (TextView) findViewById(R.id.sensor_1_text);
        mSensor[2] = (TextView) findViewById(R.id.sensor_2_text);    
    }

    @Override
    protected void onResume() {
        super.onResume();

        // 使用するセンサーの設定
        Sensor acceleration = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        // センサーを有効にする
        mSensorManager.registerListener(this, acceleration, SensorManager.SENSOR_DELAY_NORMAL);
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

=== センサー値の取得

それでは実際のセンサーの値を取得してみてみます。センサーのデータはリスナー
で取得します。センサーデータはSensorEventに入っています。SensorEventに
は以下のデータが入っています

 * accuray : 精度
 * sensor : センサーオブジェクト
 * timestamp : イベントの発生した時間
 * values : センサーデータ配列

センサーデータはvaluesに入っていますが、配列の数と意味は使用するセンサーによって
異なります。使用するセンサーによるデータの意味はリファレンスを見て下さい@<fn>{sensor_motion}。

//footnote[sensor_motion][@<href>{http://developer.android.com/guide/topics/sensors/sensors_motion.html}]

今回の加速度の例では以下のようになります。valuesの配列サイズは"3"になります。
また型はfloatなので注意して下さい。

 * SensorEvent.values[0] : x軸加速度
 * SensorEvent.values[1] : y軸加速度
 * SensorEvent.values[2] : z軸加速度

加速度なので、そのままx,y,xのそれぞれの加速度が取得できます。これを
コードで取得するには以下のように上記のコードに追加すると以下のようになります。

//list[sensorchange][センサー値変化イベントコールバック]{
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

=== 複数センサーを取得

複数のセンサーを利用したい場合はどうなるでしょうか。その場合はセンサーを複数設定する
ことで可能です。以下の例は"TYPE_ALL"で一旦すべてのセンサーをリストで取得し、リスト
から使用するものだけを選択して登録しています。

//list[multi-resume][複数のセンサーを使用 onResume]{
@Override
protected void onResume() {
    super.onResume();
    // センサの取得
    List<Sensor> sensors = mSensorManager.getSensorList(Sensor.TYPE_ALL);
    // センサマネージャへリスナーを登録
    for (Sensor sensor : sensors) {
        if (sensor.getType() == Sensor.TYPE_PROXIMITY) {
            mSensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_UI);
        }

        if (sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            mSensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_UI);
        }
    }
}
//}

ただし、取得するデータを処理する場合は、下記のように場合分けして処理する必要があります。

//list[multi-listener][複数センサー利用時のイベントコールバック]{
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

複数のセンサーを使用する例として、次に説明する傾きセンサーで方位や傾きを
利用することで、端末の中のオブジェクトを様々な角度から見られるようにしたり、
さらに位置情報とカメラを組みわせて、特定の場所にオブジェクトを出現させるような
ARアプリの作成ができるようになります。

=== 傾きセンサーの取得

API Level 8以降では傾きセンサー(TYPE_ORIENTATION)は非推奨となっており、
代わりにSensorManager#getOrientationメソッドの仕様が推奨されています。

傾きは、方位角と傾斜角、回転角で表されます。傾きの求め方は"地磁気センサー"と
"加速度センサー"から計算して求めます。前記の複数センサーの取得方法を使います。


傾きは端末の縦横が変わると変わってしまうので、縦固定しておきます。
AndroidManifest.xmlに"portrait"を追記します。
//list[orientation-manifest][縦固定にする]{
<activity android:name=".orientationSensorActivity"
          android:label="@string/app_name"
          android:screenOrientation="portrait">
//}

あとは定石通り、センサーマネージャの登録を行いますが、センサーの登録として
"TYPE_ACCELEROMETER"と"TYPE_MAGNETIC_FIELD"を使用します。onPauseでの登録破棄
も忘れないようにしてください。

//list[orientation-sensormanager][センサーの登録]{
@Override
protected void onResume() {
    super.onResume();

    // センサの取得
    List<Sensor> sensors = mSensorManager.getSensorList(Sensor.TYPE_ALL);

    // センサマネージャへリスナーを登録
    for (Sensor sensor : sensors) {
        if (sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD) {
            mSensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_UI);
        }

        if (sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            mSensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_UI);
        }
    }
}
//}

センサーの取得はonSensorCahngedで行います。この計算の詳細は省きますが
リファレンス@<fn>{orientation_ref}のSensorManager#getRotationMatrixを参照してください。

//footnote[orientation_ref][@<href>{http://developer.android.com/reference/android/hardware/SensorManager.html}]

//list[orientation-changed][傾きを求めるイベントコールバック]{
@Override
public void onSensorChanged(SensorEvent event) {
    // 信頼性の低いデータは捨てる
    if (event.accuracy == SensorManager.SENSOR_STATUS_UNRELIABLE)
        return;

    // 地磁気センサー、加速度センサーの値を取得
    switch (event.sensor.getType()) {
        case Sensor.TYPE_MAGNETIC_FIELD:
            mGeomagnetic = event.values.clone();
            break;
        case Sensor.TYPE_ACCELEROMETER:
            mAcceleration = event.values.clone();
            break;
    }

    // 両方のデータが揃ったら計算を行う
    if (mGeomagnetic != null && mAcceleration != null) {

        SensorManager.getRotationMatrix(inR, I, mAcceleration, mGeomagnetic);

        // Activityの表示が縦固定で、端末表面が自分を向いてる場合
        SensorManager.remapCoordinateSystem(inR, SensorManager.AXIS_X, SensorManager.AXIS_Z, outR);
        SensorManager.getOrientation(outR, mOrientation);

        //  radianToDegree(mOrientation[0])  Z軸方向, Azimuth
        //  radianToDegree(mOrientation[1])  X軸方向, Pitch
        //  radianToDegree(mOrientation[2])  Y軸方向, Roll
        
        mSensor[0].setText(String.valueOf(radianToDegree(mOrientation[0])));
        mSensor[1].setText(String.valueOf(radianToDegree(mOrientation[1])));
        mSensor[2].setText(String.valueOf(radianToDegree(mOrientation[2])));
    }
}
//}

これで求められる値は

 * Z軸方向, 方位角 : 北を0度とする方位
 * X軸方向, 傾斜角 : 端末の縦方向の仰角
 * Y軸方向, 回転角 : 端末の横方向の傾き

言葉で説明するのはわかりづらいので、ぜひ動かしてみてください。センサーの大半は動かして
学習することで理解できます。

//image[sensor-01-orientation][傾きセンサー]{
//}


== 使用可能なセンサーの概要

API-Level 19で規定されているセンサーをざっとですが整理してみます。

==== @<b>{加速度センサー (Acceleration sensor)}

x軸、y軸、z軸のそれぞれの加速度を表します。単位は(m/s^2)
//image[sensor-01-axis_device][加速度センサーの軸]{
//}

==== @<b>{周囲温度センサー (Temperature Sensor)}

端末の周囲の温度を表します単位は(℃)

==== @<b>{地磁気センサー (Geomagnetic field sensor)}

x軸、y軸、z軸方向の磁気の強さを表します。単位は(μT)。磁気の強さから方位を測定できます。

==== @<b>{地磁気センサー（生データ）(Uncalibrated Magnetometer)}

x軸、y軸、z軸方向の磁気の強さをキャリブレーション無しの生データを表します。単位は(μT)

==== @<b>{ジャイロスコープ (Gyroscope)}

x軸、y軸、z軸の回転の速度、角速度を表します。単位は(rad/s)
例えば時計の秒針の角速度は、60秒で一回転(360度)なので、6度/秒です。rad = 度 × π/180となります。
//image[sensor-01-gyro][角速度]{
//}

==== @<b>{照度センサー (Light)}

周囲の明るさを表します。単位は(lx)

==== @<b>{近接センサー (Proximity Sensor)}

端末の前面との距離を表します。単位は(cm)。
ただし"near"と"far"の2値しか返さない端末もあります。
その場合は"near"が最小値となり"far"が最大値となります。

==== @<b>{気圧センサー (Pressure)}

周囲の気圧を表します。単位は(hPa)

==== @<b>{相対湿度センサー (Humidity Sensor)}

周囲の湿度を表します。単位は(%)

==== @<b>{回転ベクトルセンサー (Rotation Vector Sensor)}

傾きセンサーよりも精度が高く傾きを取得できます。

==== @<b>{回転ベクトルセンサー（地磁気影響を除外） (Game Rotation Vector Sensor)}

回転ベクトルセンサーから地磁気の影響を除外したものを表します。ゲームなどに利用します。

==== @<b>{地磁気回転ベクトルセンサー (Geomagnetic Rotation Vector Sensor)}

回転ベクトルセンサーとほぼ同じものですが、ジャイロスコープの代わりに地磁気センサーを使用しています。
回転ベクトルセンサーよりも精度は落ちますが低消費電力です。バックグラウンドでの動作に使われます。

==== @<b>{重力センサー (Gravity sensor)}

重力加速度を表します。単位は(m/s^2)

==== @<b>{ジャイロスコープ（生データ） (Uncalibrated Gyroscope)}

x軸、y軸、z軸の回転の速度、角速度を温度ドリフトなどを補正しない、生データを表します。単位は(rad/s)

==== @<b>{加速度センサー（重力を除外） (Linear acceleration sensor)}

x軸、y軸、z軸のそれぞれの加速度を重力加速度を差し引いて表します。単位は(m/s2^)

==== @<b>{  動き検知 (Significant Motion Sensor)}

端末を持ったユーザが動いたことを検知します。例えば、歩く、自転車、座る、車で移動など。
このセンサーはワンショットのトリガ起動なので、"TriggerEventListener"を使用します。

==== @<b>{歩数計 (Step Counter Sensor)}

端末がリブート起動してからの歩数を表します。

==== @<b>{歩行検知 (Step Detecter Sensor)}

端末を持ったユーザが歩行中であることを検出します。

==== @<b>{傾きセンサー (Orientation Sensor)}

端末の傾きを検出します。方位角と傾斜角と回転角を表します。


== センサーのハードウェア情報の取得

端末に搭載されているセンサーをすべて列挙して取得し、センサーのハードウェア情報を取得してみます。
センサーから取得できるハードウェア情報は以下になります。

//image[sensor-01-hard][センサーのハードウェア情報]{
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

== センサー利用上の注意

センサー情報を取得するには、非常に簡単に取得できることがわかりました。またセンサー
のハードウェア情報を取得することもできました。しかしセンサーを使う上での注意事項と
して、以下の点に留意する必要があります。

 * 機種によって搭載されるセンサーの種類が異なる
 * 機種によって搭載されるセンサーのハードウェアが異なる
 * センサーの数値の上限下限が機種によって異なる

それぞれに対して、対応を考える必要があります。

==== 搭載センサーの違い

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


==== センサーのハードウェアの違い

これは実装の段階ではあまり気にすることは実はありません。ただしiPhoneと異なり、
実際に搭載されているハードウェアは機種によって違うので、センサーの感度や精度が
大きく違う場合があります。

==== センサーの数値の上限下限の違い

センサーのハードウェアが違えば、精度や上限・下限も異なる場合があります。センサー
の値の範囲を気にする必要がある場合もあります。例えば先程のNexus5と中華系の端末
を近接センサーで比較してみると、以下のようになっていたりします。
@<img>{sensor-01-proximity}と比較すると、MaxRangeが異なります。このように
端末によって細かい部分が異なる場合があるので、複数の端末で正常に動作するかどうか
きちんと確認する必要があります。（中華系端末にはMaxFifo,ReservedFifoはAPIレベル
により存在しない）

//image[sensor-01-panda][ある中華端末の場合]{
//}

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

//footnote[sensor_category][@<href>{https://source.android.com/devices/sensors/index.html}]

== センサーのバッチモード

センサーは非常に便利ですが、使うと消費電力が大きいのがデメリットです。そこでAndroid
では、センサーのデータを複数回分貯めこんでおき、バッチ処理で通知する仕組みがあります@<fn>{sensor_batch}。

バッチ処理が実行されるまでは、センサーは低消費電力な状態を維持できるので電池の消費を
抑えることができます。このバッチ処理の間隔は設定ができるます。

バッチモードは、スクリーンがオフの状態やシステムがスリープ中でも動作するため、
フィットネスや位置情報の追跡や監視などに有効な手段となります。Android 4.4からは
歩行検出センサーと、歩数計センサーが使用可能になっており、特にこの辺りのセンサー
を使用する場合に有効な手段となります。

//footnote[sensor_batch][@<href>{https://source.android.com/devices/sensors/batching.html}]

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
の略で、アメリカが上げた高度約2万キロに位置する軍事用衛星の複数からの電波を受信する
ことで、現在位置を正確に知ることができます。正確にはセンサーというよりはGPSという
電波信号を受信する受信機になります。ちょうど電波時計のように電波を受信して時計を
合わせる。というのと同じです。日本で受信可能なGPS衛星の数は6〜10個程度です。
位置測位は緯度と経度を求めるには3つの衛星を捕捉する必要があり、高度を含んだ3次元
測位を行うには4つ以上の衛星の捕捉が必要になります。

測位は三角測量の概念で説明できるのですが、GPSでは距離の計算に時間を利用しています。
電波は光速であることから、GPS衛星の距離を送信時刻と受信時刻との差から速度を掛けて
求めるわけです。しかしGPS衛星には原子時計が積まれていますが、受信機にはそのような
正確な時計がありませんので、代わりにもう１基のGPS衛星の時計を利用します。すなわち
未知数として、位置(x, y, z)と時刻tの4つを得るためには4つ以上の衛星の情報が必要と
いうことになります。

//image[sensor-01-gps][GPS測量のイメージ]{
//}

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

//footnote[mitibiki][@<href>{http://www.jaxa.jp/projects/sat/qzss/index_j.html}]

== GPSを利用した位置情報の取得

GPSを利用して位置情報を取得しますが、これは通常のセンサーと異なり"Google Play Services"
ライブラリを導入して、この中にあるLocationClientを使って取得します。

=== Google Play Services Libraryの導入

"Android SDK Manager"から@<b>{Google Play Services}を選択して、インストールします。
//image[sensor-01-play_service-01][Google Play Servicesのインストール 1]{
//}

"Existing Android Code into Worksapce"を選択します。
//image[sensor-01-play_service-02][Google Play Servicesのインストール 2]{
//}

"Browse"から@<b>{google-play-services_lib}を選択し、@<b>{Copy projects into workspaceにチェック}
を入れて"Finish"ボタンを押します。google-play-servicesはAndroid SDKを
インストールした"$(ANDORID_SDK)/extras/google/google_play_services"にあります。
//image[sensor-01-play_service-03][Google Play Servicesのインストール 3]{
//}

ビルドしてエラーが無いことを確認しておきましょう。
//image[sensor-01-play_service-04][Google Play Servicesのインストール 4]{
//}

アプリを作成時のひな形作成した後、Google Play ServicesライブラリをAndroid
ライブラリとして組み込むすることで使用が可能となります。
//image[sensor-01-play_service-05][Google Play Servicesのインストール 5]{
//}

これでGoogle Play Servicesライブラリの導入は完了です。

=== 位置情報の取得

GPSを利用した位置情報取得にはフレームワークが用意されているのでそれを使います。
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

 * onCreate()もしくはonResumeでLocationClientを取得
 * onResume()でLocationClient#connectでGoogle Play Servicesへ接続
 * onPause()でLocationClient#disconnectでGoogle Play Servicesと切断
 * 位置情報はLocationClient#getLastLocationで取得

また、気をつける点としてLocationClient#connectは非同期関数で、onConnected
コールバックが呼ばれてからでないとLocationClient#getLastLocationを呼ぶことが
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

位置情報を取得した例として、アプリで表示させると下記のようになります。
//image[sensor-01-location][位置情報取得]{
//}

さて、これだけだといわゆる緯度経度の数値しか見えないので、イマイチ取得した
位置が正しいかどうかわかりません。ちょっと工夫して、位置をMapで表示するようにしてみます。

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
})
//}

//image[sensor-01-map][IntentでMap表示]{
//}

=== 位置情報の更新

前記の方法では、起動時の位置情報しか取得できません。これでは移動しながら位置情報を更新する
用途には使いづらいので、都度更新するような方法で実装してみます。

onConnectedまでは同じですが、その後に更新間隔をリクエストするようにします。

 * LocationRequest#create で更新リクエストを生成
 * LocationRequest#setInterval で更新間隔の目安をセット(ms)
 * LocationRequest#setSmallestDisplacement で更新距離間隔をセット(m)
 * LocationRequest#setPriority 更新のプライオリティをセット
 * LocationClient#requestLocationUpdates LocationClientに更新リクエストをセット

//list[gps_request][更新設定]{
@Override
public void onConnected(Bundle connectionHint) {
    // 位置情報の更新リクエスト
    LocationRequest req = LocationRequest.create();
    req.setInterval(5000);
    req.setSmallestDisplacement(1);
    req.setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY);
    mLocationClient.requestLocationUpdates(req, this);
}
//}

プライオリティの設定は、端末の電池消費に影響します。例としてGalaxyNexusの場合
は以下のようになるという実験結果があります@<fn>{gps_priority}。

//image[sensor-01-priority][プライオリティと電池消費量]{
//}

 * PRIORITY_HIGH_ACCURACY : 屋外ではGPS、屋内ではWiFiやセル（基地局）を使用する。電池消費量が多くマップアプリ向け
 * PRIORITY_BALANCED_POWER_ACCURACY : GPSを使わない。WiFiやセルを使用する。ブロックレベル（約100m）の精度
 * PRIORITY_LOW_POWER : GPSを使わない。WiFiやセルを使用する。市レベル（約10km）の精度
 * PRIORITY_NO_POWER : 正確性はその時の状態に依存

微妙に資料と違いますが、公称と実験結果という差がありそうです。。。

//footnote[gps_priority][Googl I/O 2013講演資料より @<href>{http://y-anz-m.blogspot.jp/2013/05/google-io-2013-android-beyond-blue-dot.html}]

さて、このLocationClient#requestLocationListnerを使用する場合は、
com.google.android.gms.location.LocationListenerをリスナーにセットし、
コールバックを実装します。これで、更新タイミングでこのコールバックが呼ばれる
ようになります。また、リクエストした更新はonPauseで解除しておきます。

//list[gps_location][位置情報更新のコールバック]{
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
//}

今回は取得したLocationオブジェクトからいくつかのデータを取り出しています。

 * Latitude : 緯度（度）
 * Longitude : 軽度（度）
 * Altitude : 高度（m）
 * Speed : 移動速度（m/s）
 * Time : 取得時間（UTC）
 * Accuracy : 精度（m）
 * Bearing : 北からの時計回りの角度（度）無い場合は０

このように、ざっくりと位置を確認したい場合は"LocationClient#getLastLocation"を使い、時間や位置の変化で更新を
伴ない場合は"LocationRequest#requestLocationUpdates"を使用するというのが定石となります。

//image[sensor-01-update][位置情報データ]{
//}

== まとめ

センサーの応用例として次のようなものが挙げられます。

 * フィジカルコンピューティング
 * AR(Augmented Reality)とよばれる拡張現実空間
 * 3Dボーカロイドなどの俯瞰処理
 * 体感型ゲーム
 * フォットネスや健康管理

ここまで見てきたように、センサー自体の使い方はさほど難しくありませんが、
これらを組み合わせて応用しようとすると、行列演算やリアルタイム処理など、
かなり複雑な処理を行う必要が出てきます。数学的な視点などプログラミング
技術だけでは無い点が出てきます。また、AndroidはiPhoneのようにすべて
同じ部品が使われているわけではないため、ハードウェアの違いが少なくなく
微妙に動作が異なったり、あるいは全く動かない。ということもあります。実際
のプログラミングや検証作業では、どの端末で対応するのか？APIレベルはいくつ
以上なのか？など考慮する必要があります。

そしてもう一つ重要なのは電池消費量です。GPSも含むセンサーは電池消費が
大きいため、常時動作させることは避けるべきです。いかに電池消費を抑えつつ
効果のある動作を実現できるか？も、このセンサーを扱う上でポイントとなります。