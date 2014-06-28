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
一つはネットワークが不安定ということと、もう一つは電池消費量に気をつけなくてはならないということです。

==== ネットワークが不安定

Androidを搭載した端末の多くは携帯電話です。様々な場所に持ち歩く携帯電話なので、必ずしもネットワークに恵まれた場所にいるとは限りません。
アプリを作るにあたっていつネットワークが切れても問題ないように作らなくてはなりません。
また、ネットワークが切れてしまった場合にユーザーにストレスをかけないような作りを心がけることも重要です。

==== 電池消費量

電池消費量にも気を配らなくてはなりません。モバイルで電池を消耗しやすい機能の１つがネットワーク通信です。
できるだけ通信料を減らす、通信回数を減らす、一度まとめてに通信するといった事を心がけると、電池消費にも優しいアプリを作ることができます。

=== メインスレッドと非同期スレッド
AndroidにはUIスレッドという特別なスレッドがあります。
このUIスレッドの役割は、UIコンポーネントの表示やユーザーの入力値などのやりとりなど、UIにまつわる部分です。
このUIスレッドの特徴として、スレッド上での処理が長引くとANR(Application Not Responding) という状態に陥り、以下のようなダイアログが表示されるようになります。

//image[anr][ANR]{
//}

このダイアログが表示され、ユーザーがOKボタンを押すとアプリケーションは終了してしまいます。

一般的にネットワーク通信は時間のかかる処理です。
理論値で100Mbpsを越えるネットワークでも環境によっては数百bpsしかでないため、小さいデータのやりとりであっても時間がかかるものだと考えるべきです。
ANRに陥らないためには、UIスレッド以外でネットワーク処理を行う必要があります。

Javaで新しいスレッドを作ってその中で処理を行うには、ThreadクラスとRunnableクラスを使うのが一般的ですが、AndroidにはAsyncTaskというクラスが用意されています。

//list[implemention_of_async_task][簡単なAsyncTaskの実装方法]{
public class NetworkTask extends AsyncTask<String, Integer, String> {

  @Override
  protected void onProgressUpdate(Integer... values) {
    super.onProgressUpdate(values);
  	// UIスレッドで実行されるブロック
  }

  @Override
  protected String doInBackground(String... params) {
    // 別スレッドで実行されるブロック
    return null;
  }

  @Override
  protected void onPostExecute(String s) {
    super.onPostExecute(s);
    // UIスレッドで実行されるブロック
  }

}
//}

AsyncTaskを使うには、一般的にAsyncTaskを継承したクラスを宣言します。
継承する際にGenericsに3つのクラスを指定することで
 * どのクラスをパラメータとして受け取って
 * どのクラスを進捗に渡すか
 * どのクラスを戻り値として返す


== ソケットプログラミング
SocketServerとSocketClient
スレッド制御



== HttpURLConnection
GET / POST
InputStream / OutputStream

== HttpClient (Apache Http)
GET / POST
InputStream / OutputStream

== メッセージキューとか

== ライブラリを使ったネットワーク通信

 * Volley https://android.googlesource.com/platform/frameworks/volley/
 * okhttp https://github.com/square/okhttp
 * Picasso https://github.com/square/picasso





EOF