= ストレージ(1)

本章では、アプリのデータを端末内に保存したり、読み込んだりする方法を学びます。。本節では、その中でもSharedPreferencesやファイルの扱い方を学びます。

== 本節で学べること

 * SharedPreferencesにデータを読み書きできるようになる。

アプリの設定情報など、簡単なデータの読み書きができるようになります。

 * ファイルの読み書きができるようになる。

画像など、やや大きめのデータをファイルに保存したり、ファイルから読み込んだりできるようになります。

== 本節のキーワード

 * SharedPreferences
 * Key-Value形式
 * File
 * ストリーム
 * 内部ストレージ
 * 外部ストレージ
 * READ_EXTERNAL_STORAGE
 * WRITE_EXTERNAL_STORAGE

== ストレージとは

ストレージとは、データを永続的に記録する装置です。PCではハードディスクやSSDがストレージに該当します。Android端末にもストレージはあり、アプリはAPIを介してストレージにデータを記録したりできます。この章では、Androidアプリ開発で主に用いられる３つのストレージAPIについて学びます。

== ストレージとメモリ

まず、データの記録について学びましょう。これまでのアプリ開発で使用した「変数」や「フィールド」は一時的なデータを記録するための仕組みです。変数やフィールドに保存されたデータはアプリ実行中、メモリ上に記録されるため、アプリを終了させたり端末の電源を切ったりすると、記録した内容は失われてしまいます。

これに対し、ストレージに記録されたデータはアプリを終了させたり、端末の電源を切っても失われることはありません（経年劣化などで物理的に壊れたりした場合は別ですが）。

== ファイル

ストレージを扱う時の基本単位となるデータのまとまりを「ファイル」と呼びます。画像データが保存されているファイルや、Javaソースコードが保存されているファイルなど、既になじみ深いものだと思います。PCのハードディスクやSSDなどへの読み書きがファイル単位で行われるのと同様に、Androidでもストレージへの読み書きはファイル単位で行われます。

===[column]ファイルシステム

ストレージへの読み書きはファイル単位で行われます。このファイル単位による読み書きを提供するシステムを「ファイルシステム」と呼び、OSによって提供されます。ファイルシステムはWindowsではNTFS、Linuxではext3やext4など、いろいろな種類があります。Androidでは2.3からext4が使用されています。

===[/column]

== Androidアプリ開発で主に利用するストレージAPI

Androidでは、ストレージに対してデータを作成したり、読み込んだりするためのAPIや仕組みがいくつか用意されています。代表的なものは次の3つです。

 * SharedPreferences
 * SQLiteDatabase
 * File

 本章では、この3つの代表的なAPIの使い方を学びます。
 
また、開発したアプリのデータを他アプリに提供したり、他アプリ（たとえば電話帳など）からデータを取得するための仕組みも用意されています。

 * ContentProvider
 * Storage Access Framework（Android 4.4で追加）

== SharedPreferences

SharedPreferencesとは、アプリの設定情報などを保存する仕組みです。SharedPreferencesは、データをKey-Value形式で保存します。Key-Value形式とは、データを「キー」と「値」のペアで保存する形式で、身近なものだと辞書がこれに該当します。

=== SharedPreferencesに保存可能なデータの種類

SharedPreferencesに値として保存可能なデータの種類は次の6種類です。

 * String
 * int
 * long
 * float
 * boolean
 * Set<String>

=== SharedPreferencesにデータを保存する

SharedPreferencesにデータを保存するには、次の4ステップを行います。

 * Context(Activity)のgetSharedPreferences()で、SharedPreferencesオブジェクトを取得する。
 * SharedPreferencesオブジェクトのedit()で、Editorオブジェクトを取得する。
 * Editorオブジェクトのputメソッドで、保存するデータをセットする。
 * apply()を呼び、変更を反映させる。

//emlist[SharedPreferencesにデータを保存する]{
private static final String KEY_NAME = "name";
private static final String KEY_AGE = "age";

SharedPreferences pref =
    getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
SharedPreferences.Editor edit = pref.edit();
edit.putString(KEY_NAME, name);
edit.putString(KEY_AGE, age);
edit.apply();
//}

