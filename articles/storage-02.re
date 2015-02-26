= ストレージ(2)

== 概要

== SQLite

SQLiteとは、Android標準で利用可能なデータベース管理システムのことです。ここでは、SQLiteの使い方を学びます。

=== リレーショナルデータベース

SharedPreferencesはKey-Value形式でデータを管理する仕組みでしたが、関係モデル（リレーショナルモデル）に基づいてデータを管理するシステムもあります。このシステムを「リレーショナルデータベースマネジメントシステム（RDBMS）」と呼び、SQLiteはこのRDBMSの一つです。RDBMSによって構築されるデータベースを「リレーショナルデータベース」と呼び、データベース内の関係（リレーション）は一般に「表（テーブル）」と呼ばれます。図xxxに、RDBMSとリレーショナルデータベース・テーブルの関係を示します。

=== テーブル・列・行

リレーショナルデータベースでは、データは図xxxのように表の形式で保存されています。この表のことを「テーブル」と呼びます。テーブルには名前があり、１つのデータベースに複数のテーブルを格納することができます。また、テーブルの「名前」や「年齢」に相当するものを「列」と呼びます。「列」には名前の他に、数値や文字列などの「型」の情報も持ちます。テーブル内のデータ１つ１つを「行」と呼びます。各データがどのようなフィールドで構成されているかは、「列」を見ればわかります。

=== SQLとは

Androidアプリ開発では、端末に対する命令などをJavaという言語で記述しました。これに対し、RDBMSに対する命令（問い合わせ）はSQLと呼ばれる言語で記述します。Android標準のSQLiteに対する問い合わせもSQLで記述します。

SQLの文法は、大きく3種類に分けられます。

 * データ定義言語(DDL)
 * データ操作言語(DML)
 * データ制御言語(DCL)

SQLについて詳しく説明すると、それだけで数百ページの教科書となってしまうので、本章ではAndroidでSQLiteを使うために最低限必要となる文法のみ解説します。

=== PCでSQLを実行する

Android実機にはアプリから利用するためのSQLiteソフトウェアが入っていますが、Android SDKにもSQLiteソフトウェアが入っています。これを利用して、次節から解説するSQLをPC上で実行することができます。

Windowsでは、<Android SDKのインストールフォルダ>¥platform-tools内に「sqlite3.exe」ファイルがあるので、これをダブルクリックで起動します。（@<img>{sqlite1}）

//image[sqlite1][platform-tools内のsqlite3.exe][scale=0.35]{
//}

Mac / Linuxでは、<Android SDKのインストールフォルダ>/platform-tools内にsqlite3ファイルがあるので、ターミナルで実行します。

sqlite3を起動すると、@<img>{sqlite2}のようなウィンドウが表示されます。終了する時は「.exit」を実行します。Connected to a transient in-memory databaseというメッセージの通り、起動直後はメモリ上のデータベースに接続されます。このデータベースは.exitなどで終了すると同時に消えてしまうので、気軽にSQLを実行して動作を確認したりできます。

//image[sqlite2][sqlite3.exeが起動したウィンドウ][scale=0.35]{
//}

sqlite3は、初期設定では結果の表示方法がやや不親切なので、次の2つのコマンドを実行しておきます。

//emlist[sqlite3の設定変更]{
.mode column
.header on
//}

//image[sqlite3][設定を変更する][scale=0.35]{
//}

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

sqlite3では、@<img>{sqlite4}のようにCREATE文を実行します。sqlite3では、セミコロンまでを1つのSQLと解釈して実行するため、途中で改行をいれても大丈夫です。途中で改行をいれた場合は、入力行の先頭がsqliteから...に変化します。

//image[sqlite4][CREATE文をsqlite3で実行する][scale=0.35]{
//}


=== 練習問題

@<table>{lesson2}のようなlectureテーブルを作成するSQLを書いてみましょう。列の型は次のようにします。

 * _id : INTEGER
 * date : INTEGER
 * title : TEXT

//table[lesson2][lectureテーブル]{
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

