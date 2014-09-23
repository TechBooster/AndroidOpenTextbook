= 外部ライブラリの利用と作成

本章では、Androidアプリケーション開発における外部ライブラリの役割とその使い方、作り方を学びます。

== 外部ライブラリを使ってみよう

===  ライブラリとは何か

ライブラリとは特定の処理を行うプログラムをひとまとめにし、他のプログラムから利用できる形式にしたモノの事です。ライブラリは再利用のための仕組みだと言い表してもよいでしょう。例えば画像処理や数値計算などの汎用的な処理を行うプログラムをライブラリにする事で、他のプログラムからその機能を自由に利用できるようになります。一度開発したプログラムを再度使いまわせるという事です。

//image[library-figure][アプリケーションとライブラリの関係イメージ図]{
//}

複数のライブラリを組み合わせてより高機能なプログラムを作成し、それをまたライブラリにするといった事も可能です。

Android自身も画像処理や音声処理や様々な部分でライブラリを利用しています。また、Androidアプリケーションを開発する際に利用するAndroidフレームワークもライブラリです。Androidが提供するライブラリによって私達はアプリケーションを開発する事ができるのです。Androidに限らず、現代の様々なプラットフォームやプロダクトは過去に開発されたライブラリの積み重ねによって成り立っていると言っても過言ではありません。

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

//TODO 図 アプリケーションと一緒にライブラリがあるイメージ

//footnote[exported jar][通常jarファイルは動的リンク形式のライブラリです。しかしAndroidアプリケーションはDalvikVMというJavaとは異なる処理系で動作します。この為、classファイルをDalvikVM用のdexファイルに変換する際に、参照しているjarファイルの中身もdexファイルに変換し、アプリケーションに組み込みます。コンパイルのタイミングでアプリケーション内にライブラリを組み込んでいるのでAndroidアプリケーションではjarファイルを静的リンクしているという事になります。exportedなjarとはdexファイルに変換する対象に含めるjarファイルの事を指します。例えばandroid.jarはexportedではないのでコンパイル時には処理されません。アプリケーションを実行する時に動的にリンクします。]

==== 動的リンク

動的リンクはアプリケーションを実行する時にライブラリとリンクする方式です@<fn>{dll}。実行ファイルとは独立したファイルなので、実行ファイルのサイズを圧迫する事がありません。また複数のアプリケーションから同時に利用したり、ファイルを差し替えれば再コンパイルする事なくライブラリをアップデートしたりできます。しかしライブラリが互換性のないアップデートをしてしまった場合は動作時にエラーとなってしまいます。その為動的リンク形式のライブラリを利用する場合はバージョン管理が重要になります。AndroidではAndroidフレームワークやsoファイルが動的リンク形式のライブラリとなります。例えば最新のAndroidのAPIを使ったアプリケーションを、古いAndroidが搭載された端末で動作させるとエラーが発生します。これは動的リンクによってAndroidフレームワークとアプリケーションが結びついているからです。古いAndroidに存在しないAPIを利用していたりすると、当然APIが見つからないのでエラーとなるのです。

//TODO 図 システムの中にライブラリがあるイメージ

//footnote[dll][Windowsを利用している方は「.dll(Dynamic Link Library)」という拡張子のファイルを見たことがあると思います。これが動的リンクができるライブラリです。]

=== Androidフレームワークもライブラリ

前述の通りAndroidフレームワークもライブラリです。しかしAndroidアプリケーションを開発する時「Androidフレームワークのライブラリを使っている」と意識した事はあまりないかもしれません。実は新規プロジェクトを作成する時にAndroidフレームワークを選択しています。

//image[choose-android-framework][新規プロジェクトのウィザード]

新規プロジェクトのウィザードで選択する「Compile With」はどのバージョンのAndroidフレームワークを使ってプロジェクトをコンパイルをするか設定する項目です。この設定に従ってAndroidプロジェクトにAndroidフレームワークへの参照が追加されます。

EclipseでAndroidプロジェクトの「Package Explorer」を見てみると、参照しているAndroidフレームワークのバージョンや中身を確認できます。@<img>{android-framework}はAndroid 4.4.2のandroid.jarを参照している例です。ライブラリの中にどのようなクラスがあるか確認できます。

//image[android-framework][Androidプロジェクトで参照しているAndroidフレームワーク。膨大なクラスが含まれているほか、そのクラスが持つメソッドを見る事もできる]{
//}

==== Androidフレームワークを使ったプログラム

@<list>{android.jar}はAndroidアプリケーションを開発するにあたって最初に目にするコードの一つです。
このコードの中でライブラリを利用している部分がどこか分かるでしょうか。

//list[android.jar][Androidアプリケーションのソースコード]{
import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
  }
}
//}

importしているandroid.app.Activityクラスやandroid.os.Bundleクラス、MainActivityクラスのonCreateメソッドや、setContentViewメソッドがライブラリに依存したコードです。ほとんど全部ですね。これらのクラスやメソッドはAndroid SDKが提供しているAndroidフレームワークのライブラリ(android.jar)によって解決しているわけです。Androidプロジェクトのソースコードがコンパイルできるのは、android.jarをコンパイル時に参照しているからです。

=== jar形式のライブラリを使ってみよう

