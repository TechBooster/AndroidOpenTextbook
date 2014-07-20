= 外部ライブラリ 1日目

本章では、Androidアプリケーション開発における外部ライブラリの役割とその使い方、作り方、配布方法を学びます。

==  ライブラリとは何か

ライブラリとは特定の処理を行うプログラムをひとまとめにし、他のプログラムから利用できる形式にしたものの事です。ライブラリを利用する事でアプリケーションはさまざまな機能を簡単に実装する事が出来るようになります。Androidアプリケーションを開発する際、JavaのAPIやAndroidフレームワークのAPIを利用しますがこれらもライブラリです。ライブラリはアプリケーション開発において無くてはならない存在なのです。

//image[library-figure][アプリケーションとライブラリの関係イメージ図]{
//}


== Androidアプリケーションおけるライブラリ

Androidアプリケーションで利用できるライブラリにはjar(Java Archive)、ライブラリプロジェクト、aar(Android Archive)があります。これらはJavaから参照できるライブラリです。この他NDKを用いてC言語でプログラムを記述する場合にはso(Shared Object)というライブラリ形式を利用する事になります。

=== jar(Java Archive)

Androidアプリケーションは基本的にJavaでソースコードを記述します。その為既存のJavaライブラリの多くを利用できます。Javaライブラリの形式はjar(Java Archive)と呼ばれ、classファイル群と署名やバージョン等のメタ情報が書かれたファイルなどがアーカイブされています@<fn>{jar features}。コンパイル時にjarファイルのパスを指定する事で利用できます。Androidアプリケーションの場合プロジェクトのlibsディレクトリにjarファイルを配置するだけで自動的にパスが通り、利用が可能となります@<fn>{jar for eclipse}。

//footnote[jar features][Java Archive (JAR) Features : http://www.cse.yorku.ca/tech/other/jdk1.2.1/docs/guide/jar/]
//footnote[jar for eclipse][Eclipse + ADT pluginの環境を用いた場合です。Android Studioの場合はbuild.gradleにjarファイルの参照を記述します。]

=== ライブラリプロジェクト

ライブラリプロジェクトはAndroidアプリケーションの為の専用フォーマットです。通常のアプリケーションと同様にプロジェクト形式で開発を行います。ライブラリプロジェクトにはソースコードの他にレイアウトXMLや画像等のリソースファイルを含められます。これにより、画面を含んだUIやスタイル、画像リソースなどをアプリケーションから参照して利用できます。

=== aar(Android Archive)

aarはライブラリプロジェクトをアーカイブした形式です。2013年のGoogle I/OでAndroid Studioという新しいIDEが発表されました。Android StudioはGradleというビルドツールと強く統合されており、Gradleを通してアプリケーションをビルドする形になっています。Gradleからライブラリプロジェクトを参照する時に利用するのがこのaar形式です@<fn>{about gradle}。

//footnote[about gradle][実際にはもう少し色々と立て込んでいます。詳細については後述します]

=== so(Shared Object)

NDKを用いる事でC言語を使ってAndroidアプリケーションで利用するプログラムを記述できます。NDKを使ってビルドを行うとsoファイルを出力し、apkに同梱します。AndroidアプリケーションからsoファイルへはJNI(Java Native Interface)を通してアクセスする事になります。

== 静的リンクと動的リンク

ライブラリはソースコード、オブジェクトコード、専用のフォーマットなどさまざまな形式で提供されており、利用するプログラムはそれらの形式に合わせてライブラリと結びつく必要があります。ライブラリと結びつく方式は大きく分けると静的リンクと動的リンクに分類できます。

=== 静的リンク

静的リンクはアプリケーションとライブラリをコンパイルのリンク時に結びつけ、実行ファイルに含める方式です。静的リンクの場合リンク時にライブラリのAPIやバージョン等の整合性をチェックする事になるので、コンパイルの時点で問題があれば検出できます。ただし、実行ファイルにライブラリファイルを含めるので実行ファイルのサイズは大きくなってしまいます。また、ライブラリをアップデートするには再度アプリケーションをコンパイルし直す必要があります。Androidにおいてはライブラリプロジェクトやaar、exportedなjarが静的リンクになります@<fn>{exported jar}。

//footnote[exported jar][通常jarファイルは動的リンク形式のライブラリです。しかしAndroidアプリケーションはDalvikVMというJavaとは異なる処理系で動作します。この為、classファイルをDalvikVM用のdexファイルに変換する際に、参照しているjarファイルの中身もdexファイルに変換され、アプリケーションに組み込まれます。コンパイルのタイミングでアプリケーション内にライブラリを組み込んでいるのでAndroidアプリケーションではjarファイルを静的リンクしているという事になります。exportedなjarとはdexファイルに変換する対象に含めるjarファイルの事を指します。]

=== 動的リンク

動的リンクはアプリケーションを実行する時にライブラリとリンクする方式です@<fn>{dll}。実行ファイルとは独立したファイルなので、実行ファイルのサイズを圧迫する事がありません。また複数のアプリケーションから同時に利用したり、ファイルを差し替えれば再コンパイルする事なくアップデートしたりできます。しかしライブラリが互換性のないアップデートをしてしまった場合は動作時にエラーとなってしまいます。その為動的リンク形式のライブラリを利用する場合はバージョン管理が重要になります。AndroidではAndroidフレームワークやsoファイルが動的リンク形式のライブラリとなります。

