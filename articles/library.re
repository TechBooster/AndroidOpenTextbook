= 外部ライブラリの利用と作成

本章では、Androidアプリケーション開発における外部ライブラリの役割とその使い方、作り方を学びます。

== 外部ライブラリを使ってみよう

===  ライブラリとは何か

ライブラリとは特定の処理を行うプログラムをひとまとめにし、他のプログラムから利用できる形式にしたモノの事です。ライブラリは再利用のための仕組みだと言い表してもよいでしょう。例えば画像処理や数値計算などの汎用的な処理を行うプログラムをライブラリにする事で、他のプログラムからその機能を自由に利用できるようになります。一度開発したプログラムを再度使いまわせるという事です。

//image[library-figure][アプリケーションとライブラリの関係イメージ図]{
//}

複数のライブラリを組み合わせてより高機能なプログラムを作成し、それをまたライブラリにするといった事も可能です。

Android自身も画像処理や音声処理や様々な部分でライブラリを利用しています。また、Androidアプリケーションを開発する際に利用するAndroid SDKもライブラリです。Androidが提供するライブラリによって私達はアプリケーションを開発する事ができるのです。Androidに限らず、現代の様々なプラットフォームやプロダクトは過去に開発されたライブラリの積み重ねによって成り立っていると言っても過言ではありません。

=== Androidアプリケーションおけるライブラリ

ライブラリは、利用するOSやCPUやプログラミング言語などによって様々な形式に分かれています。Androidアプリケーションで利用できるライブラリの形式はjar(Java Archive)、ライブラリプロジェクト、aar(Android Archive)、so(Shared Object)の4種類です。

==== jar(Java Archive)

Androidアプリケーションは基本的にJavaでソースコードを記述します。ですので既存のJavaライブラリの多くを利用できます。Javaライブラリの形式はjar(Java Archive)と呼ばれ、classファイル群と署名やバージョン等のメタ情報が書かれたファイルなどがアーカイブされています@<fn>{jar features}。コンパイル時にjarファイルをクラスパスに通す事で利用できます。Androidアプリケーションの場合プロジェクトのlibsディレクトリにjarファイルを配置するだけで自動的にパスが通り、利用が可能となります@<fn>{jar for eclipse}。

//footnote[jar features][Java Archive (JAR) Features : http://www.cse.yorku.ca/tech/other/jdk1.2.1/docs/guide/jar/]
//footnote[jar for eclipse][Eclipse + ADT pluginの環境を用いた場合です。Android Studioの場合はbuild.gradleにjarファイルの参照を記述します。]

==== ライブラリプロジェクト

ライブラリプロジェクトはAndroidアプリケーション専用のライブラリ形式です。通常のアプリケーションと同様にプロジェクト形式で開発を行います。ライブラリプロジェクトにはソースコードの他にレイアウトXMLや画像等のリソースファイルを含められます。これにより、画面を含んだUIやスタイル、画像リソースなどをアプリケーションから参照して利用できます。

==== aar(Android Archive)

  aarはライブラリプロジェクトをアーカイブした形式です。2013年のGoogle I/OでAndroid Studioという新しいIDEが発表されました。Android StudioはGradleというビルドツールと強く統合されており、Gradleを通してアプリケーションをビルドする形になっています。Gradleからライブラリプロジェクトを参照する時に利用するのがこのaar形式です。

==== so(Shared Object)

NDK@<fn>{ndk}を用いる事で、C/C++を使ってAndroidアプリケーションで利用するプログラムを記述できます。NDKを使ってビルドを行うとsoファイルを出力し、apkに同梱します。AndroidアプリケーションからsoファイルへはJNI(Java Native Interface)を通してアクセスする事になります。

//footnote[ndk][Android NDK（Native Development Kit）はAndroidアプリケーションで利用する一部、あるいは全体をC/C++で開発するための開発環境です]

=== 静的リンクと動的リンク

ライブラリはソースコード、オブジェクトコード、専用のフォーマットなどさまざまな形式で提供されており、利用するプログラムはそれらの形式に合わせてライブラリと結びつく必要があります。ライブラリと結びつく方式は大きく分けると静的リンクと動的リンクに分類できます。

==== 静的リンク

静的リンクはアプリケーションとライブラリをコンパイルのリンク時に結びつけ、実行ファイルに含める方式です。静的リンクの場合リンク時にライブラリのAPIやバージョン等の整合性をチェックする事になるので、コンパイルの時点で問題があれば検出できます。ただし、実行ファイルにライブラリファイルを含めるので実行ファイルのサイズは大きくなってしまいます。また、ライブラリをアップデートするには再度アプリケーションをコンパイルし直す必要があります。Androidにおいてはライブラリプロジェクトやaar、exportedなjarが静的リンクになります@<fn>{exported jar}。

//image[static-link][静的リンクのイメージ]

//footnote[exported jar][通常jarファイルは動的リンク形式のライブラリです。しかしAndroidアプリケーションはDalvikVMというJavaとは異なる処理系で動作します。この為、classファイルをDalvikVM用のdexファイルに変換する際に、参照しているjarファイルの中身もdexファイルに変換し、アプリケーションに組み込みます。コンパイルのタイミングでアプリケーション内にライブラリを組み込んでいるのでAndroidアプリケーションではjarファイルを静的リンクしているという事になります。exportedなjarとはdexファイルに変換する対象に含めるjarファイルの事を指します。例えばandroid.jarはexportedではないのでコンパイル時には処理されません。アプリケーションを実行する時に動的にリンクします。]

==== 動的リンク

動的リンクはアプリケーションを実行する時にライブラリとリンクする方式です@<fn>{dll}。実行ファイルとは独立したファイルなので、実行ファイルのサイズを圧迫する事がありません。また複数のアプリケーションから同時に利用したり、ファイルを差し替えれば再コンパイルする事なくライブラリをアップデートしたりできます。しかしライブラリが互換性のないアップデートをしてしまった場合は動作時にエラーとなってしまいます。その為動的リンク形式のライブラリを利用する場合はバージョン管理が重要になります。AndroidではAndroid SDKやsoファイルが動的リンク形式のライブラリとなります。例えば最新のAndroidのAPIを使ったアプリケーションを、古いAndroidが搭載された端末で動作させるとエラーが発生します。これは動的リンクによってAndroid SDKとアプリケーションが結びついているからです。古いAndroidに存在しないAPIを利用していたりすると、当然APIが見つからないのでエラーとなるのです。

//image[dynamic-link][動的リンクのイメージ]

//footnote[dll][Windowsを利用している方は「.dll(Dynamic Link Library)」という拡張子のファイルを見たことがあると思います。これが動的リンクができるライブラリです。]

=== Android SDKもライブラリ

前述の通りAndroid SDKもライブラリです。しかしAndroidアプリケーションを開発する時「Android SDKのライブラリを使っている」と意識した事はあまりないかもしれません。実は新規プロジェクトを作成する時にAndroid SDKのライブラリを選択しています@<img>{choose-android-framework}。

//image[choose-android-framework][新規プロジェクトのウィザード]

新規プロジェクトのウィザードで選択する「Compile With」はどのバージョンのAndroid SDKを使ってプロジェクトをコンパイルをするか設定する項目です。この設定に従ってAndroidプロジェクトに"android.jar"ファイルの参照が追加されます。"android.jar"ファイルがAndroid SDKの実体であり、その中に含まれているActivityクラスやTextViewクラスなどを使ってアプリケーションを作ることになります。

