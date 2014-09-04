= ネットワークプログラミング

この章ではHTTPサーバからファイルを取得するさまざまな方法を通してネットワーク通信の仕組みを学びます。
Webサーバーと連携して最新のデータを取得したり、他のプラットフォームと同期するためにデータを送信したりする際にネットワークの知識が必要になります。

== ネットワークってなんだろう

ネットワーク通信とは２つ以上のコンピュータでデータのやりとりをすることです。
ネットワークを話題にするときには、コンピュータのことをホストと呼びます。コンピュータ＝ホストと思って頂いて構いません。

たとえばAndroidでインターネットブラウザを使ってhttp://tomorrowkey.jp というウェブページを開いたとしましょう。
この時、Androidの携帯電話とインターネットの向こう側にあるホストとデータのやりとりをします（@<img>{image-of-network}）。

//image[image-of-network][AndroidでWebページを表示するとき]{
//}

ウェブページを要求することをリクエストといいます。またそのリクエストに対する返信をレスポンスといいます。
リクエストをしたら必ずレスポンスが返ってきます。
ネットワーク通信では、クライアントからサーバーにリクエストを送り、サーバからレスポンスを取得することをセットで考えます。

他のホストからのリクエストを待ち、データを提供するホストをサーバーといいます。
それに対してリクエストを送る側のホストをクライアントといいます。

== IPアドレス

インターネットにはたくさんのホストが繋がっています。
ネットワーク通信を行う際は、どのホストと通信するか指定する必要があります。
各ホストは異なる番号を持っていて、それをIPアドレス@<fn>{about-IPv6}といいます。
IPアドレスは次のように4つの数字をドットで区切った形式で表します（@<list>{ipaddress}）。

//footnote[about-IPv6][ここで説明しているのはIPv4と呼ばれるものです。容量を大きくしたIPv6というものもあります。詳しくは"IPアドレス枯渇問題"というワードで検索してみてください]

//list[ipaddress][IPアドレス]{
192.168.12.10
//}

みなさんが使っているコンピュータにもIPアドレスがあります。
あなたのコンピュータがどんなIPアドレスを使っているのか確認してみましょう。

IPアドレスを調べるには次のコマンドを実行します。
使用しているOSがMacやLinuxなどのUnix系の場合は次のコマンドをターミナルで実行します（@<list>{ifconfig}）。
//list[ifconfig][IPアドレスを調べるコマンド(OSX, Linuxなど)]{
> ifconfig
//}

使用しているOSがWindowsの場合は次のコマンドをコマンドプロンプトで実行します（@<list>{ipconfig}）。
//list[ipconfig][IPアドレスを調べるコマンド(Windows)]{
> ipconfig
//}

== 名前解決

前節でIPアドレスについて説明しましたが、1つ疑問が湧いてきました。
ブラウザでWebサーバにアクセスするときにはIPアドレスを指定せずにファイルのやりとりをしました。
ネットワーク通信する際にはIPアドレスの指定が必要と説明しましたが、ブラウザはどのようにIPアドレスを知ったのでしょうか。

それを実現するのが名前解決と呼ばれる技術です。DNS（Domain Name System）とも呼ばれます。
ネットワーク通信を行う際はIPアドレスを指定しなければなりませんが、実際その値は覚えにくく、人間に分かりやすいものではありません。

名前解決とは人間に分かりやすい文字列の"ホスト名"から"IPアドレス"に変換する処理のことをいいます。
また、その逆の変換も可能なので、名前解決では"ホスト名"とIPアドレスを相互に変換することができます。

さきほどブラウザでWebページにアクセスした時を例に具体的な説明をします。
ブラウザでは"http://tomorrowkey.jp/" という値を入力しました。これの"tomorrowkey.jp"という部分がホスト名です。
ブラウザはこの"ホスト名"からIPアドレスをDNSサーバに問い合わせます。
DNSサーバはこの"tomorrowkey.jp" という値から"49.212.164.150" というIPアドレスを返します。
これでIPアドレスが分かったので、ブラウザはサーバと通信を始めるのです。

また、この名前解決はターミナル/コマンドプロンプトで実行することもできます（@<list>{nslookup}）。
//list[nslookup][名前解決のコマンド]{
nslookup "ホスト名"
//}

実行すると次のようにサーバのIPアドレスを取得できます（@<list>{result-of-nslookup}）。
//list[result-of-nslookup][nslookupの実行結果]{
$ nslookup tomorrowkey.jp
Server:   220.159.212.200
Address:  220.159.212.200#53

Non-authoritative answer:
Name: tomorrowkey.jp
Address: 49.212.164.150
//}

名前とIPアドレスは同じものなので、さきほど取得したIPアドレスをブラウザに入力した場合も"http://tomorrowkey.jp/"と入力した場合と同じページが表示されます。

