= ネットワークデバイス（Wi-Fi、Bluetooth）

//lead{
 本章ではAndroidでWi-Fiの設定をする方法、およびWi-Fiの状態を取得する方法について説明します。
 //}

== Wi-Fiの概要

無線LANの標準規格であるIEEE 802.11シリーズに準拠している無線LANそのもの、および無線LAN機器のことをWi-Fi（Wireless Fidelity）と呼んでいます。
無線LANとは、有線ケーブルを使わずにアクセスポイントと呼ばれる中継機やルータから発する電波を用いて数m～数十m程度の範囲内でPCや電子機器間、もしくはインターネットへの接続で高速なデータ通信を行う通信技術です。

Wi-Fiをはじめとした無線LANでの通信では、通信データを傍受される危険性が有線による通信に比べて高いので、セキュリティを保護する必要があります。接続する際の認証方法で用いられる要素に「SSID」、認証方式として「WEP」「WPA」「WPA2」があります。

 * SSID（Service Set ID）

Wi-FiのアクセスポイントのIDで最大32文字までの英数字を任意に設定できます。

 * WEP（Wired Equivalent Privacy）
 
固定されたPSK（Pre-Shared Key）を使用した暗号化を行います。鍵は13文字までの数字か、26桁までの16進数を使用できますがセキュリティ強度は非常に低いので現在では推奨されていません。

 * WPA（WiFi Protected Access）

WEPの強化版となる暗号化方式です。

 * WPA-PSK（TKIP）

暗号化に用いる秘密鍵を一定間隔で更新することができ、セキュリティ強度が高い暗号化技術です。ただし、更新頻度が大きくなるとネットワークに負荷がかかってしまう問題もあります。

 * WPA-PSK（AES）

解読が不可能とされており、非常にセキュリティ強度が高い暗号化技術です。CCMP（Counter Mode with Cipher Block Chaining Message Authentication Code Protocol）と呼ばれる技術が、認証とデータ暗号化に使用されています。現在、最も信頼できる暗号化技術とされており推奨される方式です。

 * WPA2

WPAの後継規格です。AESが義務化されるかたちで標準化されました。WPA同様、TKIPやAESを使用する事が可能な暗号化方式です。

== アプリケーションでWi-Fi機能を使う

Wi-Fi機能を搭載したAndroidでは無線LANを通してインターネットに接続することが可能です。また、AndroidのアプリケーションからWi-Fi機能を制御したり、情報を取得する事も可能です。
AndroidのアプリケーションでWi-Fi機能に関する制御を行うにはandroid.net.wifiパッケージ@<fn>{wifi_ref}のAPIを使用します。

//footnote[wifi_ref][http://developer.android.com/intl/ja/reference/android/net/wifi/package-summary.html]

Wi-Fi機能のAPI をアプリケーションで使用する事によって、次のような機能を実現できます。

 * 自端末のWi-Fi機能の有効/無効設定をする
 * Wi-Fiネットワークの検索をする
 * Wi-Fi機器との接続を確立する
 * Wi-Fiネットワーク設定情報の取得
 * IP Multicast@<fn>{multicast}を使用する

//footnote[multicast][ ネットワーク内で、複数の相手を指定して同じデータを送信すること]

Wi-Fiの状態を取得するにはAndroidManifest.xmlの<uses-permission>に「android.permission.ACCESS_WIFI_STATE」のパーミッションの指定が必要です。Wi-Fiの設定を変更するには「android.permission.CHANGE_WIFI_STATE」のパーミッションが必要です。
またIP Multicastを使用するには「android.permission.CHANGE_WIFI_MULTICAST_STATE」のパーミッションが必要です。


== Wi-Fiの設定制御（WifiManager）

アプリケーションからWi-Fi機能を有効／無効設定をするサンプルプログラムを次に示します（@<list>{implemention_of_wifi_enable}）。

