= Bluetooth通信

//lead{
本章ではAndroidのBluetoothを搭載したAndroid端末と他のBluetooth搭載機器との間で通信を行う方法について説明します。
//}

== Bluetooth通信の基礎知識

Bluetoothは2.4GHzの電波帯を利用した通信速度が最大24Mbpsの近距離無線通信を行うための規格です。Bluetoothの規格の最新バージョンは4.0（2014年６月現在）です。

バージョンが3.0までと4.0のBluetoothの機能とでは大きな違いがあり、3.0以前は「クラッシックBluetooth」と呼ばれています。クラッシックといっても、現在流通している一般的なBluetooth機器は2.0から3.0のBluetoothです。

4.0は「Bluetooth Low Enagy」と呼ばれています。本章の前半部分はクラッシックBluetoothが対象で、後半で「Bluetooth Low Enagy」を対象とした説明を掲載します。

また、電波強度によりBluetooth機器の種別がClass1〜Class3と分かれており、Class１の電波の到達距離は100mとなっています。
主にAndroid端末ではBluetooth対応のキーボードやヘッドセットを使用する機会が多いですが、PCや他のAndroid端末と接続してデータのやり取りを行うこともできます。
Bluetoothでは通信しようとする機器同士が同じ通信プロトコルのプロファイルを持っている場合に限り、そのプロファイルの機能に特化した通信ができます。
一般的なBluetoothプロファイルの一部を紹介します（@<table>{bt_prof}）。

//table[bt_prof][一般的なBluetoothプロファイル]{
プロファイル名	説明
-------------------------------------------------------------
SPP	Bluetooth機器を仮想シリアルポート化する
DUN	携帯電話・PHSを介してインターネットにダイアルアップ接続する
HID	マウスやキーボードなどの入力装置を無線化する
HSP	Bluetooth搭載ヘッドセットと通信する
HFP	車内やヘッドセットでハンズフリー通話を実現する
A2DP	音声をレシーバー付きヘッドフォンに伝送する
HDP	健康管理機器同士を接続する
//}


== アプリケーションでBluetooth機能を使う

AndroidのアプリケーションにBluetoothの機能を入れるにはandroid.bluetoothパッケージのAPIを使用します。
使用するクラスは次のとおりです。

 * BluetoothAdapter
 * BluetoothDevice
 * BluetoothSocket
 * BluetoothServerSocket

これらのAPIはAndroid2.0（API Level 5）から使用できます。ただし一部の機能ではAndroid3.0（API Level 11）以降のAPIもあります。今後もAPIが増える可能性もありますので使用する場合はリファレンスの確認もするようにしましょう@<fn>{bluetooth_ref}。

//footnote[bluetooth_ref][http://developer.android.com/intl/ja/guide/topics/connectivity/bluetooth.html]

Bluetooth APIをアプリケーションで使用する事によって、次のような機能を実現できます。

 * 他のBluetooth機器をスキャンする
 * ペアリングされたBluetooth機器のBluetoothアダプターを照会する
 * RFCOMM@<fn>{rfcomm}チャンネルを確立する
 * サービスを検出して他の機器に接続する
 * 他の機器との間でデータをやりとりする
 * 複数の接続を管理する

//footnote[rfcomm][RFCOMMはBluetoothでRS232Cによるシリアル通信をエミュレートするプロトコルです]

Bluetoothを扱う際にはAndroidManifest.xmlの<uses-permission>に「android.permission.BLUETOOTH」のパーミッションの指定が必要です。また端末のBluetooth機能の有効/無効設定やリモートデバイスの検索を行う際は「android.permission.BLUETOOTH_ADMIN」のパーミッションも必要になります。

Bluetooth機能を用いたアプリケーションを開発する際の注意する点として、Bluetoothの接続処理やデータの送受信処理などでは処理が完了するまでに一定時間応答を待つことがあり、ANR（Application Not Responding）となる場合があります。ANRを防ぐために、Bluetoothの接続処理やデータの送受信処理などは、ActivityのUIスレッドではなく別のスレッドで行うようにしましょう。


== 自端末のBluetooth設定制御（BluetoothAdapter）

アプリケーションからBluetoothを有効/無効設定をするサンプルプログラムを次に示します（@<list>{implemention_of_bt_enable}）。