== プロトコル
名前解決を使ってIPアドレスを取得でき、サーバを特定しました。
2つのホストが結ばれたとき、どのような内容のデータをやりとりするのでしょうか。

データをどのような順番で送るかという"方法"をあらかじめ決めておかなければ、
サーバからクライアントに送られてきたデータにどういう意味があるのか、クライアントは知ることができません。
その"方法"の総称をプロトコルといいます。

さきほど説明したDNSもプロトコルの一種です。
DNSは"名前をDNSサーバに渡すことでIPアドレスを返す"という仕組みがあらかじめ決められています。

Webサーバーからページを取得するときに使われるプロトコルをHTTPといいます。
ホストに対して特定のページがほしいとリクエストを送ると、そのページがレスポンスとして返されます。

ブラウザで"http://tomorrowkey.jp/" にアクセスした場合を例にとって、具体的な値を説明しましょう。
ブラウザはサーバに対して次のようなリクエストを送っています（@<list>{http-request}）。

//list[http-request][HTTPリクエスト]{
GET / HTTP/1.0
User-Agent: Mozilla/5.0 (Linux; Android 4.3; Build/LPV79)
 AppleWebKit/537.36 (KHTML, like Gecko) 
 Chrome/36.0.1985.94 Mobile Safari/537.36
Host: tomorrowkey.jp
//}

1行目にはHTTPアクセスする概要を指定します。

まず最初のパラメータで、アクセスの種類を指定します。
上記リクエストでは"GET"となっていますが、これはファイルの取得を表します。
スペースで区切って、1行目2つ目のパラメータとして取得するファイルの名前を指定します。
"/"と書かれていますが、Webサーバでは"/"とリクエストされたものはたいてい"index.html"と解釈されます。
さらにスペースで区切った後ろにHTTPのバージョンを書きます。

2行目以降はヘッダーと呼ばれるリクエストする際の付属的な情報を書きます。
User-Agentは、ブラウザの種類を表します。
Hostは、アクセスするホストの名前を指定します。

このリクエストは実際にコマンドとして実行することができます。LinuxやMacをご使用の方はターミナルやコンソールを開いて@<list>{request-http-via-telnet} を実行してください。

//footnote[telnet-for-windows][Windowsにも標準でtelnetがインストールされているのですが、HTTPサーバーとのやりとりではうまく動作しないためPuTTY(http://www.chiark.greenend.org.uk/~sgtatham/putty/ ) などのtelnetクライアントをダウンロードする必要があります。]

//list[request-http-via-telnet][telnetを使ったHTTP通信のリクエスト]{
telnet tomorrowkey.jp 80
GET / HTTP/1.0
User-Agent: telnet
Host: tomorrowkey.jp

//}

最後に改行を2回することを忘れないようにしてください。
実行してみると次のようなレスポンスが返ってきます（@<list>{response-ofhttp-via-telnet}）。

//list[response-ofhttp-via-telnet][telnetを使ったHTTP通信のレスポンス]{
HTTP/1.1 200 OK
Date: Sun, 13 Jul 2014 06:20:10 GMT
Server: Apache/2.2.15 (CentOS)
Last-Modified: Sat, 07 Jun 2014 14:29:18 GMT
ETag: "26133e-f3-4fb3fcdaabf43"
Accept-Ranges: bytes
Content-Length: 243
Connection: close
Content-Type: text/html

<html>
<head>
<title>Hello, Tomorrow!</title>
</head>
<body>
<h1>Hello, Tomorrow!</h1>
</body>
</html>

//}

ブラウザではこのレスポンスを解釈して、Webページを表示しています。

== ポート番号
１つのホストでは複数のソフトウェアが動いていています。ネットワーク通信をするためにただIPアドレスを指定しただけでは、そのホストのどのソフトウェアとやりとりをしたいのかが分かりません。
リクエストを送るソフトウェアを特定するための番号をポート番号といいます。

各プロトコルによって使われるポート番号は異なります。一部のプロトコルについては一般的に使われるポート番号が決まっています。それらをWell-Known Portといいます。
例えばDNSであれば53で、HTTPであれば80です。すべてを覚える必要がありませんが、よく使われるポート番号くらいは覚えておくと便利です。

== Androidアプリでネットワークを使うために

Androidアプリでインターネットに接続するにはandroid.permission.INTERNETというパーミッションが必要です。あらかじめAndroidManifestに宣言します（@<list>{internet-permission}）。

//list[internet-permission][インターネットパーミッションを追加する]{
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
          package="com.example.sample.network">

  <uses-permission android:name="android.permission.INTERNET" />

  <application
   ...
//}

== Androidで使用できる通信方式

