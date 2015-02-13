= ストレージ

ストレージとは、データを永続的に記録する装置です。Android端末にもストレージはあり、アプリはAPIを介してストレージにアプリのデータを記録したりできます。この章では、Androidアプリ開発で主に用いられる３つのストレージAPIについて学びます。

== ストレージとメモリ

まず、データの記録について学びましょう。これまでのアプリ開発で登場した変数やフィールドは身近なデータを記録するための仕組みです。変数やフィールドに保存されたデータはアプリ実行中のメモリ上に記録されるため、アプリを終了させたり端末の電源を切ったりすると、記録した内容は失われてしまいます。

これに対し、ストレージに記録されたデータはアプリを終了させたり、端末の電源を切っても失われることはありません（物理的に壊れたりした場合は別ですが）。

== ファイル

データを扱う時の基本単位となるまとまりを「ファイル」と呼びます。既にJavaソースファイルや画像ファイルなどを扱っているため、ファイルがどのようなものかはイメージできるでしょう。PCのハードディスクやSSDなどへの読み書きがファイル単位で行われるのと同様に、Androidもストレージへの読み書きはファイル単位で行われます。

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

SharedPreferencesにデータを保存するには、次の4ステップを行います。

 * Context(Activity)のgetSharedPreferences()で、SharedPreferencesオブジェクトを取得する。
 * SharedPreferencesオブジェクトのedit()で、Editorオブジェクトを取得する。
 * Editorオブジェクトのputメソッドで、保存するデータをセットする。
 * apply()を呼び、変更を反映させる。
 
//emlist[SharedPreferencesにデータを保存する]{
private static final String KEY_NAME = "name";

SharedPreferences pref =
    getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
SharedPreferences.Editor edit = pref.edit();
edit.putString(KEY_NAME, name);
edit.apply();
//}

=== SharedPreferencesからデータを読み込む

SharedPreferencesからデータを読み込むには、次の2ステップを行います。

 * Context(Activity)のgetSharedPreferences()で、SharedPreferencesオブジェクトを取得する。
 * SharedPreferencesオブジェクトのgetメソッドで、保存したデータを取得する。
 
//emlist[SharedPreferencesからデータを読み込む]{
SharedPreferences pref =
    getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
String name = pref.getString(KEY_NAME, DEFAULT_NAME);
//}

ここでは保存した名前を文字列として取得するため、SharedPreferencesオブジェクトのgetString()を呼んでいます。第1引数で指定したキーに対応する値が無かったり、型が違ったりした場合、getString()は第2引数で指定した値を代わりに返します。

=== SharedPreferencesが苦手とすること

SharedPreferencesはデータの読み書きが数行で実現できる反面、苦手とすることがいくつかあります。

 * データの検索には向かない

SharedPreferencesでデータを読み込むには、キーを事前に知っておく必要があります。なので、「年齢が20であるデータを全部取得したい」のような、検索を必要とするデータの保存には向きません。

 * 複数プロセスで読み書きすると予想外の挙動をすることがある

残念なことに、SharedPreferencesは複数プロセスから読み書きすると、「アプリで保存したはずの値がServiceで読み込めない」といった挙動をすることがあります。詳しくは紹介しませんが、「問題がある」ということは頭の片隅に置いておきましょう。

=== 練習問題

@<img>{lesson1-1}のように、TextView / EditText / Buttonを1つずつ配置してください。そして、次のような動作をするよう、SharedPreferencesを用いて処理を記述し動作確認しましょう。

 * Buttonをタップすると、EditTextに入力された内容をSharedPreferencesを用いて保存する。
 * アプリ起動時に、SharedPreferencesに値が保存されていれば@<img>{lesson1-2}のようにTextViewの文字列をセットする。

//image[lesson1-1][練習問題レイアウト][scale=0.35]{
//}

//image[lesson1-2][保存した名前を表示][scale=0.35]{
//}

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

=== テーブルを作る

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

@<table>{lesson2}のようなlectureテーブルを作成するSQLを書いてみましょう。列の型は次のようにします。

 * _id : INTEGER
 * date : INTEGER
 * title : TEXT

//table[lesson2][lectureテーブル]
_id	date	title
---------------------
1	4/2	開発環境セットアップ
2	4/3	Java基礎
3	4/4	Java応用
//}

=== テーブルに行を追加する

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

=== テーブルから行を取得する

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

=== Androidで使う