EclipseでAndroidプロジェクトの「Package Explorer」を見てみると、参照しているAndroid SDKのバージョンや中身を確認できます。@<img>{android-framework}はAndroid 4.4.2のandroid.jarを参照している例です。ライブラリの中にどのようなクラスがあるか確認できます。

//image[android-framework][Androidプロジェクトで参照しているAndroid SDK。膨大なクラスが含まれているほか、そのクラスが持つメソッドを見る事もできる]{
//}

クラスを開くとソースコードを見る事もできます。@<img>{show-activity-source}はActivityクラスのソースコードを開いた例です。Activityクラスがどのように実装されているか読む事ができます。普段よく使うsetContentViewメソッドやfindViewByIdメソッドがどの様に実装されているか是非一度読んでみてください。

//image[show-activity-source][Activityクラスのソースを覗く]{
//}

クラスを開いてもソースコードが見れない場合は、Android SDKのソースコードをインストールしているか確認してください。@<img>{download-android-framework-source}のようにAndroid SDK Managerで該当するAndroidバージョンのソースコードがインストールされているか確認できます。

//image[download-android-framework-source][Android SDKのソースをAndroid SDK Managerでダウンロードする]{
//}

=== jar形式のライブラリを使ってみよう

ライブラリの形態とAndroidアプリケーションにおけるライブラリの種類がわかった所で、早速ライブラリを使ってみましょう。本項では@<href>{https://github.com/loopj/android-async-http,android-async-http}という非同期通信のためのオープンソースライブラリ@<fn>{open-source}を使って、ネットワークプログラミングの章(14 -2 -5 HttpURLConnection の利用)で作ったHttpURLConnectionの処理を置き換えます。android-async-httpはさまざまな形式で配布していますが、ここではjarファイルを直接利用する事とします。

//footnote[open-source][オープンソースとはプログラムのソースコードを第三者に公開している事を表しますが、実際にはもう少し立て込んでいます。ソースコードの複製・改変・再頒布などの条件を定めたオープンソースライセンスを持つプログラムをオープンソースソフトウェアといいます。android-http-asyncの場合"Apache License, Version 2.0"というライセンスを持っています。オープンソースソフトウェアの利用者はオープンソースライセンスに従う必要があります。]

==== jarファイルをダウンロードし、libsディレクトリへコピーする

android-async-httpのjarファイルは@<href>{http://loopj.com/android-async-http/}でダウンロードできます。本書の執筆時点では「android-async-http-1.4.6.jar」をダウンロードできました@<img>{android-http-async-download}。

//image[android-http-async-download][android-http-asyncのjarファイルをダウンロードする]{
//}

ダウンロードしたjarファイルをAndroidプロジェクトのlibsディレクトリにコピーすれば利用できます。

//image[copy-jar][jarファイルをlibsディレクトリにコピーする]{
//}

通信処理を行うのでAndroidManifest.xmlへの"android.permission.INTERNET"パーミッションの追加も忘れずに行いましょう。

//list[add-internet-permission][AndroidManifest.xmlへ"android.permission.INTERNET"パーミッションを追加する]{
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="省略" >

  <uses-permission android:name="android.permission.INTERNET"/>

  <application>
    <!-- 省略 -->
  </application>
</manifest>
//}

==== 動作チェック

正しく導入ができたか確認してみましょう。新しく作ったプロジェクトのMainActivity.javaの冒頭に@<list>{jar-check}のimport文を記述し、エラーが発生しなければ準備完了です。

//list[jar-check][導入したライブラリのクラスをインポートする]{
import com.loopj.android.http.AsyncHttpClient;
//}

もしエラーになる場合は、jarファイルをコピーした場所が正しいかどうかや、プロジェクトを一度cleanするなどしてみてください。

==== ライブラリを用いない場合のコードをおさらい

android-async-httpを使う前にまずはライブラリを使わない場合のコードをおさらいしてみましょう。@<list>{scratch}はネットワークプログラミングの章で実装したHttpURLConnectionを用いるコードから一部のログ出力を取り除いたものです。接続の準備やデータの取得、後処理などたくさんの処理を記述しています。また、このコードはUIスレッドでは実行できないため、AsyncTask等を用いて別のスレッドで呼び出す仕組みを実装しています。

//list[scratch][HttpURLConnectionを用いる場合]{
public class MainActivity extends Activity {

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    new DownloadTask().execute();
  }

  private static class DownloadTask extends AsyncTask<Void, Void, Void> {
    @Override
    protected Void doInBackground(Void... params) {
      try {
        URL url = new URL("http://tomorrowkey.github.io");
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        connection.setRequestProperty("Host", "tomorrowkey.github.io");
        connection.connect();

        int responseCode = connection.getResponseCode();
        Log.d("TEST", "Response-Code=" + responseCode);
        String contentLength = connection.getHeaderField("Content-Length");
        Log.d("TEST", "Content-Length=" + contentLength);
        String contentType = connection.getHeaderField("Content-Type");
        Log.d("TEST", "Content-Type=" + contentType);

        InputStream inputStream = connection.getInputStream();
        String body = readToEnd(inputStream);
        Log.d("TEST", "body=" + body);
        inputStream.close();
      } catch (MalformedURLException e) {
        throw new RuntimeException(e);
      } catch (IOException e) {
        throw new RuntimeException(e);
      }

      return null;
    }

    private String readToEnd(InputStream inputStream) throws IOException {
      StringBuilder sb = new StringBuilder();
      int length;
      byte[] buffer = new byte[1024];
      while ((length = inputStream.read(buffer)) != -1) {
        sb.append(new String(buffer, 0, length));
      }
      return sb.toString();
    }
  }
}
//}

Socketを使った通信処理に比べればHttpURLConnectionはかなり楽に通信処理を記述できるようになっています。しかしまだまだ冗長です。AndroodアプリケーションでHttpURLConnectionを使って通信処理をする場合、常に以下の事を考慮しなければなりません。

 * AsyncTaskなど非同期処理のコードを追加しなければならない
 * 通信の成功や失敗を自分でチェックしなければならない
 * レスポンスをInputStreamで処理しなければならない

もちろんこれらの考慮点は一度覚えてしまえばそこまで難しいものではありません。しかし、アプリケーションで通信処理をする場合いつも実装しなければならない為、ボイラープレート・コード@<fn>{boilerplate}を沢山書く事になります。

ボイラープレート・コードが増えると、本来やりたい事に割くべき時間が削られていきます。通信処理の場合やりたい事は通信処理そのものではなく、通信の結果取得したデータを取り扱う事のはずです。ボイラープレート・コードを回避するには、それらを簡単かつ汎用的に再利用できる仕組みを用意すればよいわけですが、大抵の場合時間がかかります。そういった場合にライブラリの力を借りることになるわけです。

//footnote[boilerplate][ボイラープレートとは鋳型の事です。ボイラープレート・コードは決まりきっているけど省略できないコードの断片の事を表します]

==== android-async-httpを用いたコード

ではandroid-async-httpを利用してみましょう。android-async-httpは非同期的にネットワーク処理を行うAsyncHttpClientクラスと、同期的にネットワーク処理を行うSyncHttpClientクラスを提供しています。今回行いたいのは非同期通信なので、AsyncHttpClientクラスを利用します。

@<list>{library}はAsyncHttpClientクラスを使って実際に非同期のリクエスト処理する例です。実装の詳細はここでは深く立ち入りませんが、HttpURLConnectionを使う例に比べて簡潔にリクエスト処理を記述できている事が分かると思います。

//list[library][android-http-asyncを用いる場合]{
public class MainActivity extends Activity {

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    loadAsync();
  }

  private void loadAsync() {
    AsyncHttpClient client = new AsyncHttpClient();

    //自動的に別のスレッドで実行する
    client.get("http://tomorrowkey.github.io",
        new AsyncHttpResponseHandler() {
          @Override
          public void onSuccess(int responseCode, Header[] headers, byte[] response) {
            //通信処理が終了するとUIスレッドで呼び出される
            String body = new String(response);
            Log.d("TEST", "body=" + body);
          }

          @Override
          public void onFailure(int responseCode, Header[] headers, byte[] response, Throwable e) {
            //省略
          }
        });
  }
}
//}