Android搭載端末の多くは次のような通信方式を持っています。

 * Mobile
 ** 3G
 ** LTE
 ** WiMAX
 * Wi-Fi
 * Bluetooth
 * NFC

3GにもCDMAやHSDPAなど種類がありますがアプリ開発者が意識することはあまりなく、意識することがあってもWi-Fiに接続中なのか、もしくはMobileなのかくらいです。
厳密にはもうすこし通信方式はありますが、あまり一般的ではないので省略します。
この章ではHTTP通信を行う通信方法を紹介します。上記リストの中ではMobileとWi-Fiが該当します。

== モバイルという性質
Androidのアプリケーションを語る上で、サーバーのアプリケーションやデスクトップ向けアプリケーションとくらべて、２つの大きな特徴があります。
一つはネットワークが不安定ということと、もう一つは電池消費量に気をつけなくてはならないということです。

=== ネットワークは不安定

Androidを搭載した端末の多くは携帯電話です。さまざまな場所に持ち歩く携帯電話なので、必ずしもネットワークに恵まれた場所にいるとは限りません。
アプリを作るにあたって、いつネットワークが切れても問題ないように作らなくてはなりません。
また、ネットワークが切れてしまった場合にユーザーにストレスをかけないような作りを心がけることも重要です。

#@# どのようなつくりがストレスを与えない？

=== 電池消費量

モバイル環境下では電池消費量にも気を配らなくてはなりません。モバイルで電池を消耗しやすい機能の１つがネットワーク通信です。
できるだけ通信量を減らす、通信回数を減らす、一度にまとめて通信するといった事を心がけると、電池消費にも優しいアプリを作ることができます。

== メインスレッドと非同期スレッド
理論値では100Mbpsを越えるネットワークでも環境によっては数百bpsしかでないため、小さいデータのやりとりであっても時間がかかるものだと考えるべきです。
時間のかかる処理はUIスレッドで行ってはいけません。必ず非同期処理ができる仕組み（AsyncTaskなど）を使うようにしましょう。

== ソケットプログラミング
Androidで一番低レベルなネットワーク通信方法はSocketを使ったやり方です。
Socketを使うことで、OSI参照モデルではトランスポート層以下の機能が提供されます。
HTTPサーバーにアクセスするにはすこし貧弱ですが、HTTP以外のサーバにもアクセスできるため便利です。
たとえばhttp://tomorrowkey.github.io にアクセスしようとした場合、次のようなプログラムになります（@<list>{basic_implemention_of_socket}）。

//list[basic_implemention_of_socket][Socketの実装方法の例]{
try {
  // リクエスト
  Socket socket = new Socket();
  socket.connect(new InetSocketAddress("tomorrowkey.github.io", 80));

  String request = "GET / HTTP/1.1\n" +
    "Host: tomorrowkey.github.io\n" +
    "\n\n";
  OutputStream outputStream = socket.getOutputStream();
  outputStream.write(request.getBytes());
  outputStream.flush();

  // レスポンス
  InputStream inputStream = socket.getInputStream();
  byte[] buffer = new byte[1024];
  int length;
  while ((length = inputStream.read(buffer)) != -1) {
    Log.d("TEST", new String(buffer, 0, length));
  }

  outputStream.close();
  inputStream.close();
} catch (UnknownHostException e) {
  throw new RuntimeException(e);
} catch (IOException e) {
  throw new RuntimeException(e);
}
//}

どのような処理をしているか、順番に見ていきましょう。

=== リクエストを送信する
まずはSocketインスタンスを生成します（@<list>{make-instance-of-socket}）。
//list[make-instance-of-socket][Socketインスタンスの生成]{
Socket socket = new Socket();
//}

Socketインスタンスができたら、ホスト名とポート番号の接続先を指定します。
今回はHTTPなので、ポート番号は80番になります（@<list>{set-host-and-port-number}）。
//list[set-host-and-port-number][ホスト名とポート番号の指定]{
socket.connect(new InetSocketAddress("tomorrowkey.github.io", 80));
//}

Socketでリクエストする場合、OutputStreamに直接HTTPリクエストを書きます（@<list>{request-with-socket}）。
//list[request-with-socket][リクエストの送信]{
OutputStream outputStream = socket.getOutputStream();
outputStream.write(request.getBytes());
outputStream.flush();
//}

=== リクエストの組み立て

Socketで通信を行う場合、プロトコルに従った通信内容を自分で組み立てる必要があります。
今回はHTTP通信なので、HTTPのプロトコルに従ったリクエストを自分で組み立てます。

Webサーバからページを取得する命令はGETです。
一番単純なGETリクエストは次のようになります（@<list>{get-request}）。

//list[get-request][GETリクエスト]{
GET / HTTP/1.1
Host: tomorrowkey.github.io