//list[implemention_of_bt_enable][Bluetooth設定制御]{
BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
  // btAdapter がnullの場合はBluetoothがサポートされていない
  if(btAdapter == null){
    // ユーザに通知するなど
    return;
  }
  if (!btAdapter.isEnabled()) {  // BluetoothがOFF状態
    // 有効にするリクエストを投げる
    Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
    startActivityForResult(intent, REQUEST_BT_ENABLE); // REQUEST_BT_ENABLEは"0"などの定数
  }
//}

Bluetoothの有効/無効状態の取得にはBluetoothAdapterクラスのisEnabledメソッドを使用します。Bluetoothを有効にするにはインテントに「ACTION_REQUEST_ENABLE」を指定してstartAcitivityForResultメソッドを実行します。

アプリケーションの実行時、システムはユーザに自動でBluetooth許可リクエストダイアログを表示し、ユーザがダイアログのボタンを押下するとonActivityResultメソッドがコールバックされます。
Bluetoothが有効になった場合は、このメソッドの引数resultCodeにActivity.RESULT_OKが返ってきます（@<img>{01}）。

//image[01][Bluetooth許可リクエストダイアログ（有効設定）][scale=0.35]{
//}


== Bluetooth対応の外部デバイスの検索（BluetoothAdapter）

アプリケーションから実行中のAndroid端末の近隣にあるBluetoothデバイスを検索するサンプルプログラムについて説明します。
デバイス検索を行うにはBluetoothAdapterクラスのstartDiscoveryメソッドを使用します（@<list>{implemention_of_bt_discovery}）。

//list[implemention_of_bt_discovery][デバイス検索]{
  BluetoothAdapter  btAdapter = BluetoothAdapter.getDefaultAdapter();
  if (btAdapter.isDiscovering()) {
    //　探索中だったら一度探索をキャンセル
    btAdapter.cancelDiscovery();
  }
  btAdapter.startDiscovery();
//}

デバイス検索が終了したときに発行されるインテント「ACTION_DISCOVERY_FINISHED」とデバイスが発見されたときに発行されるインテント「ACTION_FOUND」を受信できるようにBroadcastReceiverを作成し登録しておきます（@<list>{implemention_of_bt_receiver}）。

//list[implemention_of_bt_receiver][アクションインテントとBroadcastReceiverの登録]{
  IntentFilter filter = new IntentFilter();
  filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
  filter.addAction(BluetoothDevice.ACTION_FOUND);
  // mReceiverはBroadcastReceiverのインスタンス
  registerReceiver(mReceiver, filter);
//}

「ACTION_FOUND」のインテントには「BluetoothDevice.EXTRA_DEVICE」という名前でBluetoothDeviceクラスのオブジェクトが格納されているので、デバイス名やMACアドレスなどのデバイス情報が取得できるようになります（@<list>{implemention_of_bt_action_found}）。

//list[implemention_of_bt_action_found][デバイス検索のアクションインテントの受信処理]{
  // BroadcastReceiverのonReceiveメソッド
  public void onReceive(Context context, Intent intent) {
    String action = intent.getAction();
    if (BluetoothDevice.ACTION_FOUND.equals(action)) {
      BluetoothDevice device = 
        intent .getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
    // BluetoothDeviceオブジェクトからデバイス情報などを取得する処理
        :
    } else if(BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
      // デバイス探索が完了したので探索キャンセル、レシーバーの削除など
        :
    }
  };
//}


== ペアリング済みデバイスの取得（BluetoothAdapter）

Bluetoothで接続しデータの送受信を行うためには、接続する２台の機器があらかじめ接続設定が完了している状態でなければいけません。この接続設定のことを「ペアリング」と呼びます。

Androidではペアリングしていない別のBluetooth機器と初めて接続する際、自動的に双方の機器にペアリング要求ダイアログが表示されます（@<img>{02}）。

//image[02][Bluetoothペアリング要求ダイアログ][scale=0.35]{
//}

ダイアログで接続先で表示しているPINコードを入力することでペアリングが完了します。この処理はすべてAndroidのシステムで行われるので、アプリケーション開発時に実装する必要はありません。