SQLと少しお友達になれたところで、AndroidアプリでSQLiteを使ってみましょう。AndroidでSQLiteを使うには、次の2ステップが必要です。

 * SQLiteOpenHelperクラスを継承したクラスを定義する。
 * 必要な場面で、定義したクラスのオブジェクトを作り、メソッドを呼ぶ。

=== SQLiteOpenHelperを継承したクラスを定義する

まず、SQLiteOpenHelperを継承したMyHelperというクラスを定義します。

//emlist[MyHelperクラスを定義する]{
public class MyHelper extends SQLiteOpenHelper {
    private static final String DB_NAME = "my.db";
    private static final int DB_VERSION = 1;

    /**
     * コンストラクタ
     */
    public MyHelper(Context context) {
        super(context, DB_NAME, null, DB_VERSION);
    }
}
//}

SQLiteOpenHelperクラスには引数ありコンストラクタが既に定義されているので、MyHelperクラスにもコンストラクタを用意し、super()を呼びます。第2引数はデータベース名、第3引数はnullを指定します。第4引数は自分で決めたデータベースのバージョンを指定します。現時点では1を指定しておけばよいでしょう。

=== onCreate()でテーブル作成SQLを実行する

SQLiteOpenHelperには2つの抽象メソッドが定義されています。

 * public void onCreate(SQLiteDatabase db)
 * public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion)

onCreate()は、データベースのファイルを作る必要が生じた時に呼ばれます。onUpgrade()は、データベースのバージョンに変化が起きた時に呼ばれます。

onCreate()が呼ばれた時に、テーブルを作成するSQLが実行されるようにしましょう。SQLを実行するには、引数で渡されるSQLiteDatabaseオブジェクトのexecSQL()メソッドを呼びます。

//emlist[memoテーブルを作成する]{
public class MyHelper extends SQLiteOpenHelper {

    public static final String TABLE_NAME = "memo";
    private static final String SQL_CREATE_TABLE =
            "CREATE TABLE " + TABLE_NAME + "(" +
            Columns._ID + " INTEGER primary key autoincrement," +
            Columns.MEMO + " TEXT," +
            Columns.CREATE_TIME + " INTEGER," +
            Columns.UPDATE_TIME + " INTEGER)";

    public interface Columns extends BaseColumns {
        public static final String MEMO = "memo";
        public static final String CREATE_TIME = "create_time";
        public static final String UPDATE_TIME = "update_time";
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(SQL_CREATE_TABLE);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        // 現時点では何もしない
    }
}
//}

SQLのCREATE TABLE文は間違いが発生しないよう、文字列定数の足し算で構成しています。実際にexecSQL()メソッドで実行されるSQLは次の通りです。

//emlist[CREATE TABLE文]{
CREATE TABLE memo(
    _id INTEGER primary key autoincrement,
    memo TEXT,
    create_time INTEGER,
    update_time INTEGER)
//}

memoテーブルの主キーをINTEGER型の_idというフィールドに指定しています。Androidでは、主キーがINTEGER型の_idであるテーブルに対して特定の機能を提供するAPIが存在するため、特に理由が無い場合は主キーを_idにすることが多いです。

=== MyHelperオブジェクトを作る

MyHelperクラスの定義ができたら、次はオブジェクトを作りましょう。ここでは、ActivityのonCreate()内で生成し、フィールドにセットしておきます。

//emlist[MyHelperオブジェクトを作る]{
public class MainActivity extends ActionBarActivity {
    private MyHelper mHelper;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // MyHelperオブジェクトを作り、フィールドにセット
        mHelper = new MyHelper(this);
    }
}
//}

=== 行を追加する

データベースに行を追加するには、次の3ステップを行います。

 * MyHelperオブジェクトのgetWritableDatabase()を呼び、SQLiteDatabaseオブジェクトを取得する。
 * 必要なデータを準備し、SQLiteDatabaseオブジェクトのinsert()を呼ぶ。
 * SQLiteDatabaseオブジェクトのclose()を呼び、処理の終了を伝える。

//emlist[行を追加する]{
public class MainActivity extends ActionBarActivity {
    // 中略