//}

1行目には大まかなリクエスト内容を送ります（@<table>{the-first-line-of-get-request}）。
"/"とリクエストすると、たいていのWebサーバでは"/index.html" と解釈されます。

//table[the-first-line-of-get-request][GETリクエスト1行目]{
-----------------------
GET				ファイルを取得する
/					"/"というファイルを取得する
HTTP/1.1	HTTP/1.1というプロトコル（手続き方法）を使います
//}

2行目からはリクエストヘッダ（リクエスト時の付属情報）を送信します（@<table>{the-second-line-of-get-request}）。

//table[the-second-line-of-get-request][GETリクエスト2行目]{
-----------------------
Host:									ヘッダのキーです
tomorrowkey.github.io	Hostキーはリクエスト先のホスト名を指定します
//}

リクエストの最後に改行を2回送るとリクエスト完了となります。

=== レスポンスを受け取る

socketからInputStreamを取得してHTTPレスポンスを取得します（@<list>{get-response-body-with-socket}）。

//list[get-response-body-with-socket][レスポンスの取得]{
InputStream inputStream = socket.getInputStream();
byte[] buffer = new byte[1024];
int length;
while ((length = inputStream.read(buffer)) != -1) {
  Log.d("TEST", new String(buffer, 0, length));
}
//}

レスポンスの取得まで完了したらInputStreamとOutputStreamのcloseメソッドを呼んでStreamを閉じましょう。

=== 実行結果
実行するとlogcatにレスポンスが出力されます（@<list>{response-of-socket-request}）。
//list[response-of-socket-request][Socketを使ったリクエストのレスポンス]{
D/TEST    ( 1371): HTTP/1.1 200 OK
D/TEST    ( 1371): Server: GitHub.com
D/TEST    ( 1371): Content-Type: text/html; charset=utf-8
D/TEST    ( 1371): Last-Modified: Mon, 02 Jan 2012 05:54:49 GMT
D/TEST    ( 1371): Expires: Mon, 30 Jun 2014 11:37:13 GMT
D/TEST    ( 1371): Cache-Control: max-age=600
D/TEST    ( 1371): Content-Length: 169
D/TEST    ( 1371): Accept-Ranges: bytes
D/TEST    ( 1371): Date: Mon, 30 Jun 2014 12:24:02 GMT
D/TEST    ( 1371): Via: 1.1 varnish
D/TEST    ( 1371): Age: 3409
D/TEST    ( 1371): Connection: keep-alive
D/TEST    ( 1371): X-Served-By: cache-ty66-TYO
D/TEST    ( 1371): X-Cache: MISS
D/TEST    ( 1371): X-Cache-Hits: 0
D/TEST    ( 1371): X-Timer: S1404131042.773465872,VS0,VE174
D/TEST    ( 1371): Vary: Accept-Encoding
D/TEST    ( 1371): 
D/TEST    ( 1371): <html>
D/TEST    ( 1371): <!DOCTYPE html>
D/TEST    ( 1371): <html lang="ja">
D/TEST    ( 1371): <head>
D/TEST    ( 1371): <title>tomorrowkey GitHub page</title>
D/TEST    ( 1371): <meta charset="UTF-8" />
D/TEST    ( 1371): </head>
D/TEST    ( 1371): <body>
D/TEST    ( 1371): <h1>Hello, tomorrow!!</h1>
D/TEST    ( 1371): </body>
D/TEST    ( 1371): </html>
//}

HTTPレスポンスのheaderとbody両方が出力されました。
Socketで通信する場合はこの文字列をJavaのオブジェクトに変換する処理が必要です。
Socket通信はリクエストを自分で組み立てないといけなかったり、レスポンスが生データのままなので大変ですが、とても自由度が高いのが特徴です。

== HttpURLConnection

Socketではリクエストを自分で組み立てなければなりませんでしたが、HTTP/HTTPSにアクセスする場合もっと便利なクラスがあります。それがHttpURLConnectionです。
それでは先ほどと同様http://tomorrowkey.github.ioにアクセスしてみましょう（@<list>{basic-implemention-of-http-url-connection}）。

//list[basic-implemention-of-http-url-connection][HttpURLConnectionの実装方法]{
try {
  URL url = new URL("http://tomorrowkey.github.io");
  HttpURLConnection connection = (HttpURLConnection) url.openConnection();
  connection.setRequestMethod("GET");
  connection.setRequestProperty("Host", "tomorrowkey.github.io");
  connection.connect();

  int responseCode = connection.getResponseCode();
  Log.d("TEST", "responseCode=" + responseCode);
  String contentLength = connection.getHeaderField("Content-Length");
  Log.d("TEST", "Content-Length=" + contentLength);
  String contentType = connection.getHeaderField("Content-Type");
  Log.d("TEST", "contentType=" + contentType);

  InputStream inputStream = connection.getInputStream();
  String body = readToEnd(inputStream);
  Log.d("TEST", "body=" + body);
  inputStream.close();
} catch (MalformedURLException e) {
  throw new RuntimeException(e);
} catch (IOException e) {
  throw new RuntimeException(e);
}
//}

