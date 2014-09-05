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