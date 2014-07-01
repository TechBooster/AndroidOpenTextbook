= ネットワーク

== Androidアプリでネットワークを使うための知識

Permission

インターネットに接続するには以下のパーミッションが必要です。

=== 通信方式

Androidが搭載される端末は以下のような通信方式を持っています。

 * Mobile
 ** 3G
 ** LTE
 ** WiMAX
 * Wi-Fi
 * Bluetooth
 * NFC

3GにもCDMAやHSDPAなど種類がありますがアプリ開発者が意識することはあまりなく、意識することがあってもWi-Fiに接続中なのか、もしくはMobileなのかくらいです。
厳密にはもうすこし通信方式はありますが、あまり一般的ではないので省略します。

=== モバイルという性質
ネットワークと語る上で、サーバーのアプリケーションやデスクトップ向けアプリケーションとくらべて、２つの大きな特徴があります。
一つはネットワークは不安定ということと、もう一つは電池消費量に気をつけなくてはならないということです。

==== ネットワークは不安定

Androidを搭載した端末の多くは携帯電話です。様々な場所に持ち歩く携帯電話なので、必ずしもネットワークに恵まれた場所にいるとは限りません。
アプリを作るにあたっていつネットワークが切れても問題ないように作らなくてはなりません。
また、ネットワークが切れてしまった場合にユーザーにストレスをかけないような作りを心がけることも重要です。

==== 電池消費量

電池消費量にも気を配らなくてはなりません。モバイルで電池を消耗しやすい機能の１つがネットワーク通信です。
できるだけ通信料を減らす、通信回数を減らす、一度まとめてに通信するといった事を心がけると、電池消費にも優しいアプリを作ることができます。

=== メインスレッドと非同期スレッド
理論値では100Mbpsを越えるネットワークでも環境によっては数百bpsしかでないため、小さいデータのやりとりであっても時間がかかるものだと考えるべきです。
時間のかかる処理はUIスレッドで行ってはいけません。必ず非同期処理ができる仕組み（AsyncTaskなど）を使うようにしましょう。

== ソケットプログラミング
Androidで一番低レベルなネットワーク通信方法はSocketを使ったやり方です。
HTTPサーバーにアクセスするにはすこし貧弱ですが、HTTP以外のサーバにアクセスすることができるため便利です。
例えばhttp://tomorrowkey.github.io にアクセスしようとした場合、以下のようなプログラムになります。

//list[basic_implemention_of_socket][Socketの実装方法]{
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

=== リクエスト
まずはSocketインスタンスを生成します。
//list[make-instance-of-socket][Socketインスタンスの生成]{
Socket socket = new Socket();
//}

Socketインスタンスができたら、ホスト名とポート番号の接続先を指定します。
今回はHTTPなので、ポート番号は80番になります。
//list[set-host-and-port-number][ホスト名とポート番号の指定]{
socket.connect(new InetSocketAddress("tomorrowkey.github.io", 80));
//}

Socketでリクエストする場合、OutputStreamに直接HTTPリクエストを書きます。
//list[request-with-socket][リクエストの送信]{
OutputStream outputStream = socket.getOutputStream();
outputStream.write(request.getBytes());
outputStream.flush();
//}

=== リクエストの組み立て

Socketで通信を行う場合、プロトコルに従った通信内容を自分で組み立てる必要があります。
今回はHTTP通信なので、HTTPのプロトコルに従ったリクエストを自分で組み立てます。

Webサーバからページを取得する命令はGETです。
一番単純なGETリクエストは以下のようになります。

//list[get-request][GETリクエスト]{
GET / HTTP/1.1
Host: tomorrowkey.github.io

//}

1行目には大まかなリクエスト内容を送ります
//table[the-first-line-of-get-request][GETリクエスト1行目]{
-----------------------
GET				ファイルを取得する
/					"/"というファイルを取得する。"/"とリクエストすると、大抵のWebサーバでは"/index.html" と解釈されます。
HTTP/1.1	HTTP/1.1というプロトコル（手続き方法）を使います
//}

2行目からはリクエストヘッダ（リクエスト時の付属情報）を送信します
//table[the-second-line-of-get-request][GETリクエスト2行目]{
-----------------------
Host:									ヘッダのキーです
tomorrowkey.github.io	Hostキーはリクエスト先のホスト名を指定します
//}

リクエストの最後に改行を2回送るとリクエスト完了となります。

=== レスポンス

socketからInputStreamを取得してHTTPレスポンスを取得します。

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
実行するとlogcatにレスポンスが出力されます。
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
Socketで通信する場合は、この文字列をJavaのオブジェクトに変換する処理が必要です。
リクエストを自分で組み立てないといけなかったり、レスポンスが生データのままなので大変ですが、とても自由度が高いです。