HttpURLConnectionでは、どのような処理をしているか順番に見ていきましょう。

=== リクエストを送信する

アクセスするURLを使いURLオブジェクトを作ります（@<list>{make-a-instance-of-url}）。
//list[make-a-instance-of-url][URLオブジェクトの生成]{
URL url = new URL("http://tomorrowkey.github.io");
//}

openConnection()メソッドを使い、HttpURLConnectionを取得します（@<list>{get-http-url-connection}）。
//list[get-http-url-connection][HTTPUrlConnectionの取得]{
HttpURLConnection connection = (HttpURLConnection) url.openConnection();
//}
注意点として、URL.openConnection()メソッドの戻り値はURLConnectionなのでHttpURLConnectionにパース（変換）しなければならないということです。

リクエストはファイルの取得なので、setRequestMethod()メソッドで"GET"を渡します。
また、setRequestProperty()メソッドを使うことでリクエストヘッダーを設定できます。
Hostは設定しなくても自動的に追加されるのですが、説明上、あえて追加しました（@<list>{set-request-method}）。
//list[set-request-method][リクエストの設定]{
connection.setRequestMethod("GET");
connection.setRequestProperty("Host", "tomorrowkey.github.io");
//}

connectメソッドでサーバと接続します（@<list>{connect-to-the-server}）。
//list[connect-to-the-server][サーバとの接続]{
connection.connect();
//}

=== レスポンスを受け取る

ステータスコードを取得する場合、getResponseCode()メソッドを呼びます（@<list>{get-status-code-from-http-url-connection}）。

//list[get-status-code-from-http-url-connection][ステータスコードの取得]{
int responseCode = connection.getResponseCode();
Log.d("TEST", "responseCode=" + responseCode);
//}

レスポンスヘッダを取得する場合、getHeaderField()メソッドを呼びます（@<list>{get-headers-from-http-url-connection}）。

//list[get-headers-from-http-url-connection][レスポンスヘッダの取得]{
String contentLength = connection.getHeaderField("Content-Length");
Log.d("TEST", "Content-Length=" + contentLength);
String contentType = connection.getHeaderField("Content-Type");
Log.d("TEST", "contentType=" + contentType);
//}

レスポンスボディを取得する場合、getInputStream()メソッドを呼びます（@<list>{get-body-from-http-url-connection}）。
//list[get-body-from-http-url-connection][レスポンスボディの取得]{
InputStream inputStream = connection.getInputStream();
String body = readToEnd(inputStream);
Log.d("TEST", "body=" + body);
//}
InputStreamから文字列に変換する処理はSocketと同じなので、効率化のためメソッド化しています。

=== 実行結果

実行結果は次のとおりです（@<list>{response-of-http-url-connection-request}）。

//list[response-of-http-url-connection-request][HttpURLConnectionを使ったリクエストのレスポンス]{
D/TEST    ( 1231): responseCode=200
D/TEST    ( 1231): Content-Length=null
D/TEST    ( 1231): contentType=text/html; charset=utf-8
D/TEST    ( 1231): body=<html>
D/TEST    ( 1231): <!DOCTYPE html>
D/TEST    ( 1231): <html lang="ja">
D/TEST    ( 1231): <head>
D/TEST    ( 1231): <title>tomorrowkey GitHub page</title>
D/TEST    ( 1231): <meta charset="UTF-8" />
D/TEST    ( 1231): </head>
D/TEST    ( 1231): <body>
D/TEST    ( 1231): <h1>Hello, tomorrow!!</h1>
D/TEST    ( 1231): </body>
D/TEST    ( 1231): </html>
//}

Socketとは異なりHTTPプロトコルを楽に使えるようなメソッドが用意されているため、簡単に取り扱うことができます。

== HttpClient (Apache Http)
GET / POST
InputStream / OutputStream

AndroidではApacheのHttpClientが標準で入っているので、HttpClientも使うことができます。
こちらも先ほどと同様のアクセスをしてみましょう（@<list>{basic-implementation-of-http-client}）。

