= 外部ライブラリの利用と作成

本章では、Androidアプリケーション開発における外部ライブラリの役割とその使い方、作り方、配布方法を学びます。

== 外部ライブラリを使ってみよう

===  ライブラリとは何か

ライブラリとは特定の処理を行うプログラムをひとまとめにし、他のプログラムから利用できる形式にしたモノの事です。ライブラリは再利用のための仕組みだと言い表してもよいでしょう。例えば画像処理や数値計算などの汎用的な処理を行うプログラムをライブラリにする事で、他のプログラムからその機能を自由に利用できるようになります。一度開発したモノを再度使いまわせるという事です。

//image[library-figure][アプリケーションとライブラリの関係イメージ図]{
//}

こうしたライブラリの仕組みを利用する事で、複数のライブラリを組み合わせてより高機能なプログラムを作成し、それをまたライブラリにするといったサイクルが可能となります。

Android自身も画像処理や音声処理や様々な部分でライブラリを利用しています。Androidに限らず、現代の様々なプラットフォームやプロダクトは過去に開発されたライブラリの積み重ねによって成り立っていると言っても過言ではありません。

=== Androidアプリケーションおけるライブラリ

ライブラリには様々な形式がありますが、Androidアプリケーションで利用できるライブラリはjar(Java Archive)、ライブラリプロジェクト、aar(Android Archive)、so(Shared Object)の4つです。

==== jar(Java Archive)

Androidアプリケーションは基本的にJavaでソースコードを記述します。その為既存のJavaライブラリの多くを利用できます。Javaライブラリの形式はjar(Java Archive)と呼ばれ、classファイル群と署名やバージョン等のメタ情報が書かれたファイルなどがアーカイブされています@<fn>{jar features}。コンパイル時にjarファイルのパスを指定する事で利用できます。Androidアプリケーションの場合プロジェクトのlibsディレクトリにjarファイルを配置するだけで自動的にパスが通り、利用が可能となります@<fn>{jar for eclipse}。

//footnote[jar features][Java Archive (JAR) Features : http://www.cse.yorku.ca/tech/other/jdk1.2.1/docs/guide/jar/]
//footnote[jar for eclipse][Eclipse + ADT pluginの環境を用いた場合です。Android Studioの場合はbuild.gradleにjarファイルの参照を記述します。]

==== ライブラリプロジェクト

ライブラリプロジェクトはAndroidアプリケーションの為の専用フォーマットです。通常のアプリケーションと同様にプロジェクト形式で開発を行います。ライブラリプロジェクトにはソースコードの他にレイアウトXMLや画像等のリソースファイルを含められます。これにより、画面を含んだUIやスタイル、画像リソースなどをアプリケーションから参照して利用できます。

==== aar(Android Archive)

  aarはライブラリプロジェクトをアーカイブした形式です。2013年のGoogle I/OでAndroid Studioという新しいIDEが発表されました。Android StudioはGradleというビルドツールと強く統合されており、Gradleを通してアプリケーションをビルドする形になっています。Gradleからライブラリプロジェクトを参照する時に利用するのがこのaar形式です。

==== so(Shared Object)

NDKを用いる事で、C言語を使ってAndroidアプリケーションで利用するプログラムを記述できます。NDKを使ってビルドを行うとsoファイルを出力し、apkに同梱します。AndroidアプリケーションからsoファイルへはJNI(Java Native Interface)を通してアクセスする事になります。

=== 静的リンクと動的リンク

ライブラリはソースコード、オブジェクトコード、専用のフォーマットなどさまざまな形式で提供されており、利用するプログラムはそれらの形式に合わせてライブラリと結びつく必要があります。ライブラリと結びつく方式は大きく分けると静的リンクと動的リンクに分類できます。

==== 静的リンク

静的リンクはアプリケーションとライブラリをコンパイルのリンク時に結びつけ、実行ファイルに含める方式です。静的リンクの場合リンク時にライブラリのAPIやバージョン等の整合性をチェックする事になるので、コンパイルの時点で問題があれば検出できます。ただし、実行ファイルにライブラリファイルを含めるので実行ファイルのサイズは大きくなってしまいます。また、ライブラリをアップデートするには再度アプリケーションをコンパイルし直す必要があります。Androidにおいてはライブラリプロジェクトやaar、exportedなjarが静的リンクになります@<fn>{exported jar}。

//footnote[exported jar][通常jarファイルは動的リンク形式のライブラリです。しかしAndroidアプリケーションはDalvikVMというJavaとは異なる処理系で動作します。この為、classファイルをDalvikVM用のdexファイルに変換する際に、参照しているjarファイルの中身もdexファイルに変換し、アプリケーションに組み込みます。コンパイルのタイミングでアプリケーション内にライブラリを組み込んでいるのでAndroidアプリケーションではjarファイルを静的リンクしているという事になります。exportedなjarとはdexファイルに変換する対象に含めるjarファイルの事を指します。例えばandroid.jarはexportedではないのでコンパイル時には処理されません。アプリケーションを実行する時に動的にリンクします。]

==== 動的リンク

動的リンクはアプリケーションを実行する時にライブラリとリンクする方式です@<fn>{dll}。実行ファイルとは独立したファイルなので、実行ファイルのサイズを圧迫する事がありません。また複数のアプリケーションから同時に利用したり、ファイルを差し替えれば再コンパイルする事なくアップデートしたりできます。しかしライブラリが互換性のないアップデートをしてしまった場合は動作時にエラーとなってしまいます。その為動的リンク形式のライブラリを利用する場合はバージョン管理が重要になります。AndroidではAndroidフレームワークやsoファイルが動的リンク形式のライブラリとなります。

