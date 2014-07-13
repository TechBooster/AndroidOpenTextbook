= 外部ライブラリ 1日目

本章では、Androidアプリケーション開発における外部ライブラリの役割とその使い方、作り方、配布方法を学びます。

==  ライブラリとは何か

ライブラリとは特定の処理を行うプログラムをひとまとめにし、他のプログラムから利用できる形式にしたものの事です。ライブラリを利用する事でアプリケーションは様々な機能を簡単に実装する事が出来るようになります。Androidアプリケーションを開発する際、JavaのAPIやAndroidフレームワークのAPIを利用しますがこれらもライブラリです。ライブラリはアプリケーション開発において無くてはならない存在なのです。

//image[library-figure][アプリケーションとライブラリの関係イメージ図]{
//}


== Androidアプリケーションおけるライブラリ

Androidアプリケーションで利用できるライブラリにはjar(Java Archive)、ライブラリプロジェクト、aar(Android Archive)があります。これらはJavaから参照できるライブラリです。この他NDKを用いてC言語でプログラムを記述する場合にはso(Shared Object)というライブラリ形式を利用できます。

#@# 参考
#@# jar:http://www.cse.yorku.ca/tech/other/jdk1.2.1/docs/guide/jar/
#@# manifest format:http://www.cse.yorku.ca/tech/other/jdk1.2.1/docs/guide/jar/manifest.html


=== jar(Java Archive)

Androidアプリケーションは基本的にJavaでソースコードを記述します。その為既存のJavaライブラリの多くを利用できます。Javaライブラリの形式はjar(Java Archive)と呼ばれ、classファイル群と署名やバージョン等のメタ情報が書かれたファイルがアーカイブされています。

NDKを用いる事でC言語を使ってプログラムを記述する事もできます。その為

=== ライブラリプロジェクト

=== aar(Android Archive)

=== so(Shared Object)


== 静的リンクと動的リンク

ライブラリはソースコード、オブジェクトコード、専用のフォーマットなど様々な形式で提供されており、利用するプログラムはそれらの形式に合わせてライブラリと結びつく必要があります。ライブラリと結びつく方式は大きく分けると静的リンクと動的リンクに分類できます。

=== 静的リンク

静的リンクはアプリケーションとライブラリをコンパイルのリンク時に結びつけ、実行ファイルに含める方式です。静的リンクの場合リンク時にライブラリのAPIやバージョン等の整合性をチェックする事になるので、コンパイルの時点で問題があれば検出できます。ただし、実行ファイルにライブラリファイルを含めるので実行ファイルのサイズは大きくなってしまいます。また、ライブラリをアップデートするには再度アプリケーションをコンパイルし直す必要があります。Androidにおいてはライブラリプロジェクトやaar、exportedなjarが静的リンクになります。

=== 動的リンク

動的リンクはアプリケーションを実行する時にライブラリとリンクする方式です@<fn>{dll}。実行ファイルとは独立したファイルなので、実行ファイルのサイズを圧迫する事がありません。また複数のアプリケーションから同時に利用したり、ファイルを差し替えれば再コンパイルする事なくアップデートしたりできます。しかしライブラリが互換性のないアップデートをしてしまった場合は動作時にエラーとなってしまいます。その為動的リンク形式のライブラリを利用する場合はバージョン管理が重要になります。AndroidではAndroidフレームワークが動的リンク形式のライブラリとなります。

//footnote[dll][Windowsを利用している方は「.dll(Dynamic Link Library)」という拡張子のファイルを見たことがあると思います。これが動的リンクができるライブラリです。]

== なぜライブラリが必要なのか

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

importしているandroid.app.Activityクラスやandroid.os.Bundleクラス、MainActivityクラスのonCreateメソッドや、setContentViewメソッドがライブラリに依存したコードです。これらのクラスやメソッドはAndroid SDKが提供しているライブラリによって解決しています。このライブラリをAndroidフレームワークと呼びます。Androidプロジェクトのソースコードがコンパイルできるのは、Androidフレームワークのライブラリをコンパイル時に参照しているからです。

EclipseでAndroidプロジェクトを見てみると、Androidフレームワークのバージョンや中身を確認できます。@<img>{android-framework}はAndroid 4.4.2のフレームワークを参照している例です。anrdoid.jarがAndroidフレームワークのライブラリの本体です。ライブラリの中にどのようなクラスがあるか確認できます。

//image[android-framework][Androidプロジェクトで参照しているAndroidフレームワーク]{
//}

===[column] 動的リンクとバージョン互換の問題

API Level違いで発生する問題

===[/column] 

== 演習1 

 ネットワーク章で作ったHttpURLConnectionの処理を@<href>{https://github.com/loopj/android-async-http,android-async-http}に置き換えてみよう

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