//list[basic-implementation-of-http-client][HttpClientの実装方法]{
try {
  HttpGet httpGet = new HttpGet("http://tomorrowkey.github.io");
  httpGet.addHeader("Host", "tomorrowkey.github.io");

  HttpClient httpClient = new DefaultHttpClient();
  HttpResponse httpResponse = httpClient.execute(httpGet);

  InputStream inputStream = httpResponse.getEntity().getContent();
  int length;
  byte[] buffer = new byte[1024];
  while ((length = inputStream.read(buffer)) != -1) {
    Log.d("TEST", new String(buffer, 0, length));
  }
  inputStream.close();
} catch (MalformedURLException e) {
  throw new RuntimeException(e);
} catch (IOException e) {
  throw new RuntimeException(e);
}
//}

HttpClientでは、どのような処理になるのか順番に見ていきましょう。

=== リクエストを送信する
GETリクエストなので、HttpGetオブジェクトを作ります。引数にはアクセスするURLを指定します（@<list>{make-a-instance-of-http-get}）。

//list[make-a-instance-of-http-get][HttpGetオブジェクトの生成]{
HttpGet httpGet = new HttpGet("http://tomorrowkey.github.io");
//}

ヘッダの指定はaddHeaderメソッドを使います（@<list>{add-header-to-http-get}）。
//list[add-header-to-http-get][ヘッダーの追加]{
httpGet.addHeader("Host", "tomorrowkey.github.io");
//}

実際に通信を行うのはHttpClientというオブジェクトなので、HttpClientを生成します。
今回はDefaultHttpClientを使ってHttpClientを生成していますが、AndroidHttpClientを使うこともできます（@<list>{make-a-instance-of-http-client}）。

#@# DefaultHttpClientとAndroidHttpClientの違いに触れてください。

//list[make-a-instance-of-http-client][HttpClientの生成]{
HttpClient httpClient = new DefaultHttpClient();
//}

生成したhttpClientのexecute()メソッドにリクエストオブジェクト(httpGet)を渡すことで通信をします（@<list>{communicate-with-http-client}）。
//list[communicate-with-http-client][HttpClientを使った通信]{
HttpResponse httpResponse = httpClient.execute(httpGet);
//}
通信したレスポンスはHttpResponseというクラスで戻り値に返ってきます。

=== レスポンスを受け取る

ステータスコードを取得するには、まずはgetStatusLine()メソッドでステータスラインを取得した後に、getStatusCode()メソッドを呼びます（@<list>{get-status-code}）。

//list[get-status-code][ステータスコードの取得]{
StatusLine statusLine = httpResponse.getStatusLine();
Log.d("TEST", "Status-Code=" + statusLine.getStatusCode());
//}

ステータスラインとはHTTPレスポンスの次の部分を指します（@<list>{status-line}）。
//list[status-line][ステータスライン]{
HTTP/1.1 200 OK
//}

レスポンスヘッダを取得するには、getFirstHeader()メソッドでHeaderオブジェクトを取得します。
Headerオブジェクトの値を取得するにはgetValue()メソッドを呼びます（@<list>{get-header}）。
//list[get-header][ヘッダーの取得]{
Header contentLengthHeader = httpResponse.getFirstHeader("Content-Length");
Log.d("TEST", "Content-Length=" + contentLengthHeader.getValue());
Header contentTypeHeader = httpResponse.getFirstHeader("Content-Type");
Log.d("TEST", "Content-Type=" + contentTypeHeader.getValue());
//}

レスポンスボディを取得する場合、getInputStream()メソッドを呼びます（@<list>{get-response-body}）。

//list[get-response-body][レスポンスボディの取得]{
InputStream inputStream = httpResponse.getEntity().getContent();
String body = readToEnd(inputStream);
Log.d("TEST", body);
inputStream.close();
//}

リクエストとレスポンスのオブジェクトが分かれているので分かりやすいですね。

=== 実行結果

実行結果は次のとおりです（@<list>{response-of-http-client-request}）。

//list[response-of-http-client-request][HttpClientを使ったリクエストのレスポンス]{
D/TEST    ( 1295): Status-Code=200
D/TEST    ( 1295): Content-Length=169
D/TEST    ( 1295): Content-Type=text/html; charset=utf-8
D/TEST    ( 1295): <html>
D/TEST    ( 1295): <!DOCTYPE html>
D/TEST    ( 1295): <html lang="ja">
D/TEST    ( 1295): <head>
D/TEST    ( 1295): <title>tomorrowkey GitHub page</title>
D/TEST    ( 1295): <meta charset="UTF-8" />
D/TEST    ( 1295): </head>
D/TEST    ( 1295): <body>
D/TEST    ( 1295): <h1>Hello, tomorrow!!</h1>
D/TEST    ( 1295): </body>
D/TEST    ( 1295): </html>
//}

== ライブラリを使ったネットワーク通信

ネットワーク通信をする度にAsyncTaskを継承して、同じようなバックグラウンド処理を書くのは大変です。
バックグラウンド処理を毎回書かなくてもいいようなライブラリがGoogleから公開されています。名前はVolley@<fn>{volley}といいます。