//list[implemention_of_wifi_enable][Wi-Fi設定制御]{
  WifiManager wm = (WifiManager) getSystemService(Context.WIFI_SERVICE);
  if (!wm.isWifiEnabled()) {
    wm.setWifiEnabled(true); // Wi-Fiを有効にする
  }
//}

Wi-Fiが有効かどうかWifiManagerクラスのisEnableメソッドで確認し、無効ならsetWifiEnabledメソッドで有効にします。
Wi-Fi機能の有効／無効状態の取得にはWifiManagerクラスのisEnabledメソッドを使用します。Wi-Fi機能を有効にするにはsetWifiEnabledメソッドで有効にします。

== WiFiネットワークの検索

アプリケーションから実行中のAndroid端末の近隣のWi-Fiネットワーク機器の探索をするサンプルプログラムについて説明します。
Wi-Fi機器の探索をするにはWifiManagerクラスのstartScanメソッドを使用します（@<list>{implemention_of_wifi_startscan}）。

//list[implemention_of_wifi_startscan][Wi-Fiネットワークの探索]{
  WifiManager wm = (WifiManager) getSystemService(Context.WIFI_SERVICE);
  wm.startScan(); // Wi-Fiネットワーク探索開始
//}

発見したWi-Fi機器の情報を受けとるためのインテント「SCAN_RESULTS_AVAILABLE_ACTIOND」を受信できるようにBroadcastReceiverを作成し登録しておきます（@<list>{implemention_of_wifi_receiver}）。

//list[implemention_of_wifi_receiver][アクションインテントとBroadcastReceiverの登録]{
  IntentFilter filter = new IntentFilter();
  filter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION);
  registerReceiver(mReceiver, filter);
//}

このインテントを受信したとき、WifiManagerのgetScanResultsメソッドを使用する事で、発見したWi-Fi機器の情報(ScanResultオブジェクト)が格納されているListが取得できます（@<list>{implemention_of_wifi_action_found}）。

//list[implemention_of_wifi_action_found][Wi-Fiネットワークの探索のアクションインテントの受信処理]{
  BroadcastReceiver  mReceiver = new BroadcastReceiver() {
    public void onReceive(Context context, Intent intent) {
      String action = intent.getAction();
      if (WifiManager.SCAN_RESULTS_AVAILABLE_ACTION.equals(action)) {
        WifiManager wm = (WifiManager) getSystemService(Context.WIFI_SERVICE);
        List<ScanResult> list = wm.getScanResults();

        // 取得したScanResultのリストから 機器データの取得などを行う

      }
    }
  };
//}

== Wi-Fi機器への接続と認証方法

Wi-Fi機器の検索をして得られた機器のListから特定のWi-Fi機器で接続するためのサンプルコードを示します（@<list>{implemention_of_wifi_wep}）。接続に必要な設定(SSIDの値やWEP/WPAなどのキー)は、WifiConfigrationクラスのオブジェクトにセットします。接続する際の認証方式によって、設定内容が異なります。
なお、サンプルコード中の変数ssidはScanResultから取得したSSIDの値です。

* 認証方式がWEPの場合の場合のWifiConfiguration

//list[implemention_of_wifi_wep][WEP認証]{
  WifiConfiguration config = new WifiConfiguration();
  //SSID
  config.SSID = "\"" + ssid + "\"";
  //このコンフィギュレーションで管理する認証キー
  config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE); //WEPの場合
  //IEEE 802.11認証アルゴリズム
  config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.SHARED);//WEPの場合
  //認証されたグループの暗号
  config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);//40ビット長暗号化キー
  config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);//40ビット長暗号化キー
  //WEP用キー
  config.wepKeys[0] = "\"password\"";
  config.wepTxKeyIndex = 0;
//}

WEP認証で接続する際にはWifiConfigrationクラスのallowedKeyManagementに「WifiConfiguration.KeyMgmt.NONE」を設定します。他の設定値に関してもWEP認証に必要な値を設定します（@<list>{implemention_of_wifi_wep}）。

 * 認証方式がWPA/WPA2-PSKの場合のWifiConfiguration