//footnote[dll][Windowsを利用している方は「.dll(Dynamic Link Library)」という拡張子のファイルを見たことがあると思います。これが動的リンクができるライブラリです。]

=== Androidフレームワークもライブラリ

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

importしているandroid.app.Activityクラスやandroid.os.Bundleクラス、MainActivityクラスのonCreateメソッドや、setContentViewメソッドがライブラリに依存したコードです。ほとんど全部ですね。これらのクラスやメソッドはAndroid SDKが提供しているAndroidフレームワークのAPIが定義をしたライブラリ(android.jar)によって解決しています。Androidプロジェクトのソースコードがコンパイルできるのは、android.jarをコンパイル時に参照しているからです。

EclipseでAndroidプロジェクトを見てみると、参照しているAndroidフレームワークのバージョンや中身を確認できます。@<img>{android-framework}はAndroid 4.4.2のandroid.jarを参照している例です。ライブラリの中にどのようなクラスがあるか確認できます。

//image[android-framework][Androidプロジェクトで参照しているAndroidフレームワーク]{
//}

=== jar形式のライブラリを使ってみよう

ライブラリの形態とAndroidアプリケーションにおけるライブラリの種類がわかった所で、早速ライブラリを使ってみましょう。本項では@<href>{https://github.com/loopj/android-async-http,android-async-http}という非同期通信のためのライブラリを使って、ネットワークプログラミングの章(14 -2 -5 HttpURLConnection の利用)で作ったHttpURLConnectionの処理を置き換えます。android-async-httpはさまざまな形式で配布していますが、ここではjarファイルを直接利用する事とします。

==== jarファイルをダウンロードし、libsディレクトリへコピーする

android-async-httpのjarファイルは@<href>{http://loopj.com/android-async-http/}でダウンロードできます。ダウンロードしたjarファイルをAndroidプロジェクトのlibsディレクトリにコピーすれば利用できます。

//TODO 画像

=== ライブラリを用いない場合のコード

まずはライブラリを使わない場合のコードをおさらいしてみましょう。@<list>{scratch}はネットワークプログラミングの章で実装したHttpURLConnectionを用いるコードから一部のログ出力を取り除いたものです。接続の準備やデータの取得、後処理などたくさんの処理を記述しています。また、このコードはUIスレッドでは実行できないため、AsyncTask等を用いて別のスレッドで呼び出す必要があります。

//list[scratch][HttpURLConnection]{
private void scratch() {
  try {
    URL url = new URL("http://tomorrowkey.github.io");
    HttpURLConnection connection = (HttpURLConnection) url
        .openConnection();
    connection.setRequestMethod("GET");
    connection.setRequestProperty("Host", "tomorrowkey.github.io");
    connection.connect();

    int responseCode = connection.getResponseCode();
    InputStream inputStream = connection.getInputStream();
    String body = readToEnd(inputStream);
    Log.d("TEST", "body=" + body);
    inputStream.close();
  } catch (MalformedURLException e) {
    throw new RuntimeException(e);
  } catch (IOException e) {
    throw new RuntimeException(e);
  }
}
//}

=== android-async-httpを用いたコード

次はandroid-async-httpを用いたコードを見てみましょう。android-async-httpは主に非同期的にネットワーク処理を行うAsyncHttpClientクラスと、同期的にネットワーク処理を行うSyncHttpClientクラスを提供しています。@<list>{library}ではAsyncHttpClientを使って非同期のリクエスト処理をしています。実装の詳細はここでは深く立ち入りませんが、簡潔にリクエスト処理を記述できている事が分かると思います。

//list[library][HttpURLConnection]{
private void library() {
  AsyncHttpClient client = new AsyncHttpClient();
  client.get("http://tomorrowkey.github.io",
    new AsyncHttpResponseHandler() {
    @Override
    public void onSuccess(int responseCode, Header[] headers,
      byte[] response) {
      String body = new String(response);
    }

    @Override
    public void onFailure(int responseCode, Header[] headers,
      byte[] response, Throwable e) {

    }
    });
}
//}

AsyncHttpClientクラスは自動的に非同期でネットワーク処理を行うのでUIスレッドから呼び出しても問題ありません。また実行後の


=== ライブラリを組み合わせる

android-http-asyncによって非同期通信にまつわる面倒な部分を省略できました。XMLを簡単に取得できるようになりましたが、次はXMLをどういう風に表示させるかが問題になってきます。
RSSを解析して、各記事のデータを取り出しリストで表示させるにはどのようにすればよいでしょうか？ここでもライブラリの力を借りましょう。

jsoup


他のフィードを試したい人はこちらを参考に。
http://headlines.yahoo.co.jp/rss/list



















== ライブラリプロジェクトを使ってみよう

=== ライブラリプロジェクトとはなにか


実はすでに使っている support-v7


=== ライブラリプロジェクトで出来ること



=== ライブラリプロジェクトを使ってみよう

bootstrapで

https://github.com/Bearded-Hen/Android-Bootstrap



== ライブラリプロジェクトを作ってみよう

=== ライブラリプロジェクトを作る前に

設計について


=== ライブラリプロジェクトをつくろう

Calculatorクラスを作って、足し算メソッドを追加する

他のアプリケーションから参照して使う

=== ペイントViewをライブラリ化しよう


11章で作成したペイントViewクラスをライブラリ化してみましょう。

カスタムアトリビュートを使って初期のペンの色設定を追加してみよう