AsyncHttpClientクラスは自動的に非同期でネットワーク処理を行うのでUIスレッドから呼び出しても問題ありません。また通信処理の結果を受け取るためのAsyncHttpResponseHandlerクラスのonSuccessメソッドやonFailureメソッドは自動的にUIスレッドで呼び出されるのでスレッドを意識しなくてもすみます。

==== ライブラリの使い方を知るには

大抵のライブラリには説明書があります。android-http-asyncの場合は以下のページを参照するとよいでしょう。

 * 概要と使い方 : http://loopj.com/android-async-http/
 * javadoc : http://loopj.com/android-async-http/doc/com/loopj/android/http/package-summary.html

android-http-asyncはオープンソースのライブラリですので、ソースコードそのものを参照するという手もあります。android-http-asyncのソースコードはGithubにホスティングされています。

 * Github : https://github.com/loopj/android-async-http

今回利用したAsyncHttpClientクラスのソースコードは以下のURLで読めます。

 * AsyncHttpClientクラス : https://github.com/loopj/android-async-http/blob/master/library/src/main/java/com/loopj/android/http/AsyncHttpClient.java#L831

=== 練習問題

@<list>{library}ではloadAsyncメソッドに通信先のURLが埋め込まれています。また、通信結果はログに出力だけです。これでは通信先を変更したい場合毎回loadAsyncメソッドを書き換えなければなりません。いろいろなところで「再利用」ができるように以下の条件を満たすように書き換えてみましょう。

 1. 任意に通信先のURLを変更できる
 2. 通信結果をStringで受け取って自由に処理できる

==== 任意に通信先のURLを変更できる

@<list>{library}のloadAsyncメソッドは、通信先URLがメソッド内に書かれている為loadAsyncメソッドの外からURLの変更ができません。通信先のURLを引数で受け取る形にすれば任意のURLを指定できるようになります(@<list>{load-async-01})。

//list[load-async-01][loadAsyncメソッドで任意のURLを引数に取る]{
private void loadAsync(String url) {
  AsyncHttpClient client = new AsyncHttpClient();
  client.get(url,
      new AsyncHttpResponseHandler() {
          //省略
      });
}
//}

引数を追加する事で任意のURLを受け取る事ができるようになりました。しかし実は@<list>{load-async-01}の実装ではまだ不十分です。引数urlがnullの場合NullPointerExceptionが発生してしまいます。引数の妥当性をチェックする必要があります。

AndroidにはTextUtilsクラスという文字列の操作やチェックができる便利なクラスがあります。TextUtilsクラスのisEmptyメソッドを使えば引数が空(長さが0)あるいはnullかどうかをチェックできます@<list>{load-async-02}。

//list[load-async-02][引数の妥当性をチェックする]{
private void loadAsync(String url) {
  if (TextUtils.isEmpty(url)) {
    return;
  }
  AsyncHttpClient client = new AsyncHttpClient();
  client.get(url,
      new AsyncHttpResponseHandler() {
          //省略
      });
}
//}

空、nullだけでなくURLとして正しい形式かチェックしたい場合もあるかもしれません(AsyncHttpClientの場合、誤ったURL形式を渡すとAsyncHttpResponseHandlerクラスのonFailureメソッドが呼び出されるので、事前チェックが必ず必要というわけではありません)。

StringクラスのstartsWithメソッドを使えば、指定した文字列で始まっているかどうかをチェックできます@<list>{load-async-03}。

//list[load-async-03][startsWithメソッドを使う]{
// "http://"で始まっていなかったらreturn
if(!url.startsWith("http://")){
  return;
}
//}

もっと手っ取り早く確認するためにURLクラスを用いるという手もあります。URLクラスはコンストラクタに渡されたURLが正しいかをチェックし、問題があればMalformedURLExceptionをthrowします。チェックはプロトコルとポート番号に対して行われます@<fn>{url-validate}。

//list[load-async-04][URLクラスを使う]{
try{
  URL url = new URL(urlString);
}catch(MalformedURLException e){
  // URLクラスがMalformedURLExceptionをthrowしたらreturn
  return;
}
//}

//footnote[url-validate][デフォルトではhttp,https,ftp,file,jarのプロトコルに該当するかどうかと、ポート番号が-1以上である事をチェックします。参考 : http://docs.oracle.com/javase/jp/6/api/java/net/URL.html]

==== 通信結果をStringで受け取って自由に処理できる

通信結果をメソッドの呼び出し元に渡すにはどのようにすればよいでしょうか。よくある誤りを@<list>{load-async-05}に示します。何が間違っているかわかるでしょうか。

//list[load-async-05][間違った例]{
private String loadAsync(String url) {
  AsyncHttpClient client = new AsyncHttpClient();
  client.get(url, new AsyncHttpResponseHandler() {
    @Override
    public void onSuccess(int responseCode, Header[] headers,
        byte[] response) {
      String body = new String(response);
      return body;
    }
    //省略
  });
}
//}

loadAsyncメソッドの戻り値をStringにして、通信結果を"return body;"で返しています。一見よさそうに思えるのですが実際は誤りです。まず、そもそもこのコードはコンパイルが通りません。

AsyncHttpClientクラスのgetメソッドは非同期で通信処理を行うので、getメソッドそのものはすぐに終了します。getメソッドが終了すると、次に処理するものはないのでloadAsyncメソッドが終了します。AsyncHttpResponseHandlerクラスのonSuccessメソッドは通信が終わった時呼び出されるので、それぞれの処理は@<list>{load-async-06}に示した順番で実行されるという事になります。

//list[load-async-06][処理が実行されるタイミング]{
private String loadAsync(String url) {
  AsyncHttpClient client = new AsyncHttpClient();

  //1 getメソッドを実行する。非同期処理を開始するだけなのですぐ終わる。
  client.get(url, new AsyncHttpResponseHandler() {
    @Override
    public void onSuccess(int responseCode, Header[] headers,
        byte[] response) {
      //3 通信処理が終わったら呼び出される
      String body = new String(response);
      return body;
    }
    //省略
  });

  //2 loadAsyncメソッドが終了する
}
//}

loadAsyncメソッドはStringを戻り値としているので"2"の時点でなんらかの値を返さなければなりません。通信処理はまだ終わっていないため通信結果をこのタイミングで返す事はできません。コンパイルを通すためには"return null"といった記述をする事になります。これではStringを戻り値にしている意味がありませんね。