//list[implemention_of_wifi_wep][WPA/WPA2-PSK認証]{
  WifiConfiguration config = new WifiConfiguration();
  //SSID
  config.SSID = "\"" + ssid + "\"";
  //このコンフィギュレーションで管理する認証キー
  config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
  //IEEE 802.11認証アルゴリズム
  config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);//WPA/WPA2の場合
  //セキュリティプロトコル
  config.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
  config.allowedProtocols.set(WifiConfiguration.Protocol.RSN);//WPA2
  //認証されたグループの暗号
  config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
  config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
  //WPA認証用ペア暗号
  config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
  config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
  //WPA用共通キー
  config.preSharedKey = "\"password\"";
//}

WPA/WPA2認証で接続する際にはWifiConfigrationクラスのallowedKeyManagementに「WifiConfiguration.KeyMgmt.WPA_PSK」を設定します。他にもWEP認証と異なる値をコンフィギュレーションに設定します（@<list>{implemention_of_wifi_con}）。

 * 生成したWifiConfigurationで接続する

//list[implemention_of_wifi_con][生成したWifiConfigurationで接続する]{
  if( manager.addNetwork(config) == -1 ){ //設定済のネットワークに新しいネットワーク情報を追加する
    // 失敗した場合-1となる
    return false;
  };
  wifiManager.saveConfiguration(); //設定されたネットワーク情報をこの端末に保存する
  // 最新のネットワークへ強制的に接続する
  wifiManager.updateNetwork(config);
  manager.enableNetwork(config.networkId, true);
//}

Wi-Fiの接続はWifiManagerクラスのaddNetworkメソッドを使用します。戻り値としてネットワークIDが戻ってくるので、この値をenableNetworkメソッドの引数に指定して接続を有効にします。
saveConfigurationメソッドで接続したWi-Fi機器の設定情報をシステムに保存しておきます。

次に接続中のWi-Fi機器との接続を切断する場合のサンプルコードを示します（@<list>{implemention_of_wifi_discon}）。WifiManagerクラスのdisconnectメソッドを使用することで切断することができます。

//list[implemention_of_wifi_discon][接続中のWi-Fi機器と切断]{
  WifiManager wm = (WifiManager) getSystemService(Context.WIFI_SERVICE);
  wm.disconnect();
//}


== WiFiネットワーク設定情報の取得

接続が可能になったWi-Fi機器の接続設定情報はWifiManagerクラスのsaveConfigurationメソッドによってシステムに保存されています。保存済みの設定情報を取得するにはWifiManagerクラスのgetConfiguredNetworksメソッドを使用します。戻り値としてWifiConfigurationクラスのオブジェクトが入ったListを取得することができます。
次に示すサンプルコードは、取得した接続設定情報をログに出力しています（@<list>{implemention_of_wifi_getconf}）。