    // 引数memoは、画面で入力された内容とします。
    private void insert(String memo) {
        SQLiteDatabase db = mMemoDB.getWritableDatabase();

        // 列に対応する値をセットする
        ContentValues values = new ContentValues();
        values.put(MyHelper.Columns.MEMO, memo);
        values.put(MyHelper.Columns.CREATE_TIME, System.currentTimeMillis());
        values.put(MyHelper.Columns.UPDATE_TIME, System.currentTimeMillis());

        // データベースに行を追加する
        long id = db.insert(MyHelperDB.TABLE_NAME, null, values);
        if (id == -1) {
            Log.v("Database", "行の追加に失敗したよ");
        }

        // データベースを閉じる（処理の終了を伝える）
        db.close();
    }
}
//}

追加する行のデータは、ContentValuesオブジェクトのput()で列毎に指定します。ここでは、次のように値をセットしています。

 * memo列に入力された値
 * create_timeとupdate_timeには現在時刻

=== 行を検索する

次に、追加した行をデータベースを検索して取得しましょう。行の検索は次の7ステップです。

 * MyHelperオブジェクトのgetReadableDatabase()を呼び、SQLiteDatabaseオブジェクトを取得する。
 * SQLiteDatabaseオブジェクトのquery()を呼ぶ。検索結果はCursorオブジェクトとして返却される。
 * CursorオブジェクトのmoveToFirst()を呼び、読み込み中の位置を検索結果の最初の行に移動させる。これがfalseを返した場合は、検索結果は0件。
 * CursorオブジェクトのgetColumnIndex()を呼び、列に対応するインデックスを取得する。
 * do - whileを用いて、１行ずつ読み込み位置をずらしながら行のデータを取得する。
 * Cursorオブジェクトのclose()を呼び、読み込み終了を伝える。
 * SQLiteDatabaseオブジェクトのclose()を呼び、処理の終了を伝える。

ややステップ数が多いので、最初にコード全体を示します。 
 
//emlist[行の検索]{
public class MainActivity extends ActionBarActivity {
    // 中略
    
    private List<Memo> loadMemo() {
        SQLiteDatabase db = mMyHelper.getReadableDatabase();
        
        // query()を呼び、検索を行う。
        Cursor cursor = db.query(MyHelper.TABLE_NAME, null, null, null, null, null,
                MyHelper.Columns.CREATE_TIME + " ASC");
        // 読み込み位置を先頭にする。falseの場合は結果0件
        if (!cursor.moveToFirst()) {
            cursor.close();
            db.close();
            return new ArrayList<>();
        }
        
        // 列のindex（位置）を取得する
        int idIndex = cursor.getColumnIndex(MyHelper.Columns._ID);
        int memoIndex = cursor.getColumnIndex(MyHelper.Columns.MEMO);
        int createIndex = cursor.getColumnIndex(MyHelper.Columns.CREATE_TIME);
        int updateIndex = cursor.getColumnIndex(MyHelper.Columns.UPDATE_TIME);

        // 行を読み込む。
        List<Memo> list = new ArrayList<>(cursor.getCount());
        do {
            Memo item = new Memo();
            item.mId = cursor.getInt(idIndex);
            item.mMemo = cursor.getString(memoIndex);
            item.mCreateTime = cursor.getLong(createIndex);
            item.mUpdateTime = cursor.getLong(updateIndex);

            list.add(item);
        // 読み込み位置を次の行に移動させる。
        // 次の行が無い時はfalseを返すのでループを抜ける
        } while (cursor.moveToNext());

        cursor.close();
        db.close();

        return list;
    }
}    
//}

SQLiteDatabaseオブジェクトのquery()を使用すると、自分で複雑なSELECT文を記述することなく検索が行えます。ここでは全件取得し、作成時刻の昇順でソートしています。

メモリ節約のため、取得する列を指定する場合はquery()の第2引数に列名の配列を指定します。次は、_idとmemo列だけ取得する例です。

//emlist[指定した列だけ取得する]{
String[] columns = {
        MyHelper.Columns._ID, 
        MyHelper.Columns.MEMO
};
Cursor cursor = db.query(MemoDB.TABLE_NAME, columns, null, null, null, null,
        MyHelper.Columns.CREATE_TIME + " ASC");
//}

SQLのWHEREで条件を指定して、特定の行だけ取得するには、第3引数にWHEREの内容を、第4引数には第3引数の?の部分に入れる値を指定します。文章で説明するとイメージしにくいので、_idが指定したものと一致する行だけ取得する例で説明します。

//emlist[WHEREで条件を指定する]{
// idは引数で渡された値とします。
String where = MyHelper.Columns._ID + "=?";
String[] args = { String.valueOf(id) };
Cursor cursor = db.query(MyHelper.TABLE_NAME, null, where, args, null, null,
        MyHelper.Columns.CREATE_TIME + " ASC");