== HttpURLConnection

Socketではリクエストを自分で組み立てなければなりませんでしたが、HTTP/HTTPSにアクセスする場合もっと便利なクラスがあります。それがHttpURLConnectionです。
それでは先ほどと同様http://tomorrowkey.github.ioにアクセスしてみましょう。

//list[basic-implemention-of-http-url-connection][HttpURLConnectionの実装方法]{
try {
  // 1
  URL url = new URL("http://tomorrowkey.github.io");
  HttpURLConnection connection = (HttpURLConnection) url.openConnection();

  // 2
  connection.setRequestMethod("GET");

  // 3
  connection.connect();

  // 4
  InputStream inputStream = connection.getInputStream();
  int length;
  byte[] buffer = new byte[1024];
  while ((length = inputStream.read(buffer)) != -1) {
    Log.d("TEST", new String(buffer, 0, length));
  }

  // 5
  inputStream.close();
} catch (MalformedURLException e) {
  throw new RuntimeException(e);
} catch (IOException e) {
  throw new RuntimeException(e);
}
//}

1. アクセス先を指定してURLインスタンスを作ります。HttpURLConnectionを作るにはurl.openConnection()を呼ぶ必要があります。さらにこの戻り値はURLConnectionなのでHttpURLConnectionにパースする必要があります。
2. 接続する情報を追加します。今回はGETアクセスするので、RequestMethodにGETを指定しています。例えばヘッダーを追加したい場合はここでヘッダを追加します。
3. サーバと接続します。
4. レスポンスを取得します。
5. 使い終わったリソースはcloseで閉じます

//list[response-of-http-url-connection-request][HttpURLConnectionを使ったリクエストのレスポンス]{
D/TEST    ( 1412): <html>
D/TEST    ( 1412): <!DOCTYPE html>
D/TEST    ( 1412): <html lang="ja">
D/TEST    ( 1412): <head>
D/TEST    ( 1412): <title>tomorrowkey GitHub page</title>
D/TEST    ( 1412): <meta charset="UTF-8" />
D/TEST    ( 1412): </head>
D/TEST    ( 1412): <body>
D/TEST    ( 1412): <h1>Hello, tomorrow!!</h1>
D/TEST    ( 1412): </body>
D/TEST    ( 1412): </html>
//}

== HttpClient (Apache Http)
GET / POST
InputStream / OutputStream

AndroidではApacheのHttpClientも使うことができます。
こちらも先ほどと同様のアクセスをしてみましょう。

//list[basic-implementation-of-http-client][HttpClientの実装方法]{
try {
  // 1
  HttpGet httpGet = new HttpGet("http://tomorrowkey.github.io");

  // 2
  HttpClient httpClient = new DefaultHttpClient();

  // 3
  HttpResponse httpResponse = httpClient.execute(httpGet);

  // 4
  InputStream inputStream = httpResponse.getEntity().getContent();
  int length;
  byte[] buffer = new byte[1024];
  while ((length = inputStream.read(buffer)) != -1) {
    Log.d("TEST", new String(buffer, 0, length));
  }

  // 5
  inputStream.close();
} catch (MalformedURLException e) {
  throw new RuntimeException(e);
} catch (IOException e) {
  throw new RuntimeException(e);
}
//}

1. リクエストオブジェクトを作ります。
2. HttpClientのインスタンスを作ります。DefaultHttpClientを使っていますが、この他にAndroidHttpClientを使う方法もあります。
3. リクエストを実行します。
4. レスポンスを取得します。
5. 使い終わったリソースはcloseで閉じます

リクエストとレスポンスのオブジェクトが分かれている分分かりやすいです。

//list[response-of-http-client-request][HttpClientを使ったリクエストのレスポンス]{
D/TEST    ( 1489): <html>
D/TEST    ( 1489): <!DOCTYPE html>
D/TEST    ( 1489): <html lang="ja">
D/TEST    ( 1489): <head>
D/TEST    ( 1489): <title>tomorrowkey GitHub page</title>
D/TEST    ( 1489): <meta charset="UTF-8" />
D/TEST    ( 1489): </head>
D/TEST    ( 1489): <body>
D/TEST    ( 1489): <h1>Hello, tomorrow!!</h1>
D/TEST    ( 1489): </body>
D/TEST    ( 1489): </html>
//}

== メッセージキューとか

== ライブラリを使ったネットワーク通信

 * Volley https://android.googlesource.com/platform/frameworks/volley/
 * okhttp https://github.com/square/okhttp
 * Picasso https://github.com/square/picasso





EOF