SharedPreferencesオブジェクトを取得するには、Context(Activity)のgetSharedPreferences()を呼びます。第1引数には、自分で決めたSharedPreferences名を指定します。第2引数はContext.MODE_PRIVATEを指定します。

次に、SharedPreferencesオブジェクトのedit()で、データを書き込むためのオブジェクトを取得します。このオブジェクトには、putString()やputInt()など、値の種類に応じたメソッドが用意されています。ここでは"name"というキーに、入力された文字列を値として保存しています。保存したいデータが複数ある場合は、putXXX()をデータの数だけ呼びます。また、同じキーを指定した場合、値は上書きされます。

最後に、apply()を呼ぶことでデータの変更を反映させます。これを呼び忘れるとデータは保存されないので注意しましょう。

=== SharedPreferencesからデータを読み込む

SharedPreferencesからデータを読み込むには、次の2ステップを行います。

 * Context(Activity)のgetSharedPreferences()で、SharedPreferencesオブジェクトを取得する。
 * SharedPreferencesオブジェクトのgetメソッドで、保存したデータを取得する。
 
//emlist[SharedPreferencesからデータを読み込む]{
SharedPreferences pref =
    getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
String name = pref.getString(KEY_NAME, DEFAULT_NAME);
//}

SharedPreferencesオブジェクトの取得は書き込み時と同様です。値を読み込むには、値の型に応じたgetXXX()を呼びます。ここでは保存した名前を文字列として取得するため、SharedPreferencesオブジェクトのgetString()を呼んでいます。第1引数で指定したキーに対応する値が無かったり、型が違ったりした場合、getString()は第2引数で指定した値を代わりに返します。

=== SharedPreferencesが苦手とすること

SharedPreferencesはデータの読み書きが数行で実現できる反面、苦手とすることがいくつかあります。

 * データの検索には向かない

SharedPreferencesでデータを読み込むには、キーを事前に知っておく必要があります。なので、「年齢が20であるデータを全部取得したい」のような、検索を必要とするデータの保存には向きません。

 * 複数プロセスで読み書きすると予想外の挙動をすることがある

残念なことに、SharedPreferencesは複数プロセスから読み書きすると、「アプリで保存したはずの値がServiceで読み込めない」といった挙動をすることがあります。問題となるパターンは基礎の範囲を超えるため、ここでは紹介しません。ですが、「問題がある」ということは頭の片隅に置いておきましょう。

=== 練習問題

@<img>{lesson1-1}のように、TextView / EditText / Buttonを1つずつ配置してください。そして、次のような動作をするよう、SharedPreferencesを用いて処理を記述し動作確認しましょう。

 * Buttonをタップすると、EditTextに入力された内容をSharedPreferencesを用いて保存する。
 * アプリ起動時に、SharedPreferencesに値が保存されていれば@<img>{lesson1-2}のようにTextViewの文字列をセットする。

//image[lesson1-1][練習問題レイアウト][scale=0.35]{
//}

//image[lesson1-2][保存した名前を表示][scale=0.35]{
//}

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

ストリームとは、データの流れを扱う「もの」です。流れてきたデータを読み込むストリームをInputStreaemと呼び、データを流すためのストリームをOutputStreamと呼びます。（@<img>{streams}）

//image[streams][InputStreamとOutputStream][scale=0.15]{
//}

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

ファイルの書き込みと同様に、ファイルのオープンや読み込みはIOException@<fn>{ioexception}が発生することがあります。そのため、処理全体をtry-catchで囲みます。また、処理途中でIOExceptionが発生した時も確実に各Streamをクローズする必要があるため、finallyの部分でclose()を呼びます。

//footnote[ioexception][実行時、入出力に関するエラーが発生した時に投げられる例外です。例えば、存在しないファイルを読み込み用でオープンしようとした時に発生します。]

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

=== 練習問題

@<img>{lesson2-1}のような、入力内容をファイルに保存するメモ帳アプリを作成してみましょう。メモが他のアプリに読まれてしまうのを防ぐため、内部ストレージに保存するようにしてみましょう。

//image[lesson2-1][ファイルを使ったメモ帳][scale=0.45]{
//}