指定した条件を満たす行（データ）だけ取得したい場合は、FROM <テーブル名> の後にWHEREと条件（これをWHERE句と呼びます）を追加します。次は、年齢が20歳未満の行を取得するためのSELECT文です。

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

@<img>{sqlite5}に、sqlite3でINSERT文とSELECT文を実行した例を示します。

//image[sqlite5][INSERTとSELECT文をsqlite3で実行する][scale=0.35]{
//}

=== 行を更新する

テーブル内の行を更新するには、UPDATE文を使用します。

//emlist[UPDATE文]{
UPDATE <テーブル名> SET 列名1=値,列名2=値,... WHERE 条件
//}

WHERE句で指定した条件に一致した行の、指定した列を更新します。WHERE句を忘れると、テーブル内のすべての行を更新してしまうので注意します。次は、id=1の行の収入を300に更新する例です。

//emlist[UPDATE文の例]{
UPDATE worker SET earnings=300 WHERE id=1
//}

=== 行を削除する

テーブル内の行を削除するには、DELETE文を使用します。

//emlist[DELETE文]{
DELETE FROM <テーブル名>  WHERE 条件
//}

WHERE句で指定した条件に一致した行を削除します。WHERE句を忘れると、テーブル内のすべての行を削除してしまうので特に注意します。次は、id=2の行を削除する例です。

//emlist[DELETE文の例]{
DELETE FROM worker WHERE id=2
//}

@<img>{sqlite6}に、sqlite3でUPDATE文とDELETE文を実行した例を示します。DELETE文を実行した後、再度SELECT文で行を取得しようとすると、結果が0件であることがわかります。

//image[sqlite6][UPDATEとDELETE文をsqlite3で実行する][scale=0.35]{
//}

=== Androidで使う

SQLに慣れてきたところで、AndroidアプリでSQLiteを使ってみましょう。AndroidでSQLiteを使うには、準備として次の2ステップを行います。

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

SQLiteOpenHelperには2つの抽象メソッドが定義されています。MyHelperクラスはこの2つのメソッドをOverrideしなければなりません。

 * public void onCreate(SQLiteDatabase db)
 * public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion)

onCreate()は、データベースのファイルを作る必要が生じた時に呼ばれます。onUpgrade()は、データベースのバージョンに変化が起きた時（たとえばアプリのバージョンアップなど）に呼ばれます。

onCreate()が呼ばれた時に、データベースの初期化としてテーブルを作成するSQLを実行します。SQLを実行するには、onCreate()の引数で渡されるSQLiteDatabaseオブジェクトのexecSQL()を呼びます。

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

memoテーブルの主キーをINTEGER型の_idというフィールドに指定しています。Androidでは、主キーがINTEGER型の_idであるテーブルに対して特定の機能を提供するAPIが存在するため、特に理由が無い場合は主キーを_idにしておきます。

=== MyHelperオブジェクトを作る

MyHelperクラスの定義ができたら、次にオブジェクトを作ります。ここでは、ActivityのonCreate()内で生成し、フィールドにセットしておきます。

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

 * memo列：入力された値
 * create_time列とupdate_time列：現在時刻

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

SQLのWHERE句で条件を指定して、特定の行だけ取得するには、第3引数にWHEREの内容を、第4引数には第3引数の?の部分に入れる値を指定します。文章で説明するとイメージしにくいので、_idが指定したものと一致する行だけ取得する例で説明します。

//emlist[WHERE句で条件を指定する]{
// idは引数で渡された値とします。
String where = MyHelper.Columns._ID + "=?";
String[] args = { String.valueOf(id) };
Cursor cursor = db.query(MyHelper.TABLE_NAME, null, where, args, null, null,
        MyHelper.Columns.CREATE_TIME + " ASC");
//}

ここでは、第3引数は"_id=?"という文字列になっています。=の右辺はユーザーの操作によって実行時に変化するので、?を指定します。そして、第4引数で?の部分に入れる値を指定しています。第3引数が"height > ? AND earnings > ?"のように、?を複数含む場合は、第4引数は?の数と同じ長さの配列にします。