//footnote[volley][https://android.googlesource.com/platform/frameworks/volley/]

Volleyは他のライブラリのようにjarファイルが公開されておらず、maven repositoryにて公開もされていません。
AOSPでソースコードが管理されているので、そこからjarファイルを作る必要があります。
次のコマンドを実行することでjarファイルをビルドできます（@<list>{compile-volley}）。

//list[compile-volley][Volleyのビルド]{
git clone https://android.googlesource.com/platform/frameworks/volley
cd volley
ant jar
//}

生成されたjarファイルをlibsディレクトリに入れてソースコードから参照できるようにしましょう。

JSONファイルをダウンロードしてログに出力するプログラムを実装しましょう（@<list>{download-json-file-with-volley}）。

//list[download-json-file-with-volley][APIアクセス]{
mRequestQueue = Volley.newRequestQueue(getApplicationContext());

int method = Request.Method.GET;
String url = "https://raw.githubusercontent.com/TechBooster/
    AndroidOpenTextbook/master/code/network/assets/sample.json";
JSONObject requestBody = null;
Response.Listener<JSONObject> listener = new Response.Listener<JSONObject>() {
  @Override
  public void onResponse(JSONObject jsonObject) {
    Log.d("TEST", jsonObject.toString());
  }
};
Response.ErrorListener errorListener = new Response.ErrorListener() {
  @Override
  public void onErrorResponse(VolleyError volleyError) {
    String message = volleyError.getMessage();
    Log.d("TEST", message);
  }
};

mRequestQueue.add(new JsonObjectRequest(method,
     url, requestBody, listener, errorListener));
//}

=== リクエストを送信する

まずはリクエストキューを作ります。
この作成されたリクエストキューにリクエストを追加することで、自動的にバックグラウンドで随時リクエストを送ります。
リトライ処理についてもある程度行ってくれます（@<list>{make-a-instance-of-request-queue}）。
//list[make-a-instance-of-request-queue][リクエストキューの生成]{
mRequestQueue = Volley.newRequestQueue(getApplicationContext());
//}

リクエストキューを生成したので、リクエストを追加します。
リクエストを追加する際には次のパラメータが必要です（@<table>{the-second-line-of-get-request2}）。

//table[the-second-line-of-get-request2][GETリクエスト2行目]{
-----------------------
method				リクエストメソッドを指定する
url						アクセスするURLを指定する
requestBody		リクエスト時にボディに送るJSONObjectを指定する
listener			レスポンスリスナー
errorListener	エラーレスポンスリスナー
//}

レスポンスリスナーは、正常系のステータスコード（200～299）の場合に実行されます。またエラーレスポンスリスナーは異常系のステータスコード（200～299以外）の場合に実行されます。


リクエストをリクエストキューに追加すると自動的にバックグラウンドでネットワーク通信が行われ、
ネットワーク通信が完了すると引数に渡したコールバック（listener、errorListener）が実行されます。

=== レスポンス
サーバーのレスポンスが正常系（200～299）だった場合、第４引数のlistenerのコールバックメソッドが実行されます。
レスポンスボディは自動的にJSONObjectにパースされ、引数に渡されます（@<list>{succeed_response-listener}、@<list>{succeed-response}）。

//list[succeed_response-listener][正常系レスポンスリスナー]{
@Override
public void onResponse(JSONObject jsonObject) {
  Log.d("TEST", jsonObject.toString());
}
//}

//list[succeed-response][正常系なレスポンス]{
D/TEST    ( 1699): {"users":[
    {"id":1,"gender":"female","name":"alice"},
    {"id":2,"gender":"male","name":"bob"}]}
//}

サーバのレスポンスが異常系(200～299以外)だった場合、第５引数のerrorListenerのコールバックメソッドが実行されます。
エラーの内容は引数のVolleyErrorオブジェクトに入っています（@<list>{error-response-listener}、@<list>{error-response}）。

//list[error-response-listener][エラー系レスポンスリスナー]{
@Override
public void onErrorResponse(VolleyError volleyError) {
  NetworkResponse networkResponse = volleyError.networkResponse;
  int statusCode = networkResponse.statusCode;
  Log.d("TEST", "Status-Code=" + statusCode);

  String contentLength = networkResponse.headers.get("Content-Length");
  Log.d("TEST", "Content-Length=" + contentLength);

  String body = new String(networkResponse.data);
  Log.d("TEST", body);
}
//}

//list[error-response][エラーレスポンス]{
D/TEST    ( 1654): Status-Code=404
D/TEST    ( 1654): Content-Length=9
D/TEST    ( 1654): Content-Type=null
D/TEST    ( 1654): Not Found
//}