次に"3"のタイミングで"return body"と書いていますが、処理を書いている場所はAsyncHttpResponseHandlerクラスのonSuccessメソッドの中です。onSuccessメソッドは戻り値がvoidなのでStringを返す事はできません。また、返せたとしてもloadAsyncメソッドの戻り値とはならないのでこれもまた意味がありません。

ではどの様にすれば良いか。実はすでに答えがコード中に書かれています。コールバックを利用すればよいのです。AsyncHttpClientクラスのgetメソッドは、AsyncHttpResponseHandlerクラスを渡してコールバックを受け取るスタイルになっています。同様の仕組みをloadAsyncメソッドに実装すればよいわけです。

@<list>{load-async-07}はコールバックを利用する実装の例です。

//list[load-async-07][コールバックを利用する]{

//コールバックを受け取る為のinterfaceを定義する
public interface OnLoadListener {
  public void onLoad(String result);
}

//OnLoadListenerを引数に受け取る
private void loadAsync(String url, final OnLoadListener listener) {
  AsyncHttpClient client = new AsyncHttpClient();
  client.get(url, new AsyncHttpResponseHandler() {
    @Override
    public void onSuccess(int responseCode, Header[] headers,
        byte[] response) {
      String body = new String(response);

      //コールバックする
      listener.onLoad(body);
    }

    //省略

  });
}
//}

@<list>{load-async-08}はURLとコールバック用のinterfaceを引数に取るようになったloadAsyncメソッドを利用する例です。これで色々な所から利用できるようになりました。また、android-http-asyncを利用する部分をloadAsyncメソッドの中に"隠蔽"したことによって、利用する時のコードがシンプルになりました@<fn>{wrapper}。

//list[load-async-08][コールバックを渡して結果を受け取る]{
@Override
protected void onCreate(Bundle savedInstanceState) {
  super.onCreate(savedInstanceState);
  setContentView(R.layout.activity_android_http_async);

  loadAsync("http://tomorrowkey.github.io", new OnLoadListener() {
    @Override
    public void onLoad(String result) {
      //処理
    }
  });
}
//}

//footnote[wrapper][コードがシンプルになりましたが同時に新たな問題を生んでいる点についても意識する必要があります。loadAsyncメソッドは通信エラーの時の処理ができません。またヘッダやステータスコードを見る事もできません。シンプルになった代わりに制約も生んでいるのです。この制約をどのように取り扱うかはプログラムの要件によって変わります。今回は細かなハンドリングは意識しないのでこの形にしています。]

http://rss.dailynews.yahoo.co.jp/fc/computer/rss.xml

=== 演習問題

(1) RSSフィードを読み込んで結果をTextViewに表示してみよう

以下のページから好きなRSSフィードを選んでください。

 * http://headlines.yahoo.co.jp/rss/list

(2) RSSフィードからitem要素を取り出してみよう