//}

ここでは、第3引数は"_id=?"という文字列になっています。=の右辺はユーザーの操作によって実行時に変化するので、?を指定します。そして、第4引数で?の部分に入れる値を指定しています。第3引数が"height > ? AND earnings > ?"のように、?を複数含む場合は、第4引数は?の数と同じ長さの配列にします。

勘がいい方は、「なぜ第3引数を"_id=" + idのようにしないんだろう？」と思うかもしれません。なぜこのように?を指定し、第4引数で値を指定するかは後ほど紹介します。

=== 行を更新する

行の更新は行の追加と似ています。

 * MyHelperオブジェクトのgetWritableDatabase()を呼び、SQLiteDatabaseオブジェクトを取得する。
 * 必要なデータを準備し、SQLiteDatabaseオブジェクトのupdate()を呼ぶ。
 * SQLiteDatabaseオブジェクトのclose()を呼び、処理の終了を伝える。

//emlist[行を更新する]{
public class MainActivity extends ActionBarActivity {
    // 中略
    
    private void updateMemo(int id, String memo) {
        SQLiteDatabase db = mMyHelper.getWritableDatabase();

        // 更新する値をセット
        ContentValues values = new ContentValues();
        values.put(MyHelper.Columns.MEMO, memo);
        values.put(MyHelper.Columns.UPDATE_TIME, System.currentTimeMillis());

        // 更新する行をWHEREで指定
        String where = MyHelper.Columns._ID + "=?";
        String[] args = { String.valueOf(id) };

        int count = db.update(MyHelper.TABLE_NAME, values, where, args);
        if (count == 0) {
            Log.v("Edit", "Failed to update");
        }

        db.close();
    }
}
//}
 
SQLのUPDATE文で説明しましたが、行の更新はWHEREで更新する行を指定します。WHERE部分の指定はupdate()の第3引数と第4で行います。指定方法はquery()の時と同様で、第3引数で?を含む条件を記述し、第4引数で?にいれる値を指定します。update()は呼ぶと、更新に成功した行数を返します。

=== 行を削除する

行の削除はSQLiteDatabaseオブジェクトのdelete()を呼びます。呼ぶまでの手順は行の更新とほぼ同じです。

 * MyHelperオブジェクトのgetWritableDatabase()を呼び、SQLiteDatabaseオブジェクトを取得する。
 * 必要なデータを準備し、SQLiteDatabaseオブジェクトのdelete()を呼ぶ。
 * SQLiteDatabaseオブジェクトのclose()を呼び、処理の終了を伝える。

//emlist[行の削除]{
public class MainActivity extends ActionBarActivity {
    // 中略

    private void deleteMemo(int id) {
        SQLiteDatabase db = mMyHelper.getWritableDatabase();

        String where = MyHelper.Columns._ID + "=?";
        String[] args = { String.valueOf(id) };

        int count = db.delete(MyHelper.TABLE_NAME, where, args);
        if (count == 0) {
            Log.v("Edit", "Failed to delete");
        }

        db.close();
    }
}
//}
 
こちらもupdate()と同様、どの行を削除するかをdelete()の第2引数と第3引数で指定します。指定方法もupdate()の時と同様です。delete()は呼ぶと削除した行数を返します。

=== 練習問題

== ファイル

本節では、Androidアプリでファイルを読み書きする方法を学びます。

=== ファイルを保存する場所

Androidアプリでファイルを保存できる場所は次の2箇所です。

 * 内部ストレージ
 * （アプリから見た）外部ストレージ

内部ストレージに保存したファイルは、他のアプリは読み書きできませんが、外部ストレージに保存したファイルは、他のアプリから読み書きできます。なので、作成するファイルの用途に応じて使い分けましょう。

=== ファイル(File)とストリーム(Stream)

ファイル(File)とストリーム(Stream)は、Androidアプリでファイルの読み書き時に使用するクラスです。Javaで既にこれらの使い方を学んでいる方はこの節を読み飛ばしてもかまいません。

 * ファイル

ファイルは、ディスク上のファイルやフォルダ（ディレクトリ）を表します。

 * ストリーム

ストリームとは、データの流れを扱う「もの」です。流れてきたデータを読み込むストリームをInputStreaemと呼び、データを流すためのストリームをOutputStreamと呼びます。（図xx）