//footnote[dll][Windowsを利用している方は「.dll(Dynamic Link Library)」という拡張子のファイルを見たことがあると思います。これが動的リンクができるライブラリです。]

== Androidフレームワークもライブラリ

@<list>{android.jar}はAndroidアプリケーションを開発するにあたって最初に目にするコードの一つです。
このコードの中でライブラリを利用している部分がどこかわかるでしょうか。

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

#@# ===[column] 動的リンクとバージョン互換の問題
#@# 
#@# Androidアプリケーションにおける静的リンクと動的リンクの関係は理解できたと思います。ここでひとつ気になるのは「なぜAndroidフレームワークのandroid.jarはjarファイルなのに動的リンクなのか」という事ではないでしょうか。
#@#
#@# 本コラムでは
#@# Androidアプリケーションを開発する際に参照するandroid.jarの中身を見てみるとわかりますが、
#@#
#@# 機種によってはAndroidフレームワーク部分を修正したり拡張しています。この為特定の機種ではAPIの挙動が異なるという場合があります。
#@#
#@# API Level違いで発生する問題
#@#
#@# ===[/column] 

== 演習1 

ライブラリの形態とAndroidアプリケーションにおけるライブラリの種類がわかった所で、早速ライブラリを使ってみましょう。本演習ではネットワーク章で作ったHttpURLConnectionの処理を@<href>{https://github.com/loopj/android-async-http,android-async-http}に置き換える手順を解説します。android-async-httpはさまざまな形式で配布していますが、ここではjarファイルを直接利用する事とします。

=== jarファイルをダウンロードし、libsディレクトリへコピーする

android-async-httpのjarファイルは@<href>{http://loopj.com/android-async-http/}でダウンロードできます。ダウンロードしたjarファイルをAndroidプロジェクトのlibsディレクトリにコピーすれば利用できます。

=== ライブラリを用いない場合のコード

まずはライブラリを使わない場合のコードをおさらいしてみましょう。@<list>{scratch}はネットワーク章で実装したHttpURLConnectionを用いるコードからログ出力を取り除いたものです。接続の準備やデータの取得、後処理など沢山の処理を記述しています。また、このコードはUIスレッドでは実行できないため、AsyncTask等を用いて別のスレッドで呼び出す必要があります。

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
    inputStream.close();
  } catch (MalformedURLException e) {
    throw new RuntimeException(e);
  } catch (IOException e) {
    throw new RuntimeException(e);
  }
}
//}

=== android-async-httpを用いたコード

次はandroid-async-httpを用いたコードを見てみましょう。android-async-httpは主に非同期的にネットワーク処理を行うAsyncHttpClientクラスと、同期的にネットワーク処理を行うSyncHttpClientクラスを提供しています。@<list>{library}ではAsyncHttpClientを使って非同期のリクエスト処理をしています。実装の詳細はここでは深く立ち入りませんが、簡潔にリクエスト処理を記述できている事がわかると思います。

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

===[column] Android Studio + Gradleを使う場合


===[/column] 


== ライブラリのソースコードを読む



== jarファイルの限界とライブラリプロジェクトの誕生


=== 演習2

 ライブラリプロジェクト @<href>{https://github.com/Bearded-Hen/Android-Bootstrap,Android-Bootstrap} を使ってUIを格好良くしてみよう

 * ライブラリプロジェクトのclone
 * プロジェクトのインポート
 * レイアウトXMLの書き換え
 * 実行

//list[a][デフォルトのmain_activity.xml]{
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
  xmlns:tools="http://schemas.android.com/tools"
  android:layout_width="match_parent"
  android:layout_height="match_parent"
  android:paddingBottom="@dimen/activity_vertical_margin"
  android:paddingLeft="@dimen/activity_horizontal_margin"
  android:paddingRight="@dimen/activity_horizontal_margin"
  android:paddingTop="@dimen/activity_vertical_margin"
  tools:context="com.example.techinstitute.library01.MainActivity" >

  <TextView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="@string/hello_world" />

</RelativeLayout>
//}

//list[a][Android-Bootstrapを適用する]{
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
  xmlns:tools="http://schemas.android.com/tools"
  xmlns:bootstrapbutton="http://schemas.android.com/apk/res-auto"
  android:layout_width="match_parent"
  android:layout_height="match_parent"
  android:paddingBottom="@dimen/activity_vertical_margin"
  android:paddingLeft="@dimen/activity_horizontal_margin"
  android:paddingRight="@dimen/activity_horizontal_margin"
  android:paddingTop="@dimen/activity_vertical_margin"
  tools:context="com.example.techinstitute.library01.MainActivity" >

  <com.beardedhen.androidbootstrap.BootstrapButton
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="@string/hello_world" 
    bootstrapbutton:bb_roundedCorners="true"
    bootstrapbutton:bb_text_gravity="center"
    bootstrapbutton:bb_type="primary" />

</RelativeLayout>
//}

== 依存性という考え方


== maven, gradle






