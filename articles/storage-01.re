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

== SQLite

SQLiteとは、Androidで標準で利用可能なデータベース管理システムのことです。本節では、このSQLiteの使い方を学びます。

=== リレーショナルデータベース

SharedPreferencesはKey-Value形式でデータを管理する仕組みでしたが、関係モデル（リレーショナルモデル）に基づいてデータを管理するシステムもあります。このシステムを「リレーショナルデータベースマネジメントシステム（RDBMS）」と呼び、SQLiteはこのRDBMSの一つです。RDBMSによって構築されるデータベースを「リレーショナルデータベース」と呼び、データベース内の関係（リレーション）は一般に「表（テーブル）」と呼ばれます。図xxxに、RDBMSとリレーショナルデータベース・テーブルの関係を示します。

=== テーブル・列・行

リレーショナルデータベースでは、データは図xxxのように表の形式で保存されています。この表のことを「テーブル」と呼びます。テーブルには名前があり、１つのデータベースに複数のテーブルを格納することができます。次に、テーブルの「名前」や「年齢」に相当するものを「列」と呼びます。「列」には名前の他に、数値や文字列などの「型」の情報も持ちます。最後に、テーブル内のデータ１つ１つを「行」と呼びます。各データがどのようなフィールドで構成されているかは、「列」を見ればわかりますよね。

=== SQLとは

Androidアプリ開発では、端末に対する命令などをJavaという言語で記述しています。これに対し、RDBMSに対する命令（問い合わせ）はSQLと呼ばれる言語で記述します。Androidで用意されているSQLiteに対する問い合わせもSQLで記述します。

SQLの文法は、大きく3種類に分けられます。

 * データ定義言語(DDL)
 * データ操作言語(DML)
 * データ制御言語(DCL)

SQLについて詳しく説明すると、それだけで数百ページの教科書となってしまいます。ですので、本章ではAndroidでSQLiteを使うために最低限必要となる文法のみ解説します。

=== テーブルを作ってみよう

まず、SQLに慣れるところから始めましょう。SQLでデータベースにテーブルを作るには、CREATE文を使用します。

//emlist[CREATE文]{
CREATE TABLE <テーブル名>(
  列名1 <型>,
  列名2 <型>,
  ...)
//}

<型>には、SQLiteでは次の5つが指定できます。

 * TEXT
 * INTEGER
 * NUMERIC
 * REAL
 * NONE

次は、名前と年齢、２つの列をもつuserというテーブルを作る例です。

//emlist[userテーブルを作る]{
CREATE TABLE user(
  name TEXT,
  age INTEGER)
//}

=== 練習問題

=== テーブルに行を追加しよう

テーブルができたら、次はそのテーブルにデータ（行）を追加しましょう。行の追加はINSERT文を使います。

//emlist[INSERT文]{
INSERT INTO <テーブル名>(列名1,列名2,...) VALUES(値1,値2,...)
//}

次は、userテーブルに名前=fkm/年齢=30という行を追加する例です。

//emlist[userテーブルに行を追加する]{
INSERT INTO user(name, age) VALUES('fkm', 30)
//}

列名と値の対応がとれていれば、次のように列の順序を入れ替えても正しく動作します。

//emlist[列の順序を変えてINSERT]{
INSERT INTO user(age, name) VALUES(30, 'fkm')
//}

=== テーブルから行を取得しよう

テーブルに追加した行（データ）を取得するには、SELECT文を使います。すべてのパターンを説明すると膨大な量になるので、ここでは代表的なもののみ解説します。

//emlist[すべて取得するSELECT文]{
SELECT * FROM <テーブル名>
//}

指定した列だけ取得したい場合は、SELECTの後に*ではなく列名をカンマ区切りで指定します。次は、名前列だけ取得するSELECT文です。

//emlist[名前だけ取得するSELECT文]{
SELECT name FROM user
//}

指定した条件を満たす行（データ）だけ取得したい場合は、FROM <テーブル名> の後にWHEREと条件を追加します。次は、年齢が20歳未満の行を取得するためのSELECT文です。

//emlist[20歳未満の行を取得するSELECT文]{
SELECT * FROM user WHERE age < 20
//}

条件部分には、次のような演算子が使用できます。

 * A = B : AとBが等しい
 * A <> B : AとBが等しくない
 * A < B : AがBより小さい
 * A <= B : AがBより小さいか、等しい
 * A > B : AがBより大きい
 * A >= B : AがBより大きいか、等しい

また、条件をANDやORでつなげることもできます。次は身長(height)が180より大きく、収入(earnings)が1000以上である行を取得するSELECT文です。

//emlist[ANDを使う例]{
SELECT * FROM worker WHERE height > 180 AND earnings >= 1000
//}

=== 行を更新しよう

テーブル内の行を更新するには、UPDATE文を使用します。

//emlist[UPDATE文]{
UPDATE <テーブル名> SET 列名1=値,列名2=値,... WHERE 条件
//}

WHEREの部分は、SELECT文で紹介した条件を指定し、条件に一致した列の、指定した列を更新します。WHEREの指定を忘れると、テーブル内のすべての行を更新してしまうので注意しましょう。次は、id=1の行の収入を300に更新する例です。

//emlist[UPDATE文の例]{
UPDATE worker SET earnings=300 WHERE id=1
//}

=== 行を削除しよう

テーブル内の行を削除するには、DELETE文を使用します。

//emlist[DELETE文]{
DELETE FROM <テーブル名>  WHERE 条件
//}

WHEREの部分は、SELECT文で紹介した条件を指定し、条件に一致した列を削除します。WHEREの指定を忘れると、テーブル内のすべての行を削除してしまうので注意しましょう。次は、id=2の行を削除する例です。

//emlist[DELETE文の例]{
DELETE FROM worker WHERE id=2
//}

=== Androidで使ってみよう

SQLと少しお友達になれたところで、AndroidアプリでSQLiteを使ってみましょう。AndroidでSQLiteを使うには、次の2ステップが必要です。

 * SQLiteOpenHelperクラスを継承したクラスを定義する
 * 必要な場面で、定義したクラスのオブジェクトを作る

さっそく、やってみましょう

=== SQLiteOpenHelperを継承したクラスを定義する

=== MyHelperオブジェクトを作る

=== 行を追加する

=== 行を検索する

=== 行を更新する

=== 行を削除する

=== 練習問題

== File

=== Permission

=== ファイルを作成しよう

=== ファイルを読み込んでみよう

=== フォルダを作成しよう

== まとめ

本章では、Androidアプリ開発で主に利用する3つのAPIの使い方を学びました。アプリが扱うデータの性質に応じて、適切な仕組みを選択できるようになりましょう。