ライブラリの形態とAndroidアプリケーションにおけるライブラリの種類がわかった所で、早速ライブラリを使ってみましょう。本項では@<href>{https://github.com/loopj/android-async-http,android-async-http}という非同期通信のためのライブラリを使って、ネットワークプログラミングの章(14 -2 -5 HttpURLConnection の利用)で作ったHttpURLConnectionの処理を置き換えます。android-async-httpはさまざまな形式で配布していますが、ここではjarファイルを直接利用する事とします。

==== jarファイルをダウンロードし、libsディレクトリへコピーする

android-async-httpのjarファイルは@<href>{http://loopj.com/android-async-http/}でダウンロードできます。本書の執筆時点では「android-async-http-1.4.6.jar」をダウンロードできました@<img>{android-http-async-download}。

//image[android-http-async-download][android-http-asyncのjarファイルをダウンロードする]{
//}

ダウンロードしたjarファイルをAndroidプロジェクトのlibsディレクトリにコピーすれば利用できます。

//image[copy-jar][jarファイルをlibsディレクトリにコピーする]{
//}

libsディレクトリへのjarファイルのコピーの他に、AndroidManifest.xmlへ"android.permission.INTERNET"パーミッションを追加する事も忘れずに行いましょう。

//list[add-internet-permission][AndroidManifest.xmlへ"android.permission.INTERNET"パーミッションを追加する]{
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="省略" >

  <uses-permission android:name="android.permission.INTERNET"/>

  <application>
    <!-- 省略 -->
  </application>
</manifest>
//}

==== ライブラリを用いない場合のコード

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

Socketを使った通信処理に比べればHttpURLConnectionはかなり楽に通信処理を記述できるようになっています。しかしまだまだ冗長です。HttpURLConnectionを使って通信処理をする場合常に以下の事を考慮しなければなりません。

 * AsyncTaskなど非同期処理のコードを追加しなければならない
 * 通信の成功や失敗を自分でチェックしなければならない
 * レスポンスをInputStreamで処理しなければならない

こうした課題はある意味でチャンスだとも言えます。これらの課題を解決する便利なライブラリを作る事ができればAndroidアプリケーションの開発がより楽に、高品質になるからです。一方で「車輪の再発明」という考え方もプログラマの世界にはあります。「車輪の再発明」は既に課題を解決する成果物が存在するにも関わらず、1から開発をする事を表します。

学習のために「車輪の再発明」をする事はよくありますが、今回は既に解決策として公開されているライブラリの一つを使ってみる事にしてみましょう。

==== android-async-httpを用いる

android-async-httpは主に非同期的

もちろん非同期通信処理のライブラリはandroid-async-httpだけではありません。画像に特化したものや、SPDYなどの新しいプロトコルをサポートしたもの、

==== android-async-httpを用いたコード

次はandroid-async-httpを用いたコードを見てみましょう。android-async-httpは主に非同期的にネットワーク処理を行うAsyncHttpClientクラスと、同期的にネットワーク処理を行うSyncHttpClientクラスを提供しています。@<list>{library}ではAsyncHttpClientを使って非同期のリクエスト処理をしています。実装の詳細はここでは深く立ち入りませんが、簡潔にリクエスト処理を記述できている事が分かると思います。

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
    client.get("http://tomorrowkey.github.io",
        new AsyncHttpResponseHandler() {
          @Override
          public void onSuccess(int responseCode, Header[] headers, byte[] response) {
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

AsyncHttpClientクラスは自動的に非同期でネットワーク処理を行うのでUIスレッドから呼び出しても問題ありません。また通信完了後に呼び出されるコールバックはUIスレッドで実行されます。

=== jarライブラリが利用できる仕組み

 クラスパスについて
 1章のあたりで解説していないか確認する

=== ライブラリを組み合わせてみよう

android-http-asyncによって非同期通信にまつわる面倒な部分を省略できました。XMLを簡単に取得できるようになりましたが、次はXMLをどういう風に表示させるかが問題になってきます。
RSSを解析して、各記事のデータを取り出しリストで表示させるにはどのようにすればよいでしょうか？ここでもライブラリの力を借りましょう。

jsoup


他のフィードを試したい人はこちらを参考に。
http://headlines.yahoo.co.jp/rss/list


//TODO urlを変える

//TextViewに出す

http://rss.dailynews.yahoo.co.jp/fc/computer/rss.xml


=== 演習問題


=== まとめ











== ライブラリプロジェクトを使ってみよう

=== ライブラリプロジェクトとはなにか


実はすでに使っている support-v7

=== ライブラリプロジェクトで出来ること



=== ライブラリプロジェクトを使ってみよう

bootstrapで

https://github.com/Bearded-Hen/Android-Bootstrap

=== ライブラリとライセンス

 - GPL, Apache2, MITについて解説

=== ライブラリのドキュメントを読む

=== ライブラリのソースを読む





== ライブラリプロジェクトを作ってみよう

=== ライブラリプロジェクトを作る前に

設計について


=== ライブラリプロジェクトをつくってみよう

Calculatorクラスを作って、足し算メソッドを追加する

他のアプリケーションから参照して使う

=== カスタムアトリビュートを作成する

11章で作成したペイントViewクラスをライブラリ化してみましょう。

カスタムアトリビュートを使って初期のペンの色設定を追加してみよう

===  Activityをライブラリに含める