jsoup(http://jsoup.org/)というjar形式のライブラリを導入し、RSSフィードのitem要素を取り出してみましょう。その後以下の処理を実装してみましょう。

 * RSSフィードのitem要素をListViewに表示してみよう
 * ListViewに表示したitem要素をタップして、ブラウザでURLを開くようにしよう(URLをブラウザで開く方法については第6章 6-3-2 暗黙的Intentの項が参考になるでしょう)

=== まとめ

本節ではライブラリとは何か、Androidで利用できるライブラリの種類、jar形式のライブラリの使い方を学びました。ライブラリを用いることで効率的に開発が行える事を体験できたのではないかと思います。本節では通信処理に関するライブラリを利用しましたが、他にも様々な種類のライブラリがあります。どの様なライブラリがあり、どういった性質を持っているかを知ることはアプリケーションの開発効率に大きく影響を与えます。不要な開発を減らせるだけでなく、先人が通った道を見本にしてさらなる改善ができるからです。ライブラリをただ利用するだけでなく、ソースコードを読んだり、ドキュメントを読んだりして"何故そのような実装になっているか"、"何故そのような設計になっているか"を日頃から考えるようにしてみましょう。

== ライブラリプロジェクトを使ってみよう

=== ライブラリプロジェクトとは何か

ライブラリプロジェクトはAndroid専用のライブラリ形式です。ソースコードとリソースファイルを内部に持つ事ができ、レイアウトファイルやdrawableを利用するActivityやViewなどをライブラリとして提供できます。

//image[jar-libraryproject][JAR形式とライブラリプロジェクト形式の違い]

=== Androidアプリケーションのビルドフロー

なぜライブラリプロジェクトが誕生したのでしょうか。JAR形式のライブラリにリソースファイルを含める事はできないのでしょうか。これらの点を理解するにはAndroidアプリケーションがどの様なプロセスでビルドされているかを知る必要があります。

@<img>{build-flow}はAndroid DevelopersのBuilding and Running@<fn>{Building and Running}に記載されているAndroidアプリケーションをビルドするフローです。Androidプロジェクトからapkに変換されるまでの間に複数のツールで順番に処理されている事がわかります@<fn>{sdk-tools}。それぞれのツールで何が行われているか見て行きましょう。

//image[build-flow][Androidアプリケーションをビルドするフロー]{
//}
@<fn>{creative-commons-2.5}

//footnote[Building and Running][https://developer.android.com/tools/building/index.html]
//footnote[creative-commons-2.5][Creative Commons Attribution 2.5 http://creativecommons.org/licenses/by/2.5/legalcode]
//footnote[sdk-tools][これらのツールは$ANDROID_SDK/build-tools/(build tool version)/の中にあります。apkbuilder]

==== aidl

AIDLはAndroid Interface Definition Languageの略で、プロセス間通信の為のインタフェースを定義するDSLです。異なるプロセスで動作するServiceに対してActivityなどから接続する際に利用します@<fn>{aidl-example}。AIDLはJavaとは異なる独自の言語ですのでそのままではJavaコンパイラでコンパイルできません。アプリケーションのソースコードをコンパイルする前に、AIDLをコンパイルしてJavaのソースコードを生成します。

//footnote[dsl][DSLはDomain-Specific Languageの略でドメイン固有言語と呼びます。DSLは特定のタスク向けに設計された独自の言語の事を差します。AIDLはプロセス間通信の為に設計された独自の言語という事になります]
//footnote[aidl-example][典型的な例としてはアプリ内課金などを実装する際にAIDLを利用します。アプリ内課金はGoogle Playを通して行います。Google Playに対して決済の処理などを依頼する為にAIDLを使って接続します]

==== aapt

aapt(Android Asset Packaging Tool)はアプリケーションのリソースファイルやAndroidManifest.xmlを解析しR.javaを生成します。ここで生成するR.javaを使って画像やレイアウトファイルへプログラムからアクセスする事になります。その他valuesディレクトリをコンパイルしてアーカイブします。

==== Java Compiler

通常のJava言語のコンパイラ(javac)です。aidlやaaptが生成したソースコードと、アプリケーションのソースコードをまとめてコンパイルしてclassファイルを出力します。

==== dex

classファイルをコンパイルし、Dalvik VMのバイトコードであるdexファイルへ変換します。この時、AndroidプロジェクトがJAR形式のライブラリを利用している場合は、それらのライブラリ内のclassファイルも同時にdexファイルへコンパイルします。

==== apkbuilder(sdklib.jar)

dexファイル、コンパイルしたリソース、画像類などのリソースファイルをアーカイブしてapkファイルを作成します。

==== jarsigner

jarsignerはJavaの標準のツールです。jarsignerを使ってapkファイルにデジタル署名を行います。デバッグビルドの時はデバッグ用の共有の署名が使われますが、アプリケーションをリリースする場合は自分で証明書を作成する必要があります。

==== zipalign

zipalignはapkを最適化するツールです。apkはアプリケーションのインストール時や、ホームアプリケーションに名前やアイコンを表示する時など様々な場面で読み取られます。apkbuilderでアーカイブしたapkの内部をzipalignを使って整列する事で、apkの読み取りを高速化できるようになります。

=== JAR形式のライブラリがリソースファイルを持てない理由

Androidアプリケーションのビルドフローを見ると、JAR形式のライブラリが処理されるのはdexファイルに変換されるタイミングだという事がわかります。リソースファイルを取り扱うにはR.javaに定義されたリソースIDが必要です。R.javaはaaptコマンドによって生成されるので、JAR形式のライブラリからはR.javaの中のリソースIDを事前に知ることができません。その為JAR形式のライブラリではリソースファイルを持てない、持ったとしても内部から利用できないという事になるわけです。

==== ライブラリプロジェクトの誕生

そこで誕生したのがライブラリプロジェクトです。ライブラリプロジェクトはAndroidプロジェクトと同じ構造をなのでリソースファイルを持てます。ビルドの際はAndroidプロジェクトと一緒にビルドフローを辿るので、R.javaが問題になる事はありません。この仕組みによって画面を持つActivityやViewなどをライブラリ化できるようになりました。

=== ライブラリプロジェクトを導入してみよう

ライブラリプロジェクトを使ってアプリケーションを開発してみましょう。本節ではAndroid-BootstrapというスタイリッシュなUIの構築を助けるライブラリを導入します。

==== Android-Bootstrapとは

Android-Bootstrapは、Web用のフロントエンドフレームワーク「Bootstrap」のコンセプトを元に構成したAndroid用のUIライブラリです。角丸ボタンやアイコン付きボタン、FontAwesomeの利用、円形サムネイルなど様々なUI部品を提供しています。

//image[android-bootstrap][Android-Bootstrapのコンポーネントたち]{
//}

==== ライブラリプロジェクトをダウンロードする

Android-Bootstrapは@<href>{https://github.com/Bearded-Hen/Android-Bootstrap/}でダウンロードできます。画面右下の「Download ZIP」からプロジェクト全体をダウンロードしてください。

//image[download-library-project][Android-Bootstrapのzipファイルをダウンロードする]

==== ライブラリプロジェクトをインポートする

ライブラリプロジェクトは通常のAndroidプロジェクトと同じ構造を持つので、そのままEclipseにインポートできます。zipファイルを解凍し、ライブラリプロジェクトをインポートしましょう。

//image[setting-android-bootstrap01][「Android Project from Existing Code」でライブラリプロジェクトをインポートする]

解凍したディレクトリをインポート対象に選択すると、AndroidBootstrapとAndroidBootstrapTestの2つのプロジェクトが表示されます(@<img>{setting-android-bootstrap02})。両方インポートしても問題ありませんが、AndroidBootstrapTestは今回利用しないのでチェックを外しておきましょう。

//image[setting-android-bootstrap02][AndroidBootstrapを選択してインポートする]

インポートが成功すればPackage ExprolrerにAndroidBootstrapが追加されます(@<img>{setting-android-bootstrap03})。

//image[setting-android-bootstrap03][]

==== ライブラリプロジェクトを参照する

次に、新しいAndroidプロジェクトを作成してください。本書では"UseAndroidBootstrap"という名前のプロジェクトを作成したものとして進めます。新しく作成したAndroidプロジェクトのディレクトリの上で右クリックし、"Properties"を選択してください@<img>{setting-android-bootstrap04}。

//image[setting-android-bootstrap04][AndroidプロジェクトのPropertiesを開く]

"Android"の項目を開き、LibraryセクションのAddボタンを押下してください(@<img>{setting-android-bootstrap05})。

//image[setting-android-bootstrap05][LibraryセクションのAddボタンを押下する]

すると参照できるライブラリプロジェクトの一覧が表示されるので、AndroidBootstrapを選択し、OKを押下してください(@<img>{setting-android-bootstrap06})。

//image[setting-android-bootstrap06][参照できるライブラリプロジェクトの一覧]

LibraryセクションにAndroidBootstrapが追加されていれば成功です(@<img>{setting-android-bootstrap07})。

//image[setting-android-bootstrap07][AndroidBootstrapを参照すると、Libraryセクションに参照しているライブラリが追加される]

==== 動作確認

正しく準備ができたか確認してみましょう。新しく作ったプロジェクトのMainActivity.javaの冒頭に@<list>{library-project-check}のimport文を記述し、エラーが発生しなければ準備完了です。

//list[library-project-check][導入したライブラリのクラスをインポートする]{
import com.beardedhen.androidbootstrap.BootstrapButton;
//}

もしエラーになる場合はプロジェクトを一度cleanしたり再ビルドするなどしてみてください。

=== Android-BootstrapのBootstrapButtonを使う

Android-BootstrapはいくつかのカスタムViewを提供しています。そのうちの一つであるBootstrapButtonを使ってみましょう。@<list>{BootstrapButton}の様にレイアウトファイルにBootstrapButtonを定義してください。

//list[BootstrapButton][res/layout/activity_main.xml]{
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
  xmlns:tools="http://schemas.android.com/tools"
  xmlns:library="http://schemas.android.com/apk/res-auto"
  android:layout_width="match_parent"
  android:layout_height="match_parent">

  <com.beardedhen.androidbootstrap.BootstrapButton
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:layout_margin="10dp"
    android:text="Hello Bootstrap!"
    library:bb_type="success" />

</LinearLayout>
//}

アプリケーションを起動すると@<img>{use-android-bootstrap}の様な表示になるはずです。

//image[use-android-bootstrap][BootstrapButtonの表示]

=== カスタム属性

BootstrapButtonのレイアウトの定義に見慣れない記述がありました。レイアウトの要素には通常"android:"をプレフィクスとした属性値を定義しますが、BootstrapButtonでは@<list>{BootstrapButton-attribute}の様に異なるプレフィクスを持った属性を定義しています。

//list[BootstrapButton-attribute][BootstrapButtonに指定したプレフィクスが異なる属性]{
library:bb_type="success"
//}

これは「カスタム属性」というAndroidの機能です。Androidではレイアウトファイルで利用する属性値を自分で定義できます。Android-Bootstrapは「カスタム属性」を各種コンポーネントに定義し、レイアウトファイル上で様々な設定ができるようになっています@<fn>{custom-attribute-namespace}。

BootstrapButtonの「bb_type」属性の値を変更してみましょう。

//list[BootstrapButton-attribute-bb-type][bb_typeを変更する]{
library:bb_type="danger"
//}

アプリケーションを実行すると@<img>{use-android-bootstrap2}の様になります。このようにAndroid-Bootstrapはカスタム属性によって様々な表示を簡単に設定できる仕組みを提供しています。

//image[use-android-bootstrap2][カスタム属性bb_typeを"danger"に変更する]

//footnote[custom-attribute-namespace]["library:"というプレフィクスを使っていますが、特に決まりはありません。このプレフィクスは「名前空間」と呼び、属性の名前が衝突しない為に用いられます。カスタム属性の為の名前空間は、レイアウトファイルの冒頭のxmlns属性定義で自由に決められます。本書ではxmlns:library="http://schemas.android.com/apk/res-auto"という定義し"library:"というプレフィクスを用いることを宣言しています]

==== ライブラリが定義しているカスタム属性を知るには

UIコンポーネントを提供するライブラリではカスタム属性を定義している場合が多いです。ライブラリが定義しているカスタム属性を知る方法として以下が挙げられます。

 * ドキュメントを読む
 * ソースを読む

基本的にまずはドキュメントを探して読む事になるでしょう。例えばAndroid-BootstrapのBootstrapButtonの使い方は以下のページに解説があります。

 * Bootstrap Button : https://github.com/Bearded-Hen/Android-Bootstrap/wiki/Bootstrap-Button

Android-Bootstrapは比較的丁寧なドキュメントが揃っていますが、中にはドキュメントがなかったりカスタム属性についての記述が不足している場合があります。参考になるドキュメントがない場合は、ライブラリのソースを読んで調べる事になります。

==== ソースを読んでカスタム属性を理解する

EclipseにインポートしたAndroid-Bootstrapの中を覗いてBootstrapButtonクラスのカスタム属性について調べてみましょう。Android-Bootstrapの"res/values/attrs.xml"を開いてください@<img>{AndroidBootstrap-attr}。

//image[AndroidBootstrap-attr][Android-Bootstrapのattrs.xml]

@<list>{AndroidBootstrap-attr.xml}にattrs.xmlファイルの冒頭部分を抜粋します。declare-styleable要素のname属性にBootstrapButtonという記述があります。これはBootstrapButtonのカスタム属性の宣言を表します。declare-styleable要素の中に複数のattr要素を持っています。attr要素には属性名と指定できる値の種類を定義します。

//list[AndroidBootstrap-attr.xml][res/values/attrs.xml]{
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <declare-styleable name="BootstrapButton">
        <attr name="bb_type" format="string"/>
        <attr name="bb_icon_left" format="string"/>
        <attr name="bb_icon_right" format="string"/>
        <attr name="bb_roundedCorners" format="boolean"/>
        <attr name="bb_text_alignment" format="string"/>
        <attr name="bb_size" format="string"/>
        <attr name="bb_text_gravity" format="string"/>
        <attr name="android:textSize"/>
        <attr name="android:text"/>
        <attr name="android:enabled"/>
        <attr name="android:layout_width"/>
    </declare-styleable>

    <!-- 省略 -->
//}

これでBootstrapButtonの持つカスタム属性はわかりました。次はこれらの属性がどの様に使われているかを調べます。すべて調べるのは大変なので、代表として"bb_type"属性がどの様に使われているかをBootstrapButtonクラスのソースコードを読んで確認してみましょう。BootstrapButtonクラスは"src/com/beardedhen/androidbootstrap/BootstrapButton.java"で定義されています。

//image[BootstrapButton][BootstrapButton.java]

BootstrapButton.javaを開き、"bb_type"でソースコード内を検索をすると140行目@<fn>{BootstrapButton-hash}に"bb_type"属性を扱う処理が見つかります@<list>{BootstrapButton.javaL140}。

//list[BootstrapButton.javaL140][BootstrapButton.javaの140行目]{
113:private void initialise( AttributeSet attrs ) {
-省略-
121:  TypedArray a = getContext().obtainStyledAttributes(attrs,
122:    R.styleable.BootstrapButton);
-省略-
126:    String bootstrapType = "default";
-省略-
140:  if (a.getString(R.styleable.BootstrapButton_bb_type) != null) {
141:    bootstrapType = a.getString(R.styleable.BootstrapButton_bb_type);
142:  }
//}

カスタム属性を定義すると、RクラスのstyleableにIDが追加されます。ID名はdeclare-styleable要素で定義したname属性とattr要素のname属性を"_(アンダースコア)"でつないだものになります。このIDを使ってTypedArrayクラスから指定された値を取り出す事ができます。BootstrapButtonのbb_type属性の場合は"BootstrapButton_bb_type"という名前となります。

TypedArrayクラスはContextクラスのobtainStyledAttributesメソッドで取り出します。この時引数にAttributeSetクラスと"R.styleable.BootstrapButton"を渡します。AttributeSetクラスはViewのコンストラクタで受け取ったものを使います@<fn>{AttributeSet}。"bb_type"の属性値はStringなので、TypedArrayクラスのgetStringメソッドで取り出します。取り出した値はbootstrapTypeにセットしています。

次はbootstrapTypeがどこで使われているかを探します。"bootstrapType"でソースコード内で検索すると240行目に@<list>{BootstrapButton.javaL240}のような実装が見つかります。

//list[BootstrapButton.javaL240][BootstrapButton.javaの240行目]{
26:   private static Map<String, BootstrapTypes> bbuttonTypeMap;
-省略-
36:   private boolean roundedCorners = false;
-省略-
125:  BootstrapTypes type = null;
-省略-
240:  //get the correct background type
241:  if(roundedCorners == true)
242:  {
243:    type = bbuttonTypeMapRounded.get(bootstrapType);
244:  } else {
245:    type = bbuttonTypeMap.get(bootstrapType);
246:  }
247:
248:  //set up as default
249:  if (type == null)
250:  {
251:    type = BootstrapTypes.DEFAULT;
252:  }
253:
254:  //apply the background type
255:  layout.setBackgroundResource(type.backgroundDrawable);
256:  lblLeft.setTextColor(getResources().getColor(type.textColour));
257:  lblMiddle.setTextColor(getResources().getColor(type.textColour));
258:  lblRight.setTextColor(getResources().getColor(type.textColour));
//}

241行目のif文は"roundedCorners"という変数名から「ボタンを角丸にするかどうか」を判定していることがわかります。今回は特に角丸の指定はしていなかったので245行目の処理に進みましょう。245行目ではbbuttonTypeMapという変数のgetメソッドにbootstrapTypeを渡しています。

bbuttonTypeMapはMap<String, BootstrapTypes>クラスです。getメソッドでBootstrapTypesクラスを取り出せます。BootstrapTypesクラスを取り出して"type"という変数にセットし、その後255行目で利用しています。

次はbbuttonTypeMapをどの様に初期化しているか、BootstrapTypesクラスがどのようなクラスなのかを見ていきます。まずはBootstrapTypesクラスです。BootstrapTypesクラスは84行目に定義されています。BootstrapTypesクラスは"class"ではなく"enum"と宣言されています。"enum"は列挙型を表します。

//list[BootstrapButton.javaL84][BootstrapButton.javaの84行目]{
84:  private enum BootstrapTypes
85:  {
86:    DEFAULT(R.drawable.bbuton_default, R.color.black),
87:    PRIMARY(R.drawable.bbuton_primary, R.color.white),
88:    SUCCESS(R.drawable.bbuton_success, R.color.white),
89:    INFO(R.drawable.bbuton_info, R.color.white),
90:    WARNING(R.drawable.bbuton_warning, R.color.white),
91:    DANGER(R.drawable.bbuton_danger, R.color.white),
92:    INVERSE(R.drawable.bbuton_inverse, R.color.white),
-省略-
102:   private int backgroundDrawable;
103:   private int textColour;
104:
105:   BootstrapTypes(int backgroundDrawable, int textColour)
106:   {
107:     this.backgroundDrawable = backgroundDrawable;
108:     this.textColour = textColour;
109:   }
110: }
//}

列挙型は通常のクラスと同じようにコンストラクタやフィールドやメソッドを持ちます。ただしインスタンスはenum内に宣言した列挙子のみに限られるという特徴があります。インスタンスが宣言した数しか存在しない事が保証されている為、switch文で比較したり定数として扱う事ができます。

BootstrapTypesクラスはフィールドにbackgroundDrawable(背景のdrawable)とtextColour(文字色)を持った単純なクラスです。列挙子として"DEFAULT"や"PRIMARY"などが宣言されています。列挙子を宣言する際に背景のdrawableと文字色のIDを渡しています。これによりボタンの色のバリエーションを定義しています。

次はbbuttonTypeMapの初期化です。41行目のstaticイニシャライザ@<fn>{static-initializer}でキーに対応するBootstrapTypesクラスをセットしている事がわかります@<list>{BootstrapButton.javaL41}。

//list[BootstrapButton.javaL41][BootstrapButton.javaの41行目]{
41:  static{
42:
43:  bbuttonTypeMap = new HashMap<String, BootstrapTypes>();
44:
45:  bbuttonTypeMap.put("default", BootstrapTypes.DEFAULT);
46:  bbuttonTypeMap.put("primary", BootstrapTypes.PRIMARY);
47:  bbuttonTypeMap.put("success", BootstrapTypes.SUCCESS);
48:  bbuttonTypeMap.put("info", BootstrapTypes.INFO);
49:  bbuttonTypeMap.put("warning", BootstrapTypes.WARNING);
50:  bbuttonTypeMap.put("danger", BootstrapTypes.DANGER);
51:  bbuttonTypeMap.put("inverse", BootstrapTypes.INVERSE);
-省略-
//}

これですべての情報がそろいました。bb_type属性で指定した値はbbuttonTypeMapに登録されたBootstrapTypesクラスを取り出すためのキーとして利用されていました。BootstrapTypesは背景のdrawableと文字色を持つクラスで、このクラスが持つ値を使ってViewの表示色などを変更している事がわかりました。

少し大変だったかと思います。しかしソースを読む事によってbb_type属性の役割がわかっただけでなく、「表示を属性値によって簡単に切り替える仕組み」がどの様に実現されているかもわかりました。また「bb_type属性の値の種類が7つある事」や「角丸表示の設定がある事」や「enumクラスの使い方の参考」なども得られました。ソースコードを読むことで非常に多くの学びを得られます。


//footnote[BootstrapButton-hash][将来変更される可能性があるので、ソースコードのハッシュ値を記載しておきます。9067416920a2a9a000a77280010fdc580aaf06b1]
//footnote[AttributeSet][Viewのコンストラクタが3種類あった事を覚えているでしょうか。11章の第1節でカスタムViewを作成する際に"XMLより呼び出す際のコンストラクタ"として解説したコンストラクタの引数にAttributeSetクラスが渡ってきます。AttributeSetクラスにはレイアウトファイルで定義した属性値が入っています]
//footnote[static-initializer][staticイニシャライザは、クラスが初めてクラスローダに読み込まれる時に一度だけ実行されるブロックの事です。クラスがインスタンス化される時では無い点に注意してください。]

=== ライブラリとライセンス

Androidアプリケーションで利用するライブラリのほとんどはオープンソースです。前項でAndroid-Bootstrapのカスタム属性を調べる事ができたのもAndroid-Bootstrapがオープンソースだからです。オープンソースのソフトウェアには「オープンソースライセンス」が定義されています。オープンソースのソフトウェアを利用する場合「オープンソースライセンス」に記載されている条件に従う必要があります。

オープンソースライセンスには沢山の種類があります。本項では近年のオープンソースのライブラリでよく利用されている「Apache License 2.0」、「MIT License」に加え、コピーレフトという思想に則った「GPLライセンス」について簡単に解説します。

==== GNU General Public License(GPLライセンス)

GPLライセンスは「オープンソース」という概念が提唱される時代より前に作られたライセンスのひとつです。ライセンス条項はコピーレフトの思想に則っています。コピーレフトは著作権(コピーライト)を保持したまま、すべての者がその著作物の利用、再頒布、改変ができなければならないという考え方です。

このため、GPLライセンスを持ったオープンソースのライブラリを利用する場合ライブラリを利用したアプリケーションのソースコードにもGPLライセンスが適用されます。つまりアプリケーションソースコードを公開しなければならないという事です。

==== Apache license 2.0

Apache license 2.0はApacheソフトウェア財団 (ASF) が策定したオープンソースライセンスです。Apache license 2.0ではソースコードの開示義務はありません。

Apache license 2.0ライセンスのライブラリにNOTICEファイルが含まれている場合、その内容についてアプリケーション上で表示する必要があります。多くのアプリケーションではNOTICEファイルの有無に関係なくライブラリ名、著作権者、ライセンス条項を表示する場合が多いです。

==== MITライセンス

MITライセンスはマサチューセッツ工科大学公開したオープンソースライセンスです。非常にシンプルなライセンスで原文も20行程度しかありません。

利用にあたって著作権表示と許諾表示を行う必要があります。MITライセンスもソースコードの開示義務はありません。

==== ライセンスを確認しよう

代表的なオープンソースライセンスについてざっくり説明をしました。オープンソースライセンスには様々な種類があり、それぞれで条件が異なります。オープンソースのライブラリを利用する時は必ずライセンスの確認をし、守るべき条件を確認しましょう。@<img>{license-example}はApache license 2.0やMITライセンスの著作権表示や許諾表示の例です。既存のアプリケーションがどの様にオープンソースライセンスを取り扱っているか参考にするとよいでしょう。

//image[license-example][著作権表示、許諾表示の例]

=== 演習問題

(1) Android-Bootstrapを使ってプロフィール画面を作ってみましょう。

Android-Bootstrapのコンポーネントを使ってプロフィール画面を作ってみましょう。以下の例はBootstrapButton、FontAwesomeText、BootstrapCircleThumbnailを使っています。例を再現してもよいですし、独自にデザインしても構いません。

//image[profile-app][プロフィール画面の例]

(2) Android-Bootstrapのライセンスを調べてみましょう。

Android-Bootstrapのライセンスの種類はなにか、利用にあたって何をしなければならないか調べてみましょう。

=== まとめ

本節ではライブラリプロジェクトの生い立ち、ライブラリプロジェクトの使い方、オープンソースライセンスについて学びました。ライブラリプロジェクトはソースコードをインポートして利用する形式ですので、ライブラリ自身のソースコードを参照しやすいです。Androidアプリケーションを作る時、Android自身のソースコードを参照する事がよくあります。ソースコードを読めばドキュメントに書かれていない詳細な処理やAndroidバージョン毎の違いを知る事ができます。普段からソースコードを読む癖をつけておきましょう。

===[column] Androidはオープンソース

Androidのソースコードのダウンロード方法は以下のページで確認できます。

 * Downloading the Source : https://source.android.com/source/downloading.html

Androidのソースコードは巨大で、すべてのソースコードをダウンロードすると数十GBの容量になります。気軽にダウンロードできるサイズではないですね。Androidのソースコードを簡単に参照できるサービスがいくつかあります。まずはこれらのサービスを利用してソースコードを読むとよいでしょう。

 * Developer Collaboration Project : https://sites.google.com/site/devcollaboration/codesearch
 * GrepCode : http://grepcode.com/project/repository.grepcode.com/java/ext/com.google.android/android/

== ライブラリプロジェクトを作ってみよう

=== ライブラリプロジェクトを作る

ライブラリプロジェクトの開発はほとんどAndroidプロジェクトと同じです。新規のライブラリプロジェクトを作成し、簡単な計算を行うCalculatorクラスを追加してみましょう。

=== 新規ライブラリプロジェクトを作成する

新規プロジェクトのウィザードを立ち上げ、"Android Application Project"を選択してください(@<img>{new_library_project_01})。

//image[new_library_project_01]["Android Application Project"を選択する]

プロジェクト名は"MyLibrary"とします。パッケージ名は適宜書き換えても構いません。"Minimum Required SDK"を"API 15: Android 4.0.3"に設定する事を忘れないようにしてください(@<img>{new_library_project_02})。"Minimum Required SDK"とライブラリプロジェクトの関係については後ほど解説します。

//image[new_library_project_02][プロジェクト名などを入力する]

プロジェクト設定のウィザードで"Mark this project as a library"にチェックを入れてください(@<img>{new_library_project_03})。このチェックの有無によって作成するプロジェクトの種類が変わります。"Create custom launcher icon"や"Create activity"のチェックは外してください。

//image[new_library_project_03]["Mark this project as a library"にチェックを入れる]

新規プロジェクトの作成が完了したら、プロジェクトのプロパティを開いて確認しましょう(@<img>{new_library_project_04})。Libraryのセクションの"is Library"にチェックが入っていれば成功です。

//image[new_library_project_04]["is Library"にチェックが入っているか確認する]

==== Calculatorクラスを追加する

Calculatorクラスは足し算を行う"plus"メソッドを持つ単純なクラスです。このクラスをライブラリプロジェクトに追加しましょう。"Calculator"クラスを新規作成し、@<list>{Calculator}を参考に実装をしてください。今回作成したライブラリプロジェクトはパッケージが無い状態ですので、パッケージの作成を忘れないようにしましょう。

//list[Calculator][com.example.mylibrary.Calculator.java]{
package com.example.mylibrary;

public class Calculator {
  public int plus(int a, int b) {
    return a + b;
  }
}
//}

=== 作成したライブラリプロジェクトを使う

次にCalculatorを利用するAndroidアプリケーションを作成しましょう。新しいAndroidプロジェクトを作成し、第2節でAndroid-Bootstrapを利用した時の様に"MyLibrary"を参照してください。本節では"UseMyLibrary"という名前でプロジェクトを作成したものとします。

設定が終わったら、UseMyLibraryのプロパティを開き、@<img>{use_new_library_project}の様にMyLibraryが参照できている事を確認してください。

//image[use_new_library_project][MyLibraryが参照できている事を確認する]{
//}

==== Calculatorクラスを使う

Calculatorクラスのplusメソッドを使って得た結果をTextViewに表示してみましょう。UseMyLibraryのMainActivityで使うレイアウトを次のように作成してください。

//list[use_libary_layout][res/layout/activity_main]{
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent">
    <TextView
        android:id="@+id/text"
        android:textSize="40sp"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/hello_world" />
</LinearLayout>
}

次にMainActivityの実装を行います。

//list[use_library_code][MainActivity.java]{
import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import com.techinstitute.mylibrary.Calculator;

public class MainActivity extends Activity {
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    Calculator calculator = new Calculator();
    int result = calculator.plus(10, 15);

    TextView textView = (TextView) findViewById(R.id.text);
    textView.setText("result=" + result);
  }
}
//}

アプリケーションを実行し、@<img>{use_library}の様な結果が得られるか確認してください。

//image[use_library][Calculatorクラスのplusメソッドの結果を表示する]{
//}

=== ライブラリのminSdkVersionを確認する

今回作成したライブラリプロジェクトは"Minimum Required SDK"(以後、minSdkVersionとします)を"API 15: Android 4.0.3"に設定しました。この設定はライブラリを利用する側にとって重要な意味を持ちます。

例えば作成するAndroidアプリケーションのminSdkVersionを"10"(Android 2.3.3)に設定するとします。この時、アプリケーションが利用するライブラリプロジェクトはminSdkVersionが10以下でなければなりません(@<img>{library-minSdkVersion})。

//image[library-minSdkVersion][ライブラリプロジェクトとminSdkVersionの関係]{
//}

アプリケーションをコンパイルする時、minSdkVersionの齟齬については警告してくれません。この為ライブラリプロジェクトが指定しているminSdkVersionよりも低いバージョンの端末にアプリケーションをインストールして実行できてしまいます。その端末のAndroidバージョンには存在しないAPIを利用している場合にアプリケーションがクラッシュしてしまいます。

===  Activityをライブラリに含める

ライブラリプロジェクトにActivityを追加してみましょう。追加そのものは通常のAndroidアプリケーションと同じ手順でできますが、ライブラリプロジェクト内のActivityを利用する際に注意点があります。

==== LibraryActivityをMyLibraryに追加する

MyLibraryに"LibraryActivity"という名前のActivityを追加してください。

//list[LibraryActivity][LibraryActivity.java]{
package com.techinstitute.mylibrary;

public class LibraryActivity extends Activity {
  public static Intent createIntent(Context context) {
    return new Intent(context, LibraryActivity.class);
  }

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_library);
	}
}
//}