Volleyを使うと非同期処理を書かなくてよいのでとても便利ですね。このようなライブラリを使うと独自で実装するよりも手間が省けます。
その一方、実装の自由度は低くなりがちなことを覚えておくとよいでしょう。とはいっても、
一般的なネットワーク通信において、Volleyのようなライブラリは非常に有用です。

== WebAPIにアクセスする

各種オンラインサービスはApplication Programming Interface(API)を公開していることがあります。
たとえばTwitterのAPIを使うと、自分だけのオリジナルのTwitterアプリを作ることができます。
また天気予報サービスのAPIを使えば、自分好みの天気予報アプリを作れます。

本節では、WebAPIを使う上で気をつけなければならないことを挙げて解説します。

==== 料金体系

Web APIを使うための料金体系は様々です。
完全に無料のものもありますが、たいていは一定期間のアクセス数に応じて制限がかかったり、有料になったりします。

==== 利用規約を読む

すべてのWeb APIには利用規約があり、使用するための条件があります。
たとえば、オープンソースアプリや無料で公開するアプリについては無料でAPIを使うことができるが、
広告を掲載したり有償でアプリを公開する場合はAPI使用料を払わないといけないケースがあります。

他にもAPIを使用するにあたって気をつけなければならないことが利用規約に書いてありますので、必ず使用する前に熟読しましょう。
もしこの利用規約を守れなかった場合、予告のないサービスの提供停止や、場合によってはサービス提供元から法的手段をとられる場合も考えられるので注意しましょう。

== ソーシャルIMEを使ってみよう

ここではソーシャルIME@<fn>{IME}というサービスを使って入力したひらがなを変換してみましょう。

//footnote[IME][Social IME ～みんなで育てる日本語入力～ http://www.social-ime.com/]

Input Method Editor(IME)は文字入力を補助するソフトウェアです。言葉で説明するとすごく分かりづらいですが、
ざっくりとわかりやすく説明すると、キーボードを使って入力するときにひらがなを漢字に変換してくれるソフトウェアです。
ソーシャルIMEでは変換するための仕組みをWebAPIとして提供しています。

ソーシャルIMEは簡単に試すことができます。
たとえば次のURLにブラウザでアクセスしてみてください（@<list>{url-of-social-ime}）。

//list[url-of-social-ime][Social IMEの簡単なリクエスト]{
http://www.social-ime.com/api/?string=けいたいでんわ
//}

次のように変換された結果がタブ区切りで表示されます（@<list>{result-of-access-to-social-ime}）。

//list[result-of-access-to-social-ime][Social IMEのレスポンス]{
携帯電話  けいたいでんわ ケイタイデンワ 
//}

それではこれをAndroidで実装してみましょう（@<list>{access-to-social-ime-with-android}）。

//list[access-to-social-ime-with-android][Social IME]{
try {
  String keyword = params[0];
  URL url = new URL("http://www.social-ime.com/api/?string=" + keyword);
  HttpURLConnection connection = (HttpURLConnection) url.openConnection();
  connection.connect();
  InputStream inputStream = connection.getInputStream();

  StringBuilder sb = new StringBuilder();
  int length;
  byte[] buffer = new byte[1024];
  while ((length = inputStream.read(buffer)) != -1) {
    sb.append(new String(buffer, 0, length, "EUC-JP"));
  }
  return sb.toString();
} catch (IOException e) {
  throw new RuntimeException(e);
}
//}

基本的には前述したHttpURLConnectionを使ったアクセスと同じです。

気をつけなければならないのは、byte配列から文字列にする際に指定する文字コードです。
ソーシャルIMEでは特に指定しなければレスポンスは"EUC-JP"という文字コードでレスポンスを返すプロトコルを採用しています。
受け取ったbyte配列からそのまま文字列を生成すると文字化けを起こしてしまうので、文字コードを指定します。
WebAPIを使う際に文字化けを起こしてしまうことはよくあるので注意しましょう。

WebAPIごとにリクエスト、レスポンスのフォーマットが異なるため、仕様に合わせてリクエストを組み立て、レスポンスをパースしてください。

== まとめ

さまざまな方法を使ってサーバーからファイルを取得する方法を実装しました。
やり方がたくさんあってどれを使っていいか迷うかもしれませんが、それぞれの実装方法の特徴とやりたいことを比較して、最適な方法を選択してください。

なにかアプリを作りたいけど、アイデア浮かばない！というときにWebAPIを眺めていると、
あのAPIとこのAPIを組み合わせれば面白いことができるぞ！なんて思いつくことがあります。
たいていのWebAPIはHTTP通信ができれば使えるので、おもしろいWebAPIを見つけて奇抜なアプリを作りましょう！