=== 内部ストレージにファイルを作成する

内部ストレージにファイルを作成するには、次の3ステップを行います。

 * ContextのopenFileOutput()を呼び、OutputStreamオブジェクトを取得する。
 * 取得したOutputStreamオブジェクトに対し、データを書き込む。
 * OutputStreamオブジェクトのclose()を呼び、処理の終了を伝える。

//emlist[内部ストレージにファイルを作成する]{
public class MainActivity extends ActionBarActivity {
    // 中略
    
    void internalSaveClicked() {
        OutputStream out = null;
        OutputStreamWriter writer = null;
        BufferedWriter bw = null;
        try {
            out = openFileOutput("myText.txt", Context.MODE_PRIVATE);
            writer = new OutputStreamWriter(out);
            bw = new BufferedWriter(writer);

            String text = mEdit.getText().toString();
            bw.write(text);

            Toast.makeText(this, R.string.save_done, Toast.LENGTH_SHORT).show();

        } catch (IOException e) {
            Log.e("Internal", "IO Exception " + e.getMessage(), e);
        } finally {
            try {
                if (bw != null) { bw.close(); }
                if (writer != null) { writer.close(); }
                if (out != null) { out.close(); }
            } catch (IOException e) {
                Log.e("Internal", "IO Exception " + e.getMessage(), e);
            }
        }
    }
}
//}

まず、openFileOutput()を呼び、OutputStreamオブジェクトを取得します。第1引数にはファイル名を指定します。第2引数にはContext.MODE_PRIVATEを指定します。

続いて、OutputStreamWriterオブジェクトとBufferedWriterオブジェクトを生成します。OutputStreamオブジェクトを用いて直接データを書き込んでもよいのですが、OutputStreamクラスには文字列を効率的に書き込むためのメソッドが用意されていないので、このようにBufferedWriterオブジェクトを用意します。

BufferedWriterオブジェクトの生成まで完了したら、後はwrite()でストリームに文字列を書き込みます。

ファイルのオープンや書き込みはIOExceptionが発生することがあります。そのため、処理全体をtry-catchで囲みます。また、処理途中でIOExceptionが発生した時も確実に各Streamをクローズする必要があるため、finallyの部分でclose()を呼びます。

=== 内部ストレージ内のファイルを読み込む

内部ストレージ内のファイルを読み込むには、次の3ステップを行います。

 * ContextのopenFileInput()を呼び、InputStreamオブジェクトを取得する。
 * 取得したInputStreamオブジェクトから、データを読み込む。
 * InputStreamオブジェクトのclose()を呼び、処理の終了を伝える。

//emlist[内部ストレージ内のファイルを読み込む]{
public class MainActivity extends ActionBarActivity {
    // 中略

    void internalLoadClicked() {
        InputStream in = null;
        InputStreamReader sr = null;
        BufferedReader br = null;
        try {
            in = openFileInput("myText.txt");
            sr = new InputStreamReader(in);
            br = new BufferedReader(sr);

            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = br.readLine()) != null) {
                sb.append(line).append("\n");
            }
            mEdit.setText(sb.toString());

            Toast.makeText(this, R.string.load_done, Toast.LENGTH_SHORT).show();
        } catch (IOException e) {
            Log.e("Internal", "IO Exception " + e.getMessage(), e);
        } finally {
            try {
                if (br != null) { br.close(); }
                if (sr != null) { sr.close(); }
                if (in != null) { in.close(); }
            } catch (IOException e) {
                Log.e("Internal", "IO Exception " + e.getMessage(), e);
            }
        }
    }    
}
//}

まず、openFileInput()を呼び、InputStreamオブジェクトを取得します。第1引数にはファイル名を指定します。

続いて、InputStreamReaderとBufferedReaderオブジェクトを生成します。ファイルの書き込みと同様、InputStreamクラスには文字列を効率的に読み込むためのメソッドが用意されていないので、BufferedReaderオブジェクトを用意します。

BufferedReaderオブジェクトの生成まで完了したら、readLine()を用いて1行ずつ読み込みます。

ファイルの書き込みと同様に、ファイルのオープンや読み込みはIOExceptionが発生することがあります。そのため、処理全体をtry-catchで囲みます。また、処理途中でIOExceptionが発生した時も確実に各Streamをクローズする必要があるため、finallyの部分でclose()を呼びます。

=== 外部ストレージにファイルを作成する

