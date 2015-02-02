= ストレージ

ストレージとは、データを永続的に記録する装置です。Android端末にもストレージはあり、アプリはAPIを介してストレージにアプリのデータを記録したりできます。この章では、Androidアプリ開発で主に用いられる３つのストレージAPIについて学びます。

== ストレージとメモリ

メモリ->アプリを終了させたり電源を切ったりすると消える
ストレージ->電源を切っても消えない

== ファイル

これまでにJavaソースファイルや画像ファイルを扱っていますよね。
PC->ストレージにデータをファイルという形で保存
Androidもストレージへのデータ保存はファイルという形

ファイルシステムはコラムで

== Androidアプリ開発で主に利用するストレージAPI

Androidでは、ストレージに対してデータを作成したり、読み込んだりするためのAPIや仕組みがいくつか用意されています。

 * SharedPreferences
 * SQLiteDatabase
 * File

また、開発したアプリのデータを他アプリに提供したり、他アプリ（たとえば電話帳など）からデータを取得するための仕組みも用意されています。

 * ContentProvider
 * Storage Access Framework（Android 4.4で追加）

== SharedPreferences

SharedPreferencesとは、アプリの設定情報などを保存する仕組みです。SharedPreferencesは、データをKey-Value形式で保存します。

=== SharedPreferencesに保存可能なデータの種類

SharedPreferencesに値として保存可能なデータの種類は次の6種類です。

 * String
 * int
 * long
 * float
 * boolean
 * Set<String>

=== SharedPreferencesを取得する

さっそくSharedPreferencesを使ってデータの読み書きしてみましょう。アプリ内でSharedPreferenceを利用するには、Context(Activity)のgetSharedPreferences()メソッドを呼び、SharedPreferencesオブジェクトをを取得します。第一引数には、自分で決めたSharedPreferences名を指定します。第二引数はContext.MODE_PRIVATEを指定します。

//emlist[SharedPreferencesを取得する]{
SharedPreferences pref =
    getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
//}

=== SharedPreferencesにデータを保存する

次に、取得したSharedPreferencesオブジェクトを用いてデータを保存します。まず、SharedPreferencesオブジェクトのedit()メソッドを呼び、Editorオブジェクトを取得します。Editorにはデータの型に応じたputメソッドが用意されています。ここでは、入力された名前をputString()メソッドで保存します。

保存したいデータをputメソッドで保存したら、最後にEditorオブジェクトのapply()メソッドを呼び、変更を反映させます。

//emlist[SharedPreferencesにデータを保存する]{
private static final String KEY_NAME = "name";

SharedPreferences pref =
    getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
SharedPreferences.Editor edit = pref.edit();
edit.putString(KEY_NAME, name);
edit.apply();
//}

=== SharedPreferencesからデータを読み込む

データをSharedPreferencesに保存できたら、次はそのデータを読み込んでみましょう。データの読み込みもSharedPreferencesオブジェクトを使用します。SharedPreferencesにはデータの種類に応じたgetメソッドが用意されています。ここでは、先ほど保存した名前をgetString()メソッドで読み込んでみます。もし、第一引数で指定したキーに対応する値が無い場合は、代わりに第二引数で指定した値が返却されます。

//emlist[SharedPreferencesからデータを読み込む]{
SharedPreferences pref =
    getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
String name = pref.getString(KEY_NAME, DEFAULT_NAME);
//}

=== SharedPreferencesが苦手とすること

SharedPreferencesはデータの読み書きが数行で実現できる反面、苦手とすることがいくつかあります。

 * データの検索には向かない

SharedPreferencesでデータを読み込むには、キーを事前に知っておく必要があります。なので、「年齢が20であるデータを全部取得したい」のような、検索を必要とするデータの保存には向きません。

 * 複数プロセスで読み書きすると予想外の挙動をすることがある

残念なことに、SharedPreferencesは複数プロセスから読み書きすると、「アプリで保存したはずの値がServiceで読み込めない」といった挙動をすることがあります。詳しくは紹介しませんが、「問題がある」ということは頭の片隅に置いておきましょう。

=== 練習問題

== SQLiteDatabase

=== リレーショナルデータベース

=== SQLとは

=== テーブルを作ってみよう

=== 練習問題

=== テーブルに列を追加しよう

=== テーブルから列を取得しよう

=== テーブル内を検索しよう

=== 列を更新しよう

=== 列を削除しよう

=== 練習問題

== File

=== Permission

=== ファイルを作成しよう

=== ファイルを読み込んでみよう

=== フォルダを作成しよう

== まとめ

本章では、Androidアプリ開発で主に利用する3つのAPIの使い方を学びました。アプリが扱うデータの性質に応じて、適切な仕組みを選択できるようになりましょう。