アプリケーションの中でペアリング済みのデバイス情報一覧を取得するにはBluetoothAdapterクラスのgetBondedDevicesメソッドを使用します。次のサンプルでは取得したペアリング済みデバイスの検索結果（Setクラスのオブジェクト）からデバイス名が"SampleDevice"のデバイス情報を取得しています（@<list>{implemention_of_bt_bonded}）。

//list[implemention_of_bt_bonded][ペアリング済みデバイス一覧を取得]{
  BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
  Set<BluetoothDevice> bondedDevices = btAdapter.getBondedDevices();
  BluetoothDevice device;
  for (BluetoothDevice bluetoothDevice : bondedDevices) {
    if (bluetoothDevice.getName().equals(“SampleDevice”)) {
      device = bluetoothDevice;
      break;
    }
  }
//}


== 検索されたデバイスに接続（クライアント端末として振る舞う）

検索されたデバイスにクライアントとしてSPP接続をするには、BluetoothSocketクラスのオブジェクトを使用します。
次にサンプルプログラムを示します（@<list>{createRfcommSocketToServiceRecord}）。このサンプルではペアリング済みデバイスの検索結果の中から選択したデバイスとSPPで接続しています。


//list[createRfcommSocketToServiceRecord][deviceにクライアントとしてSPP接続する]{
  BluetoothSocket socket = 
    device.createRfcommSocketToServiceRecord(
        UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")); //SPPのUUIDを指定
  socket.connect();// 接続実行

  InputStream in= socket.getInputStream();　// データ受信
  // InputStreamのオブジェクトから受信データを取り出す処理など

  OutputStream out= socket.getOutputStream();　// データ送信
  // OutputStreamのオブジェクトに送信データを設定、write処理など

//}

BluetoothSocketクラスのconnectメソッドで接続処理が実行されます。正常に接続されるとBluetoothSocketクラスのオブジェクトからInputStreamとOutputStreamを取得できるので、それらを使ってデータの送受信を行います。

Bluetoothでは、「接続する機器同士が同じサービス（プロファイル）をSDP@<fn>{sdp}対象のサービス一覧データベースに保有している事」が接続の条件となります。

TCP/IPのソケット通信では接続先のIPアドレスとポート番号を指定して接続をしますが、Bluetooth通信では接続先のMACアドレスとUUIDを指定して接続します。

//footnote[sdp][Service Discovery Protocolとは相手の機器がどのようなサービスをサポートしているのかを検索するのに利用されるプロトコルです]

サンプルプログラム中のcreateRfcommSocketToServiceRecordメソッドは、RFCOMMチャンネルで接続を可能にするためのソケットオブジェクトを作成します。引数には特定のサービス（プロファイル）のUUIDを指定します。
今回はSPPで接続するのでUUIDは "00001101-0000-1000-8000-00805F9B34FB" を用います@<fn>{rfuuid}。

//footnote[rfuuid][SDPで使用されるUUIDの規定値はこちらのリンクを参照してください。https://www.bluetooth.org/ja-jp/specification/assigned-numbers/service-discovery]


== 自端末を別のBluetooth機器から発見可能にする

別のBluetooth機器からのデバイス検索に対して自端末を発見可能な状態にするには、「ACTION_REQUEST_DISCOVERABLE」を指定したインテントをstartAcitivityForResultメソッドで発行します。するとBluetooth許可リクエストダイアログが表示されるのでユーザが許可することにより一定時間応答できる状態になります（@<img>{03}）。

//image[03][Bluetooth許可リクエストダイアログ（発見可能）][scale=0.25]{
//}

発見可能な状態でいる時間をインテントに設定することもできます。その場合、インテントの付加情報としてEXTRA_DISCOVERABLE_DURATIONに時間（秒）を設定します。

サンプルプログラムを次に示します（@<list>{implemention_of_bt_client}）。

//list[implemention_of_bt_client][デバイス検索への応答]{
  Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
  intent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
  startActivityForResult(intent, 
    REQUEST_BT_DISCOVERABLE); // REQUEST_BT_DISCOVERABLEは"1"などの定数
//}


@<img>{03}のようなBluetooth許可リクエストダイアログを表示されます。ユーザが許可をすることで、自端末は一定時間（サンプルコードの場合は300秒間）、別のBruetooth機器からのデバイス検索に対して発見可能な状態になります。


ダイアログでユーザが許可をすると、onActivityResultメソッドにコールバックがきます。引数resultCodeには設定した発見可能な時間の値（秒）が入っています。

自端末がBruetooth機器として発見可能な状態かどうかの変化が起きた場合、BroadcastReceiverで通知を受け取ることができます。
それを実現するためにはあらかじめ「ACTION_SCAN_MODE_CHANGED」インテントをBroadcastReceiverとして登録します。
状態の変化がおき、BroadcastReceiverで受け取ったインテントには、古い状態「EXTRA_PREVIOUS_SCAN_MODE」と新しい状態「EXTRA_SCAN_MODE」に次の値が入っています。

 * SCAN_MODE_CONNECTABLE_DISCOVERABLE：発見可能な状態でかつ、接続も可能な状態
 * SCAN_MODE_CONNECTABLE：発見可能な状態ではないが、接続可能な状態
 * SCAN_MODE_NONE：発見可能な状態でもなく、接続もできない状態

== 別のBluetooth機器からの接続要求を受ける（サーバー端末として振る舞う）

サーバーとして別のBluetooth機器からの接続要求を受付けるにはBluetoothServerSocketクラスを使用します。
次にサンプルプログラムを示します（@<list>{implemention_of_bt_server}）。

//list[implemention_of_bt_server][リモートデバイスからの接続要求の受付]{
  BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
  BluetoothServerSocket serverSocket =
    btAdapter.listenUsingRfcommWithServiceRecord("SampleServerConn",
    UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"));
  BluetoothSocket socket = serverSocket.accept();
  if (socket != null) {   // クライアントからの接続要求を受け付け接続が完了した状態
    // データの送受信処理などを行う
      :
    serverSocket.close()
  }
//}

はじめにBluetoothAdapterクラスのlistenUsingRfcommWithServiceRecordメソッドを使用して
BluetoothServerSocketクラスのオブジェクトを生成します。接続を受付ける条件（プロファイルのUUID）を第２引数に指定します。
サンプルコードは、RFCOMMチャンネルでSPPプロファイルの接続受付を可能とする実装になっています。

前述のサンプルプログラムで生成したBluetoothServerSocketクラスのオブジェクトのacceptメソッドで、接続要求の受付を行います。同じUUIDが指定された接続要求がきた場合、acceptメソッドの戻り値としてBluetoothSocketオブジェクトが得られます。
BluetoothSocketオブジェクトからInputStreamとOutputStreamを取得できるので、それらを使ってデータの送受信を行います。

クライアントとなる端末から接続要求がこない限り、acceptメソッドの応答（BluetoothSocketオブジェクト）は得られません。
acceptメソッドで受付待ち状態のときは処理が一定時間中断してしまうので、接続処理や、継続的なデータの送受信の処理はUIスレッドとは別のスレッドで処理するようにします。

接続要求の受付処理、およびデータの送受信が完了したらBluetoothSocketオブジェクトはcloseメソッドで破棄します。


== プロファイルのサポート

Bluetoothにはさまざまなプロファイルが存在します。これらすべてのプロファイルをAndroid SDKに含まれるAPIから使用（Bluetooth機器からのデータの取得やBluetooth機器のコントロール）ができるわけではありません。
AndroidではAndroid 3.0（HonneyComb：API Level 11）以降では、「HSP」、「HFP」と「A2DP」対応のBluetooth機器からの情報、Android 4.0（IceCreamSandwich：API Level 14）以降では「HDP」対応のBluetooth機器から情報を取得できるようになりました。

各プロファイルの概要を説明します。

 * HSP（Headset Profile）
 
HSPは、携帯電話といっしょに使うBluetoothヘッドセットの接続機能を提供します。
Androidでは、android.bluetooth.BluetoothHeadsetクラスが提供されています。
これはAndroidの端末の内部で動作しているBluetooth Headset Serviceにプロセス間通信（IPC）で接続し、実際に制御するためのプロキシとなります。

 * HFP（Hands-Free Profile）
 
自動車の車載器などのようなハンズフリーデバイスで、通信（発呼、着呼）をするために携帯電話を使用するための機能を提供します。HFPのバージョン1.5で提供される音質は非常に悪く、バージョン1.6で高音質対応されました。Androidでは4.0以降でバージョン1.6に対応しています。
このプロファイルのAndroidにおけるAPIでのサポートは、HSPと同様にandroid.bluetooth.BluetoothHeadsetクラスで提供されています。

 * A2DP（Advanced Audio Distribution Profile）

A2DPはBluetooth機器間で高品質のオーディオストリームを流すしくみを提供します。
Android はandroid.bluetooth.BluetoothA2dpクラスが提供されています。
これはAndroidの端末の内部で動作しているBluetooth A2DP Serviceにプロセス間通信（IPC）で接続し、実際に制御するためのプロキシとなります。

 * HDP（Health Device Profile）
HDPはBluetooth搭載の健康管理機器間を接続、データ通信を提供します。
Androidではandroid.bluetooth.BluetoothHealthクラスが提供されています。

=== BluetoothProfileインターフェースのServiceListener

これらのプロファイルを持つBluetooth機器に接続してAndroidアプリケーションからそれをサポートするためには、android.bluetooth.BluetoothProfileインターフェースを用います。

@<list>{implemention_of_bt_service}のように、BluetoothProfileインターフェースが持つServiceListenerを生成し、BluetoothAdapterに登録することで指定のプロファイルを持つBluetooth機器への接続時のイベント、および切断時のイベントメソッドを使うことができます。

ServiceListenerを使用した各プロファイルのプロキシオブジェクトの取得例を次に示します。

//list[implemention_of_bt_service][BluetoothProfile.ServiceListenerの実装]{

  private BluetoothHeadset mBluetoothHeadset; // HSPのオブジェクト
  private BluetoothA2dp mBluetoothA2dp; // A2DPのオブジェクト
  private BluetoothHealth mBluetoothHealth; // HDPのオブジェクト

  private BluetoothProfile.ServiceListener mProfileListener =
      new BluetoothProfile.ServiceListener() {

    @Override
    public void onServiceConnected(int profile, BluetoothProfile proxy) {
      // プロキシオブジェクトがアダプタに指定のサービスに接続されたときに呼ばれる
      // 接続された機器のプロファイル種別とBluetoothProfileのオブジェクトを受け取り
      // プロファイル専用の型のオブジェクトに保持しておく
      if (profile == BluetoothProfile.HEADSET) {
        mBluetoothHeadset = (BluetoothHeadset) proxy;
      } else if (profile == BluetoothProfile.A2DP) {
        mBluetoothA2dp = (BluetoothA2dp) proxy;
      } else if (profile == BluetoothProfile.HEALTH) {
        mBluetoothHealth= (BluetoothHealth) proxy;
      }

      // 接続されているデバイスをBluetoothDeviceオブジェクトのリストで取得する
      List<BluetoothDevice> devices = proxy.getConnectedDevices();

      // １件ずつBluetoothDeviceオブジェクトを取り出し、
      // 機器名やMACアドレスなどの必要な情報の処理を行うなど・・・

    }

    @Override
    public void onServiceDisconnected(int profile) {
      // プロキシオブジェクトがアダプタに指定のサービスから切断されたときに呼ばれる

      // 必要な後処理など・・・

      // 最後に切断されるプロファイルのオブジェクトの参照を切り離す処理を行う
      if (profile == BluetoothProfile.HEADSET) {
        mBluetoothHeadset = null;
      } else if (profile == BluetoothProfile.A2DP) {
        mBluetoothA2dp = null;
      } else if (profile == BluetoothProfile.HEALTH) {
        mBluetoothHealth = null;
      }
    }
  };

    // Bluetoothアダプターにサービスリスナーで取得するプロファイルをプロキシオブジェクトとして登録する
    BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    mBluetoothAdapter.getProfileProxy(this,
      mProfileListener, BluetoothProfile.HEADSET); //HSPを指定
    mBluetoothAdapter.getProfileProxy(this,
      mProfileListener, BluetoothProfile.A2DP); //A2DPを指定
    mBluetoothAdapter.getProfileProxy(this,
      mProfileListener, BluetoothProfile.HEALTH); // HDPを指定

//}

=== プロファイル固有のオブジェクト

次にリスナーで受け取ったandroid.bluetoothパッケージのプロファイル固有クラスのオブジェクト使用方法について解説します。

==== BluetoothHeadset
BluetoothHeadsetクラスでは以下のアクションインテントが定義されています。
ブロードキャストレシーバでIntentを取得することで、ヘッドセットの接続状態変化、音声状態変化、ベンダー固有のイベント通知をリアルタイムに取得可能です（@<table>{bt_hsp_intent}）。

//table[bt_hsp_intent][BluetoothHeadsetのアクションインテント]{
値	説明
-------------------------------------------------------------
ACTION_AUDIO_STATE_CHANGED	A2DPの音声状態変化
ACTION_CONNECTION_STATE_CHANGED HSP	の接続状態の変化
ACTION_VENDOR_SPECIFIC_HEADSET_EVENT	ベンダー固有のイベント通知
//}

BluetoothHeadsetクラスが持つメソッドで音声認識やベンダ固有の機器操作用のコマンド送信を行うことが可能です。

 * 音声認識の開始と終了
 ** boolean  startVoiceRecognition(BluetoothDevice device)
 ** boolean  stopVoiceRecognition(BluetoothDevice device)

 * ベンダ固有の機器操作用のコマンド送信
 ** boolean  sendVendorSpecificResultCode(BluetoothDevice device, String command, String arg)

==== BluetoothA2dp
BluetoothA2dpクラスでは次のアクションインテントが定義されています（@<table>{bt_a2dp_intent}）。
ブロードキャストレシーバでIntentを取得することで、接続状態変化、再生状態変化をリアルタイムに取得可能です。

//table[bt_a2dp_intent][BluetoothA2dpのアクションインテント]{
値	説明
-------------------------------------------------------------
ACTION_CONNECTION_STATE_CHANGED	A2DPの接続状態変化
ACTION_PLAYING_STATE_CHANGED	A2DPプロファイルの再生状態変化
//}

ブロードキャストレシーバーによる状態変化を取得する例を次に示します（@<list>{implemention_of_bt_bcast1}）。

//list[implemention_of_bt_bcast1][アクションインテントの登録]{
    IntentFilter filter = new IntentFilter();
    filter.addAction(BluetoothA2dp.ACTION_CONNECTION_STATE_CHANGED);
    filter.addAction(BluetoothA2dp.ACTION_PLAYING_STATE_CHANGED);
    registerReceiver(mReceiver, filter);
//};

//list[implemention_of_bt_bcast2][ブロードキャストレシーバーの実装]{
  private BroadcastReceiver mReceiver = new BroadcastReceiver() {
    @Override
    public void onReceive(Context context, Intent intent) {
      String action = intent.getAction();
      //現在の状態
      int status = intent.getIntExtra(BluetoothProfile.EXTRA_STATE, -1);
      //直前の状態
      int prevStatus =
         intent.getIntExtra(BluetoothProfile.EXTRA_PREVIOUS_STATE, -1);
      // 接続しているBluetooth機器の情報
      BluetoothDevice device =
         intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);

      // 接続状態変更のインテントを受信した場合
      if (action.equals(BluetoothA2dp.ACTION_CONNECTION_STATE_CHANGED)) {
        switch (status){
        case BluetoothProfile.STATE_CONNECTED:  //接続済の状態
          :
        case BluetoothProfile.STATE_DISCONNECTED:  //切断している状態
          :
        case BluetoothProfile.STATE_DISCONNECTING:  //切断処理中の状態
          :
  };

//};

==== BluetoothHealth

Bluetoothヘルス機器と通信をするためにはBluetoothHealthクラスを使用します。
ヘルス機器の状態をアプリケーション側で受け取るには、HSPやA2DPで扱えているようなアクションインテントは用意されていません。
android.bluetooth.BluetoothHealthCallbackクラス（Android 4.0以降：API Level 14）を生成して状態変化時にコールバックメソッドが呼ばれるようにします。
作成したBluetoothHealthCallbackオブジェクトは、BluetoothHealthクラスのregisterSinkAppConfigurationメソッドで登録します。

Bluetoothヘルス機器には２つのデータタイプが定義されています。ひとつは、SOURCEタイプ（BluetoothHealth.SOURCE_ROLE）でデータを送信する側（ヘルス機器）を指します。もうひとつは、SINKタイプ（BluetoothHealth.SINK_ROLE）でデータを受信する側です。
接続した機器のデータタイプは、BluetoothHealthAppConfigurationオブジェクトのgetRoleメソッドで取得できます。
SINKタイプは複数のSOURCEタイプの機器から情報を受け取ることができますがその反対はできません。

Androidアプリケーションを作成する場合、接続したヘルス機器から情報を取得して画面に表示させることが一般的ですので、上述したregisterSinkAppConfigurationメソッドの引数のデータタイプはBluetoothHealth.SINK_ROLEにします。

BluetoothHealthCallbackが持つコールバックメソッドは、onHealthAppConfigurationStatusChange、onHealthChannelStateChangeがあります。

onHealthAppConfigurationStatusChangeメソッドは、ヘルス機器の登録状態が変わったときにコールバックされるメソッドで、第２引数に次の値が入ってきます（@<table>{bt_health_status1}）。

//table[bt_health_status1][onHealthAppConfigurationStatusChangeメソッド]{
値	説明
-------------------------------------------------------------
APP_CONFIG_REGISTRATION_SUCCESS	登録成功
APP_CONFIG_REGISTRATION_FAILURE	登録失敗
APP_CONFIG_UNREGISTRATION_SUCCESS	登録解除成功
APP_CONFIG_UNREGISTRATION_FAILURE	登録解除失敗
//}

onHealthChannelStateChangeメソッドはチャンネルの状態が変更されたときにコールバックされるメソッドで第３引数（変更前の状態）と第４引数（変更後の状態）に次の値が渡されます（@<table>{bt_health_status2}）。

//table[bt_health_status2][onHealthChannelStateChangeメソッド]{
値	説明
-------------------------------------------------------------
STATE_CHANNEL_CONNECTING	接続処理中
STATE_CHANNEL_CONNECTED		接続完了
STATE_CHANNEL_DISCONNECTING	切断処理中
STATE_CHANNEL_DISCONNECTED	切断完了
//}

コールバックを登録したら、Bluetoothヘルス機器への接続を行います。接続にはBluetoothHealthクラスのconnectChannelToSourceメソッドを使用します。

アプリケーションからBluetoothヘルス機器への接続と状態変化通知を受け取るまでのコード例を次に示します（@<list>{implemention_of_bt_health}）。

//list[implemention_of_bt_health][luetoothヘルス機器への接続と状態変化通知の受信のサンプルコード]{
  private BluetoothHealthAppConfiguration mHealthConfig;
  private int mChannelId;
  private BluetoothDevice mBluetoothDevice;

  class MyBluetoothHealthCallback extends BluetoothHealthCallback {

    @Override
    public void onHealthAppConfigurationStatusChange(
        BluetoothHealthAppConfiguration config, int status) {
      super.onHealthAppConfigurationStatusChange(config, status);

      // 登録が成功したら通信に必要なアプリケーションコンフィギュレーションを保持
      mHealthConfig = config;
        :
    }

    @Override
    public void onHealthChannelStateChange(
        BluetoothHealthAppConfiguration config,
        BluetoothDevice device, int prevState, int newState,
        ParcelFileDescriptor fd, int channelId) {

      super.onHealthChannelStateChange(config,
        device, prevState, newState, fd, channelId);

      if (newState == BluetoothHealth.STATE_CHANNEL_CONNECTED){
        // チャンネルに関連づいたIDを保持しておく
        // このIDは切断するときに使用する
        mChannelId = channelId;
        // BluetoothDeviceオブジェクトを保持しておく
        // 接続時に使用する
        mBluetoothDevice = device;
          :
          :
      }
    }
  };

  private void register() {
    MyBluetoothHealthCallback mCallback = new MyBluetoothHealthCallback();
    // ヘルス受信機器として機能するアプリケーションコンフィギュレーションを登録する
    mBluetoothHealth.registerSinkAppConfiguration("HEALTH_DEVICES",
      BluetoothHealth.SINK_ROLE, mCallback);
  }

  private void unregister() {
    // アプリケーションコンフィギュレーションを登録解除する
    mBluetoothHealth.unregisterAppConfiguration(mHealthConfig);
  }

  private void connect() {
    // Bluetoothヘルス機器に接続する
    mBluetoothHealth.connectChannelToSource(mBluetoothDevice, mHealthConfig);
  }

  private void disconnect() {
    // Bluetoothヘルス機器から切断する
    mBluetoothHealth.disconnectChannel(mBluetoothDevice,
      mHealthConfig, mChannelId);
  }

//};


実際にヘルス機器から取得したデータはonHealthChannelStateChangeメソッドの第５引数に入ってくるファイルディスクリプタを使って読み出します。
ただしデータの解析には機器ごとに異なる解析処理が必要となります。
心拍計ならIEEE 11073-10407、歩数計ならIEEE 11073-10441というように、IEEE 11073-104xxの規格に沿ったデータ解析処理の実装がヘルス機器種別ごとに必要となります。
また接続する実機が必要です。ここで紹介したサンプルコードを実機にそのまま用いても動作の保証はなく、機器固有の設定が必要です。


== Bluetooth Low Energy

本章の冒頭で説明したとおり、Bluetoothのバージョン4.0にはBLE（Bluetooth Low Enagy）という仕組みが採用されています。ここではBLEの概要を説明します。
BLEの最大の特徴は超低消費電力であることです。BLEの通信機器はリチウム乾電池１つで１年程度動作可能と言われています。
データ転送速度は1Mbps程度（ただし、一度に20byte前後の小さいサイズのデータしかやり取りできない）で、周波数帯も同じく2.4GHz帯です。同じ周波数帯の機器からの干渉を防ぐための仕組みも備わっています。
このような特徴を活かして、フィットネス系のセンサーやウェアラブルデバイス、位置情報サービスや店舗サービスに採用されつつあります。
もともとクラッシックBluetoothとBLEは全く互換性がないのですが、ハードウェアとして共存させることは可能で、Bluetoothチップセットの中には両モードを同時に利用できるものもあります。

Bluetooth4.0搭載の機器には、BLEやクラッシックBluetoothの対応の状態を識別するために下記のような名称が定められています。

 * Bluetooth SMART：BLEでのみ通信可能で、クラシックとの通信はできない
 * Bluetooth SMART READY：BLEとクラシックの両方の通信に対応

BLEではGATT（Generic Attribute）というプロファイルで通信を行います。
AndroidアプリケーションでBLE機器と通信する場合、次の条件が揃っていなければいけません。

 * Android端末がハード的にBluetooth4.0に対応していること
 * Android OSのバージョンがBLEに対応していること（Android4.3）

BLE機器と通信するアプリケーションの開発で使用するandroid.bluetoothパッケージAPIを紹介します。

=== BluetoothManager

Android4.3 （API Level 18）より、BluetoothManagerクラスがサポートされ、BluetoothAdapterのインスタンスの取得などができるようになりました（@<list>{implemention_of_ble1}）。

//list[implemention_of_ble1][BluetoothManager]{
  BluetoothManager manager =
     (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
  mBluetoothAdapter = manager.getAdapter();
//};

=== LeScanCallback
GATTプロファイル対応のデバイスの探索結果を受け取るためのコールバックメソッドが定義されたクラスです。

=== BluetoothGattCallback
指定したBLE機器と接続が完了したときに呼ばれるコールバックメソッドが定義されたクラスです。BluetoothGattクラスのインスタンスを受け取ります。

=== BluetoothGatt
BLE機器とGATTプロファイルで接続するためのクラスです。
GATTプロファイルには、１つのプロファイルに付き複数のサービス（使用用途や機器の種類によって異なる）を持っています。また１つのサービスにつき、データ種別ごとに複数のキャラクタリスティックという単位でデータが管理されています。実際にBLE機器とデータをやり取りする場合はこのキャラクタリスティックに対してデータをやり取りします。
BluetoothGattクラスでは、指定したUUIDを持つGATTサービスの検索、キャラクタリスティックの読み書き、ディスクリプタの読み書き、BLE機器からのNotificationの受信設定、GATTプロファイルでの通信の接続や切断などが実行できます。

=== BluetoothGattService
GATTサービスのクラスです。このオブジェクトからUUIDで指定されたキャラクタリスティックが取得できます。

=== BluetoothGattCharacteristic
GATTプロファイルのキャラクタリスティックのクラスです。

=== BluetoothGattDescriptor
BluetoothGattCharacteristicクラスのオブジェクトからディスクリプタを取得できます。アプリケーションからの値の読み書きの対象となります。



#@# == BluetoothSocketによるデータ通信の実践

#@# TODO 教科書には載せない

#@# EOF