//list[implemention_of_wifi_getconf][Wi-Fiネットワークの設定情報を取得する]{
  WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
  List<WifiConfiguration> cfgList = wm.getConfiguredNetworks();
  for (int i = 0; i < cfgList.size(); i++) {
    Log.v("WifiConfiguration", "NetworkID = " + cfgList.get(i).networkId);
    Log.v("WifiConfiguration", "SSID = " + config_cfgListlist.get(i).SSID);
//}

== WiFi接続情報の取得とその情報の使用例など

Wi-Fiの接続状態を取得するためのサンプルコードを次に示します（@<list>{implemention_of_wifi_getconf}）。
接続状態はWifiManagerクラスのgetConnectionInfoメソッドで取得することができます。取得結果は戻り値のWifiInfoクラスのオブジェクトに含まれています。

//list[implemention_of_wifi_getconf][Wi-Fiの接続状態を取得する]{
  WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
  WifiInfo info = wm.getConnectionInfo();
  Log.v("WifiInfo", "SSID = " + info.getSSID());
  Log.v("WifiInfo", "BSSID = " + info.getBSSID());
  Log.v("WifiInfo", "IP Address = " + info.getIpAddress());
  Log.v("WifiInfo", "Mac Address = " + info.getMacAddress());
  Log.v("WifiInfo", "Network ID = " + info.getNetworkId());
  Log.v("WifiInfo", "Link Speed = " + info.getLinkSpeed());
//}

このWifiInfoクラスに含まれる接続情報の一覧を次に示します（@<table>{wifi_info}）。

//table[wifi_info][取得可能なWi-Fi接続情報]{
値 説明
-------------------------------------------------------------
SSID  無線識別用のID（Service Set Identifier）
IPアドレス  機器のIPアドレス@<fn>{ad}（論理アドレス）
MACアドレス 機器のMACアドレス（物理アドレス）
RSSI  受信信号強度
BSSID BSS (Basic Service Set)のID（接続しているWi-Fi機器のMACアドレスと同じ）
ネットワークID  接続時にシステムに与えられたID
接続状態  Wi-Fiの接続状態
//}

Wi-Fiの接続状態については次の種類があります。

 * WIFI_STATE_DISABLING：切断処理中
 * WIFI_STATE_DISABLED：切断
 * WIFI_STATE_ENABLING：接続処理中
 * WIFI_STATE_ENABLED：接続済み
 * WIFI_STATE_UNKNOWN：不明な状態

//footnote[ad][IPアドレスはint型で返ってくるのでオクテット表記にビット演算で直す必要がある]

#@# TODO 教科書には載せない
#@# int ip_addr_i = w_info.getIpAddress();
#@# String ip_addr = ((ip_addr_i >> 0) & 0xFF) + "." + ((ip_addr_i >> 8) & 0xFF) + "." + ((ip_addr_i >> 16) & 0xFF) + "." + ((ip_addr_i >> 24) & 0xFF);
#@# Log.i("Sample", "IP Address:"+ip_addr);


== Wi-Fi Directとは
Wi-Fi Directとは無線LANを利用した通信方式の一つでアクセスポイント（無線LANのルーター機器）がなくてもWi-Fi Directに対応する機器同士が１対１で通信することができる仕組みです。
もともとWi-Fi Directに対応している機器が無線LANのアクセスポイント機能を内蔵しているので、 自身がアクセスポイントになることにより通信を実現します。
この時、アクセスポイントの役割を果たす機器をグループオーナーと呼び、他の機器からは通常のアクセスポイントと同じように見えます。

 * Wi-Fi Direct対応の機器 <=> Wi-Fi Direct対応の機器
 * Wi-Fi Direct対応の機器 <=> Wi-Fi Directに対応していない機器

他の特徴として以下があげられます。

 * 「１対多」の通信も行える。但し、同時に通信できるのは「１対１」となる。
 * 無線の接続設定には簡易接続方式「WPS」を使用している。
 * 無線機器のSSID（固有ID）や暗号化キーは自動的に決められる。

Androidは4.0以降（API Level 14以降）でWi-Fi Directに対応しています。
接続方法はアクションインテントを用いて接続可能な機器を探し、自動的に接続します。本項の前半で説明したWi-Fiネットワーク検索と同じようにWi-Fi Directで接続できる機器を探すことができます。
自ら接続可能な機器を探索する以外に、他の機器からの接続待ちの状態にすることも可能です。
他の機器から接続要求（invite）がきた場合、それに応えて接続します。

Wi-Fi Directは、あくまでも無線による接続機能のみの提供なので、接続後に何かしらの動作をさせたい場合には別途アプリケーションが必要になります。


#@# == WiFi情報利用例

#@# TODO 教科書には載せない

#@# == WiFi情報を取得の実践

#@# TODO 教科書には載せない