外部ストレージにファイルを作成するには、次の4ステップを行います。

 * Fileオブジェクトを作成する。
 * 作成したFileオブジェクトを元に、FileOutputStreamオブジェクトを作成する。
 * 作成したFileOutputStreamオブジェクトに対し、データを書き込む。
 * OutputStreamオブジェクトのclose()を呼び、処理の終了を伝える。

OutputStreamオブジェクトを作成した後の処理は内部ストレージへの書き込みと同様なので、ここではOutputStreamオブジェクトを作成するまでを解説します。

//emlist[外部ストレージにファイルを作成する]{
public class MainActivity extends ActionBarActivity {
    // 中略
    void externalSaveClicked() {
        OutputStream out = null;
        OutputStreamWriter writer = null;
        BufferedWriter bw = null;
        try {
            File foler = Environment.getExternalStoragePublicDirectory(
                "MyDocuments");
            if (!foler.exists()) {
                boolean result = foler.mkdir();
                if (!result) {
                    return;
                }
            }
            File file = new File(foler, "myText.txt");
            out = new FileOutputStream(file);

            // 以下、内部ストレージへの書き込みと同様
        }
    }
}
//}

まず、保存先のフォルダを表すオブジェクトをEnvironment.getExternalStoragePublicDirectory()で取得します。引数にはフォルダの種類を文字列で指定します。ここでは独自に決めたMyDocumentsという種類を指定していますが、システムで用意されている定数を指定することもできます。

 * Environment.DIRECTORY_ALARMS
 * Environment.DIRECTORY_PICTURES
 * Environment.DIRECTORY_MUSIC
 * Environment.DIRECTORY_MOVIES

次に、フォルダが実際に存在するかをexists()で確認します。もし存在しない場合はmkdir()でフォルダを作成します。

フォルダの作成まで完了したら、newで保存先となるFileオブジェクトを作成します。第1引数には先ほど作成したフォルダオブジェクトを指定します。第2引数にはファイル名を指定します。

ファイルオブジェクト作成後、newでFileOutputStreamオブジェクトを作成します。引数には保存先となるFileオブジェクトを指定します。

=== 外部ストレージ内のファイルを読み込む

外部ストレージ内のファイルを読み込むには、次の4ステップを行います。

 * Fileオブジェクトを作成する。
 * 作成したFileオブジェクトを元に、FileInputStreamオブジェクトを作成する。
 * 取得したFileInputStreamオブジェクトから、データを読み込む。
 * InputStreamオブジェクトのclose()を呼び、処理の終了を伝える。

InputStreamオブジェクトを作成した後の処理は内部ストレージ内の読み込みと同様なので、ここではInputStreamオブジェクトを作成するまでを解説します。

//emlist[外部ストレージ内のファイルを読み込む]{
public class MainActivity extends ActionBarActivity {
    // 中略
    
    void externalLoadClicked() {
        InputStream in = null;
        InputStreamReader sr = null;
        BufferedReader br = null;
        try {
            File foler = Environment.getExternalStoragePublicDirectory(
                "MyDocuments");
            File file = new File(foler, "myText.txt");
            in = new FileInputStream(file);
            /// 以下、内部ストレージ内のファイル読み込みと同様
        }
    }
}
//}

Fileオブジェクトの作成までは、外部ストレージにファイルを作成する時と同様です。Fileオブジェクトの作成が完了したら、newでFileInputStreamオブジェクトを作成します。引数には読み込み元を表すFileオブジェクトを指定します。

=== Permission

内部ストレージへのファイル読み書きは特別なPermissionは不要ですが、外部ストレージへのファイル読み書きは他アプリに影響が出るため、Permissionの追加が必要です。

開発したAndroidアプリが外部ストレージのファイルを読み込む場合はandroid.permission.READ_EXTERNAL_STORAGEを、外部ストレージのファイルに書き込む場合はandroid.permission.WRITE_EXTERNAL_STORAGEをAndroidManifest.xmlに追加します。

//emlist[Permissionの追加]{
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="jp.androidopentextbook.storage.filesample" >

    <!-- ファイルを読み込む場合 -->
    <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
    
    <!-- ファイルに書き込む場合 -->
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
    <!-- 中略 -->
</manifext>
//}

== まとめ

本章では、Androidアプリ開発で主に利用する3つのAPIの使い方を学びました。アプリが扱うデータの性質に応じて、適切な仕組みを選択できるようになりましょう。