==== LibraryActivityを起動する処理を実装する

MyLibraryをビルドしたあと、UseMyLibraryのMainActivityにLibraryActivityを起動する処理を実装しましょう。

//list[UseLibraryActivity][MainActivityからLibraryActivityを起動する]{
public class MainActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		findViewById(R.id.button).setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = LibraryActivity.createIntent(MainActivity.this);
				startActivity(intent);
			}
		});
	}
}
//}

問題がなければコンパイルが通り、アプリケーションを実行できるはずです。しかしアプリケーションを起動しLibraryActivityを起動しようとするとクラッシュしてしまいます。

==== AndroidManifest.xmlにactivityを追加する

アプリケーションがクラッシュしてしまう理由はUseMyLibraryのAndroidManifest.xmlにLibraryActivityが定義されていないからです。通常のActivityと同様に、ライブラリプロジェクトのActivityを利用する場合も次のようにAndroidManifest.xmlを定義しておく必要があります。

//list[AndroidManifest.xml][UseMyLibraryのAndroidManifest.xmlにactivityを追加する]{
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
  package="com.techinstitute.usemylibrary">
  <application>
    <!-- 省略 -->

    <activity android:name="com.techinstitute.mylibrary.LibraryActivity"/>

  </application>
</manifest>
//}

Activityだけでなく、ServiceやBroadcastReceiverなどAndroidManifest.xmlに定義が必要なコンポーネントを利用する場合はアプリケーション側のAndroidManifest.xmlに定義しなければなりません。こちらもアプリケーションをコンパイルした時点ではエラーとならないので、minSdkVersionと同様に気をつけなければならない点です。

=== 演習問題

(1) 今まで作ったプログラムをライブラリ化してみましょう。

例えば11章の第1節で作成したペイントViewなど、今まで作ったプログラムを再利用できるようにしてみましょう。

=== まとめ

本節ではライブラリプロジェクトの作り方と、minSdkVersionやActivityをライブラリプロジェクトに含める場合の注意点について学びました。ライブラリプロジェクトにアプリケーションで共通に利用できる機能を実装していく事で、効率的に開発が行えるようになります。アプリケーションを開発する際、共通で利用できそうなクラスができた場合はライブラリプロジェクトに含める事を検討しましょう。