勘がいい方は、「なぜ第3引数を"_id=" + idのようにしないんだろう？」と思うかもしれません。なぜこのように?で場所を指定し、第4引数で値を指定するかは後ほど紹介します。

query()の結果はCursorオブジェクトで返されます。Cursorオブジェクトには検索結果の行と、読み込み中の行の位置が格納されています。query()直後は読み込み中の行の位置が不定なので、moveToFirst()を呼び、先頭に移動させます。検索結果が0件の場合は、moveToFirst()がfalseを返すので、そこで処理を終了します。

結果が1件以上あった場合、次にCursorオブジェクトのgetColumnIndex()で、指定した列名に対するインデックス（位置）を列毎に取得します。これは、Cursorオブジェクトから読み込み中の行の、指定した列のデータを読み込む際、列名ではなく位置を指定するためです。

列名に対する位置が取得できたら、いよいよ各行のデータを読み込みます。moveToFirst()で読み込み位置が最初の行に移動しているので、do-whileループを使用します@<fn>{cursor-next}。ここでは、1行分のデータを表すMemoオブジェクトを作り、その中に読み込んだデータを格納するようにしています。

//footnote[cursor-next][Cursorクラスには「次の要素があるか」を判定するメソッドが無いため、このようにdo-whileループを用いるしかありません。]

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
 
SQLのUPDATE文で説明しましたが、行の更新はWHERE句で更新する行を指定します。WHERE部分の指定はupdate()の第3引数と第4で行います。指定方法はquery()の時と同様で、第3引数で?を含む条件を記述し、第4引数で?にいれる値を指定します。update()は呼ぶと、更新に成功した行数を返します。

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

=== なぜWHERE句に?を使用するか

query()やupdate()などは、SQLiteDatabaseクラスの内部でSQLを組み立てた上で実行されます。この時、WHERE句で指定した条件はSQL組み立て時にそのまま使用されます。もし、WHERE句に?が含まれていた場合は、SQLとして意味が変わらないよう、順に値が割り当てられます。

では、WHERE句の条件に?を使用せず、"name=" + nameのように文字列を連結したものを渡した場合を考えてみます。変数nameはString型として、"fkm"が入っていた場合、最終的に次のようなSQLが実行されるでしょう。

//emlist[nameにfkmが入っていた場合]{
SELECT * FROM user WHERE name=fkm
//}

もし、変数nameに"a OR 1=1"が入っていた場合、次のようなSQLが実行されてしまいます。

//emlist[nameにa OR 1=1が入っていた場合]{
SELECT * FROM user WHERE name=a OR 1=1
//}

これは、「userテーブル内で、nameがaと等しいか、1=1が真となる行をすべて取得しなNさい」という意味になります。この場合、1=1は常に真となるので、userテーブル内のすべての行が取得できてしまいます。

?を含む文字列を条件として指定し、その次の引数で?に割り当てる値を指定した場合、次のようなSQLが実行され、「全件取得してしまう」のような意図しない動作を防ぐことができます。

//emlist[条件に?を用いた場合]{
SELECT * FROM user WHERE name='a OR 1=1'
//}

このように、WHERE句の指定に?を使用せず、入力値を連結したものを使用した場合、値によって意図していないSQLが実行されてしまうことがあります。上記のnameのような変数にSQLの動作を変える文字列を入れることで、不正にデータを取得したり、データベースを破壊したりすることを「SQLインジェクション」と呼びます。

=== 練習問題

@<img>{lesson2-1}のようなレイアウトを作成し、入力されたToDoをSQLiteに保存するミニアプリを作ってみましょう。

 * 「保存」ボタンを押すと、入力されたToDoをtodoテーブルにinsertする。
 * 「取得」ボタンを押すと、todoテーブルの内容を全件取得し、TextViewに1行ずつ表示する。

//image[lesson2-1][ToDoを保存するアプリ][scale=0.35]{
//}

テーブル定義などはこれまでの内容を参考にして、自分で決めてみましょう。

== まとめ

本章では、Androidアプリ開発で主に利用する3つのAPIの使い方を学びました。アプリが扱うデータの性質に応じて、適切な仕組みを選択できるようになりましょう。


