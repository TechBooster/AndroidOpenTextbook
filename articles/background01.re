= Androidアプリのバックグラウンド

本章では、Androidアプリケーションの非同期処理、バックグラウンド処理を解説します。
UIスレッドで時間のかかる処理を行うと、その処理が終わるまでユーザの操作を受け付けなくなります。Androidには時間のかかる処理、たとえばデータ通信やストレージアクセスなどを非同期で行う仕組みがあります。
また常駐型アプリケーションではServiceを使い、バックグラウンドで処理を行えます。
ここではアプリケーションを作るために、Androidの持つ非同期通信、バックグラウンド処理の概要を理解したあと、AsyncTaskやServiceなど実際のAPIについて学んでいきましょう。

ゴールは、非同期処理、バックグラウンド処理について独力で最適な実装を選択できるようになることです@<fn>{最適解}。
ひとくちに最適な手法といっても、アプリケーションの仕様や開発者の実力、プラットフォームの互換性など多くの要因に左右され、解はひとつではありません。誰かが「この手法が最も良かった」と言ったからといって自分にとっても最適であるか、わからないためです。各手法の特徴を学び、設計にあった取捨選択を目指します。

//footnote[最適解][ソフトウェア開発において共通した要素があり、プログラミングでの定石をデザインパターンとよびます。開発における設計ノウハウをまとめたものとしては、書籍『オブジェクト指向における再利用のためのデザインパターン』が特に有名です]

== 非同期処理とバックグラウンド
はじめに、本章で取り扱う機能について概要を紹介します。

本章では、Androidのアプリケーション開発の現場でよく使われる非同期処理、バックグラウンド処理を取り扱います。

 * AsyncTask、AsyncTaskLoader：もっともポピュラーな非同期処理
 * Thread、Handler：スレッドとハンドラーによるスレッド間通信
 * Service、IntentService：常駐型のバックグラウンド処理

これらの非同期処理を理解するには、アプリがどのように動作しているか背景を知ることが重要です。
まずは、Androidアプリケーションのスレッドモデルについて学んでいきましょう

=== フォアグラウンドとバックグラウンド
フォアグラウンドとは、ユーザーインターフェイス（UI）や操作できる画面や処理、状態を指します。
バックグラウンドとは、ユーザーにみせるUIの裏側で動作し、ユーザーに意識させずに行われる処理、状態です。バックグラウンドではUIを持たない性質上、ユーザーとの対話なしに行えるタスク（処理）に向いています。

AndroidではActivityがフォアグラウンド処理を、Serviceがバックグラウンド処理を担当します。バックグラウンド処理を持つAndroidアプリケーションとしては、Musicアプリなどが代表的です。

現在のコンピュータの多くは複数の作業を切り替えて実行するマルチタスク環境を持っており、それはAndroidも同様です。
コンピュータのCPUやI/O（ファイル入出力）など限りあるリソースを効率的に利用するために、マルチタスク環境では待ち時間の合間に他のタスクを実行しています。
一般的にフォアグラウンド処理はUIを担当するため、バックグラウンド処理より優先度が高く設定されています。


=== プロセスとスレッド
Androidアプリケーションではプロセスはアプリケーションごとの実行単位、実行環境です。基本的に1つのアプリケーションに対して1つのプロセスが割りあてられます。
アプリを実行するために必要なリソース（メモリやCPUなど）は、プロセスごとに管理されています。
またプロセスは新しいプロセス（子プロセス）を生成できます。子プロセスは、生成元となったプロセス（親プロセス）とは異なるリソースを利用します。
このため、複数のプロセスで同じメモリ空間を読み書きするといったリソースの共有はできません。

プログラムが動く仕組みを理解するうえで重要な考え方です。

スレッドとは処理の単位のことです。プロセスAndroidのActivityに関する処理は通常mainスレッドで動作しています。
onCreateやonPauseメソッド、各UIパーツ（ボタン、Viewなど）の描画処理はmainスレッドで動いています。


====[column] デーモンスレッドとユーザースレッド
スレッドの種類は2つに分類されます。デーモンスレッドとは、プログラム終了時にスレッドの実行終了を待ちません。
プログラム終了のタイミングでデーモンスレッドの処理は中断され、終わることになります。生成したスレッドで終了処理を意識しないで済むので使い勝手がよい側面があります。

ユーザースレッドは、デーモンスレッドの反対でプログラムを終わるときに、スレッドの実行終了を待ちます。プログラムを終了しようとしても、
ユーザースレッドの処理が終わるまで終了できません（ユーザースレッドが生き残っている間は、プログラム実行状態です）。
さきほど説明にでてきた、UIスレッドはユーザースレッドです。プログラム終了にあたっては、すべてのユーザースレッドできちんと終了処理がハンドリングできていないといけません。
利点として、処理が中断してしまう恐れはないので紛失できない大事な処理などに適しています。
====[/column]


=== 同期と非同期処理
同期と非同期は、複数の処理（スレッド）を同時に行う上で重要な考え方です。複数のスレッドを並行的に処理することをマルチスレッドと呼びます。同期処理とは同一スレッド上での動作を指します。また複数のスレッドが処理を待ち合わせることを同期する、と表現します。

非同期処理では、複数のスレッドが阻害し合うことなく、独立して処理を行います。UIスレッドで行うには時間がかかりすぎる場合、たとえばネットワーク通信、ディスクアクセス、画像処理などは、積極的に非同期化すべきです。本来の役割であるUI描画をブロックしてしまい、画面が固まったようにみえるためです。


=== UIスレッドをブロックしない

Androidではユーザーの操作は、UIスレッドで処理されます@<fn>{single}。ボタンやテキストなどUIコンポーネントのインスタンスは、すべてUIスレッドで生成、操作しています。たとえばボタンを押したときに発生するonClick、画面をタッチするときに発生するonTouchEventでは、UIスレッドで実行しています。

//footnote[single][シングルスレッドモデル]

これらのメソッドで時間のかかる処理を行った場合、UIスレッドの他の処理はブロックされ、見た目に変化がなく固まった状態となります。
一般的には、無応答時間（処理時間）が100ms〜200msを超えると応答性が悪いと感じるため、
処理時間は非同期化を検討する指標のひとつといえるでしょう。

Androidアプリ開発では、原則として次の2つを守ってください。

 * UIスレッドで時間のかかる処理をしない
 * UIスレッド以外からUIコンポーネントを操作しない

特にアプリケーションが5秒以内に応答しなかった場合を、ANR（Application Not Responding）と呼び、次のようなダイアログが表示されます（@<img>{001_ANR}）。

//image[001_ANR][ANR（Application Not Responding）ダイアログ]{
//}

ANRは、アプリケーションの異常を検出してユーザーが操作に困るような事態を防ぐ機構です。Android OSは、アプリケーションの挙動を常時監視しています。
５秒以上反応がないということは、アプリそのものが異常な状態（フリーズ、ハングアップ）ですので、このような状況は開発者として絶対に避けるべきです。

=== ANRを発生させる要因

では、ANRが起きる、というのはどういう状況なのでしょうか。ここではサンプルコードを使って実際にANRを発生させてみましょう。次の３つは、時間がかかる処理のなかでも代表的な処理です。

 * CPUによる演算
 * ストレージアクセス
 * ネットワーク通信

順番にどのようなコードがANRを引き起こすのか確認してみましょう。

はじめのCPUによる演算とは、目に見えて処理時間がかかるほどの膨大な計算量が要因です（@<list>{loop}）。

//list[loop][計算量の多い処理]{
    int i,j,sum=0;
    for (j = 0; j < 1024*1024; j++) {
        for (i = 0; i < 1024*1024; i++) {
            sum++;
        }
    }
//}

サンプルコードではループ回数が大きく、非常に時間がかかっています。サンプルのような簡単なソースコードでは、時間がかかることも一目瞭然です。一般に、複雑なコードになればなるほど処理時間は、延びていきます。

アプリケーション開発において、すべての処理について理解し、把握していることが望ましいですが、実際には用意されたAPIを使ったり、ライブラリを使ったり、と多くの要素が絡み合っています。開発者の意図どおり、遅滞無く実行されているかは、処理時間を計測していくことでのみ確認できます。ANRの予防には計測が重要です。

つぎに、ストレージアクセスを見てみましょう。ストレージは設定値の保持や画像の一時保存などで利用されています。Android端末内のメモリストレージにデータを読み書きする場合、UIスレッドで処理しないほうがよいケースがあります（@<list>{read}）。

//list[read][ストレージアクセスの例]{
    InputStream in;
    String lineBuffer;

    in = openFileInput("bigdata.txt");
    BufferedReader reader= new BufferedReader(new InputStreamReader(in,"UTF-8"));
    while( (lineBuffer = reader.readLine()) != null ){
        Log.d("FileAccess",lineBuffer);
    }
//}

サンプルコードではデータを読み込もうとしています。一般に書込みは読込みより時間がかかり、ストレージアクセス開始から終了まで操作ができなくなります。
ごくわずかな時間で処理が完了する場合はスレッドを分けるべきでしょうか。応答速度の観点からみるとUIスレッドで処理しても良いと言えます@<fn>{異論}。

//footnote[異論][もちろん他スレッドで実行するほうが好ましいです。しかし、処理が複雑になると不具合が混入する可能性も増えます。分かりにくいソースコードになってしまうことも本意ではありません]

最後に、ネットワーク通信です。AndroidではUIスレッドでネットワーク通信を行った場合、エラーが発生します。

//list[network][ネットワーク通信]{
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        String url="http://localhost:8888/test";
        HttpClient httpClient = new DefaultHttpClient();
        HttpPost post = new HttpPost(url);
 
        ArrayList <NameValuePair> params = new ArrayList <NameValuePair>();
        params.add( new BasicNameValuePair("content", "send message"));
 
        HttpResponse res = null;
 
        try {
            post.setEntity(new UrlEncodedFormEntity(params, "utf-8"));
            res = httpClient.execute(post);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
//}
ネットワーク通信は、非常に時間がかかるためで、UIスレッドでの実行が禁止されています。

//cmd{
E/AndroidRuntime(10396): java.lang.RuntimeException:
  Unable to start activity ComponentInfo{com.example.
  background.sample00_anr/com.example.background.sample00_anr.
  MainActivity}: android.os.NetworkOnMainThreadException
//}
Logcatを確認するとNetworkOnMainThreadExceptionエラーが発生していることがわかります。

=== Androidアプリはどうやって同時に動作しているのか？

Androidでは複数のアプリケーションを同時に実行できます。原則としてActivityは1つしか許可されませんが、バックグラウンドでServiceが動作しています。
本文中の言葉で言い換えると、Androidはマルチプロセス動作のための機構を持っているわけです。
そのための機構はActivityManagerと呼ばれ、ユーザーへのインタラクションを管理しています。

ActivityManagerは、アプリケーションを管理するため、次のようなことを行っています。

 * ライフサイクルに応じてActivity／Serviceを管理する
 * 優先度に応じてプロセスを制御する
 * 未使用のプロセスを終了させてメモリ空間を確保する

利用者がストレスを感じないように調停し、場合によっては未使用のプロセス（アプリ）を終了します。たとえば利用中のアプリがより多くのメモリを要求した場合、
バックグラウンドに存在しているアプリを終了し、利用できるメモリを増やします。

優先度についても良好な応答性が維持できるように制御されています。
フォアグラウンドのActivityは、ユーザーへのインタラクションを提供しているため、最も優先度が高いプロセスとして制御します。
またバックグラウンドにあるServiceもメモリに余裕がある限り動作し続けられます。言い換えるとメモリがひっ迫した場合は、Serviceであっても終了させられます。

おおまかに優先度は次のとおりです。

 * フォアグラウンドのActivity
 * フォアグラウンドのService@<fn>{startForeground}
 * バックグラウンドのService
 * バックグラウンドのActivity

//footnote[startForeground][ServiceクラスのstartForegroundメソッドをつかい、通知バーに常駐することでフォアグラウンド動作が可能]

システムによる終了は低メモリ時に行われ、外部要因に依存します。これはOSが高負荷状態であるなど機種の状態や、他のアプリケーションの影響を受けることを示唆しています。
フレームワークの挙動を理解してアプリの設計段階から考慮しておくべきでしょう。

== 基本的な非同期処理
Androidフレームワークには、非同期処理のための機能が用意されています。ここでは良く用いられる非同期通信の方法について概要を解説します。実装、用法については後述の節を参考にしてください。

 * AsyncTask

一回限りの非同期処理に向いています。Androidフレームワークの非同期用クラスです。UIスレッドからしか生成できません。非同期処理には、スレッドの生成や破棄といった作業は不可欠ですが、煩雑です。AsyncTaskクラスは、このようなスレッド管理をフレームワーク内部で完結しています。

 * AsyncTaskLoader

複数回繰り返される非同期処理に向いています。AsyncTaskクラスを効率的に扱うためのLoaderクラスです。Loaderとは、AsyncTaskを読み込む（ロードする）ためのクラスです。高速化やリソースの効率化の仕組みとしてキャッシュを持っています。複数回繰り返す処理に向いています。UIスレッド以外からも生成できます。

 * Thread

Javaで提供されているクラスです。もっとも基本的なクラスで、スレッドの生成、スリープ、実行など基本的な操作ができます。ただし、Androidでは描画はUIスレッドでしか行えないため、処理結果をUIに反映する場合はHandlerを理解して利用しなければいけません。自由度が高い一方、実装コストも高くなります。

 * ExecutorService

Javaで提供されているクラスです。ExecutorServiceの実装は目的に応じて複数用意されており、単一のスレッドでの順次実行、複数のスレッドを使い分けられます。処理内容、CPUやメモリなどのリソースに応じて、どの実装（提供されたクラス）を使うか、取捨選択が必要です。

 * Handler

Handlerクラスは、Looperと連携して処理順序のスケジューリングや、処理を別スレッドで実行するための機能を持っています。
特にUIスレッドで描画処理を実行するために利用します。

 * Service

バックグラウンド動作のためのクラスです。音楽再生のようにバックグラウンドで継続して処理を行うために利用します。
スレッドとは異なります。

 * IntentService

 IntentServiceはアプリケーションの動作状況に依存しないで、自分の仕事がなくなるまでバックグラウンドで処理を行います。AsyncTaskと似ていますがアプリケーションがフォアグラウンドでなくなっても中断されない利点があります。


=== 良い実装のための指標
良い設計の第一歩は、各手法の特徴を理解しておくことです。めやすとしては次の通りです。

 * １回限りの処理、または繰り返しの少ない処理：AsyncTask
 * リスト表示するなど頻出する繰り返し処理：AsyncTaskLoader
 * 実行完了の保証が欲しい処理：IntentService
 * アプリ終了後もバックグラウンド動作したい処理：Service

プログラミングの目的は、さまざまなので、すべてのケースに当てはまるわけではありませんが、基本として理解しておくと応用の幅がひろがります。


== AsyncTask

AsyncTaskは非同期処理のための便利なヘルパークラスです。AsyncTaskクラス内部では、
非同期処理の為にThreadとHendlerが使われていますが、クラス内で隠蔽されており意識する必要はありません。

AsyncTaskクラスは、非同期処理を４つのステップに区切っています。実行前、実行開始、実行中、実行後です。それぞれonPreExecuteメソッド、doInBackgroundメソッド、onProgressUpdateメソッド、onPostExecuteメソッドが担当します（@<img>{002_AsyncTask}）。

//image[002_AsyncTask][AsyncTaskの処理シーケンス]{
//}

AsyncTaskクラスを使う際には次の特徴に注意してください。

 * UIスレッドでインスタンスを生成する
 * 実行済みのインスタンスは再利用できない
 * onPostExecuteメソッドはUIスレッドで実行される

AsyncTaskクラス最大の利点は簡単に非同期処理できるところです。そのための制約としていくつか制限があり、そのうちの1つがUIスレッドでインスタンスを生成することです。もうひとつは実行済みのインスタンスは再利用できません。つまり、１度実行したAsyncTaskは再度実行できません。破棄するのみです。

再利用性が低い設計がになっていることからも、一度限りの処理に向いている（そのために作られた）といえます。
そしてもっとも嬉しい点は、onPostExecuteメソッドはUIスレッドで実行されることでしょう。

通常ならUIコンポーネントを操作するためには、Handlerを使ってUIスレッドで処理するコードが必要です。
AsyncTaskならそのような面倒なコードは不要というわけです。

#@# ジェネリクスの話

=== AsyncTaskを使わない場合

ここでは非同期処理が必要な計算量の多い例として、画像処理を扱います。
Play Storeにもトイカメラ風の写真を撮るアプリなど面白い効果をもったカメラアプリがいくつも公開されていますが、
今回はシンプルに用意されている画像をカラーからモノクロに変換するアプリを作ってみましょう（@<img>{003_Sample01}）。

//image[003_Sample01][AsyncTaskサンプルアプリ]{
//}

画面下の開始ボタンを押すと画像をモノクロに変換する処理を行います。

まずはレイアウトを作成します（@<list>{01}）。

//list[01][activity_main.xml]{

<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    ...省略...
    tools:context=".MainActivity" >

    <ImageView
        android:id="@+id/imageView"
        android:layout_width="200dp"
        android:layout_height="200dp"
        android:layout_alignParentTop="true"
        android:layout_centerHorizontal="true"
        android:src="@drawable/ic_launcher_big" />

    <Button
        android:id="@+id/execButton"
        android:layout_below="@+id/imageView"
        ...省略...
        android:text="開始" />

    <Button
        android:id="@+id/countButton"
        android:layout_below="@+id/execButton"
        ...省略...
        android:text="カウントアップ" />

</RelativeLayout>
//}

はじめは、AsyncTaskを使わずにプログラミングしてみます。これは、計算量の多い処理があるとUIスレッドが止まり、
ユーザーが操作できないことを確認するためです。操作できないことを体験するために、２番目のボタンも用意します。
このボタンを押すと表示されているラベル（数字）がカウントアップしていくものです（@<list>{02}）。

//list[02][MainActivity.java]{
public class MainActivity extends Activity {

    private ImageView mImageView;
    private Bitmap mBitmap;
    private Button mCountButton;
    private Integer mCount = 0;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        // イメージの準備
        mBitmap = BitmapFactory.decodeResource(
                getResources(), R.drawable.ic_launcher_big);

        // 変換前のイメージを表示
        mImageView = (ImageView)findViewById(R.id.imageView);
        mImageView.setImageBitmap(mBitmap);

        // 押されるたびにカウントアップするボタン
        mCountButton =  (Button)findViewById(R.id.countButton);
        mCountButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                mCount++;
                ((Button)view).setText(mCount.toString());
            }
        });
        ...省略...
    }
}
//}


2番目のボタンであるカウントボタンを配置し、引き続き開始ボタンの処理を記述します（@<list>{03}）。

//list[03][MainActivity.java]{

    ...省略...
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        ...省略...
        
        // 同期処理の開始
        Button execButton = (Button)findViewById(R.id.execButton);
        execButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                // モノクロにする処理
                Bitmap out = mBitmap.copy(Bitmap.Config.ARGB_8888, true);
 
                int width = out.getWidth();
                int height = out.getHeight();
                int totalPixcel = width * height;
 
                int i, j;
                for (j = 0; j < height; j++) {
                    for (i = 0; i < width; i++) {
                        int pixelColor = out.getPixel(i, j);
                        int y = (int) (0.299 * Color.red(pixelColor) +
                                0.587 * Color.green(pixelColor) +
                                0.114 * Color.blue(pixelColor));
                        out.setPixel(i, j, Color.rgb(y, y, y));
                    }
                }
 
                // 変換が終わったので表示する
                mImageView.setImageBitmap(out);
            }
        });
    }
}

//}

アプリを実行して、開始ボタンとカウントボタンを押してみましょう。しばらく待っていると画像が、カラーからモノクロへ変換されます。
すべてUIスレッドで処理しているため、開始ボタンを押したあとはカウントボタンが操作できず、最悪の場合、ANRに陥ります@<fn>{連打}。

//footnote[連打][ANRを確認するために、カウントボタンを連続で押してみてください]

=== AsyncTaskを使う場合

前述したサンプルコードを変更して、画像処理部分を分離し、非同期化します。
このサンプルコードでは画像処理という、比較的わかりやすい例を取り扱っています。実際のアプリ開発においては、
重い処理を探し出して非同期化することになります。
サンプルは処理を非同期化するスタンダードな手法として覚えておくとよいでしょう。

AsyncTaskの主なメソッドは次の通りです。

 * onPreExecute()：事前に準備する内容を記述する
 * doInBackground(Params...)：バックグラウンドで実行する
 * onProgressUpdate(Progress...)：進捗状況をUIスレッドで表示する
 * onPostExecute(Result)：バックグラウンド処理が完了し、UIスレッドに反映する

各メソッドのParams、Progress、Resultは引数となるクラスの例です。実際には必要に応じて指定します（@<list>{031}）。


//list[031][MonochromeTask.java]
public class MonochromeTask extends AsyncTask<Bitmap, Integer, Bitmap> {
    private ImageView mImageView;
 
    public MonochromeTask(ImageView imageView) {
        super();
        mImageView = imageView;
    }
 
    @Override
    protected Bitmap doInBackground(Bitmap... bitmap) {
        // 非同期で処理する
        Bitmap out = bitmap[0].copy(Bitmap.Config.ARGB_8888, true);

        int width = out.getWidth();
        int height = out.getHeight();
        int totalPixcel = width * height;

        int i, j;
        for (j = 0; j < height; j++) {
            for (i = 0; i < width; i++) {
                int pixelColor = out.getPixel(i, j);
                // モノクロ化
                int y = (int) (0.299 * Color.red(pixelColor) +
                        0.587 * Color.green(pixelColor) +
                        0.114 * Color.blue(pixelColor));
                out.setPixel(i, j, Color.rgb(y, y, y));
            }
        }
        return out;
    }
 
    @Override
    protected void onPostExecute(Bitmap result) {
        // 実行後にImageViewへ反映
        mImageView.setImageBitmap(result);
    }
}
//}

ここで注目すべきは 「extends AsyncTask<Bitmap, Integer, Bitmap>」です。
前述の引数ではParams、Progress、Resultが出てきましたが、ここで引数の型を指定しています。

１番目のParamsはバックグラウンド処理を実行する時に与えるexecuteメソッドの引数の型です。
２番目のProgressは進捗状況を表示するonProgressUpdateメソッドの引数の型です。
最後のResultはバックグラウンド処理の後に受け取るonPostExecuteメソッドの引数の型です。

今回はビットマップ画像を与えてモノクロに変換されたビットマップを受け取ります。

Activity側で必要な処理は、MonochromeTaskクラス（Asynctaskのサブクラス）のインスタンスを生成することと、
非同期処理を開始するために、executeメソッドを呼び出すことです。

execButtonのOnClickListenerを次の通り書き換えてみましょう（@<list>{04}）。

//list[04][MainActivity.java]{
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        ...省略...
        Button execButton = (Button)findViewById(R.id.execButton);
        execButton.setOnClickListener(new OnClickListener() {
            
            MonochromeTask task =
                    new MonochromeTask(mImageView);
            
            @Override
            public void onClick(View view) {
                // モノクロにする処理
                task.execute(mBitmap);
            }
        });
        ...省略...
    }
//}

AsyncTaskを使わない場合と違い、UIスレッドをブロックしません。
この状態でカウントボタンを押すと、問題なくカウントアップされていきます。

=== 進捗を表示する
画像処理に関わらず、重たい処理を行う時に見た目が固まるのは良くありません。
何がおきているか、利用者が把握できず、不安になるためです。
Windowsなどでファイルをコピーするときに出るプログレスバーがユーザビリティの良い例ですね。
大量のファイルをコピーする時などプログレスバーが出ていないと本当にコピーしているのか不安になってします。

最後にプログレスバーによる進捗表示を追加してみましょう（@<list>{05}）。

//list[05][MainActivity.java]{
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        ...省略...
        Button execButton = (Button)findViewById(R.id.execButton);
        execButton.setOnClickListener(new OnClickListener() {
            
            MonochromeTask task =
                    new MonochromeTask(getParent(),mImageView);
            
            @Override
            public void onClick(View view) {
                // モノクロにする処理
                task.execute(mBitmap);
            }
        });
        ...省略...
    }
//}

MonochromeTask（AsyncTask）のコンストラクタにgetParentメソッドで取得したActivityを渡します。

MonochromeTaskでは、進捗を表示するためのプログレスバーをonPreExecuteメソッドで準備します。
また、非同期処理を行うdoInBackgroundメソッドから進捗を随時アップデートするonProgressUpdateメソッドを呼び出します（@<list>{06}）。

//list[06][MonochromeTask.java]

public class MonochromeTask extends AsyncTask<Bitmap, Integer, Bitmap> {
    private ImageView mImageView;
    private ProgressDialog mDialog;
    private Context mContext;
 
    public MonochromeTask(Context context, ImageView imageView) {
        super();
        mContext = context;
        mImageView = imageView;
    }
    @Override
    protected void onPreExecute() {
        //処理前にプログレスバーを準備
        mDialog = new ProgressDialog(mContext);
        mDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
        mDialog.setIndeterminate(false);
        mDialog.setMax(100); //100%表記
        mDialog.show();
    }
    
    @Override
    protected Bitmap doInBackground(Bitmap... bitmap) {
        // 非同期で処理する
        Bitmap out = bitmap[0].copy(Bitmap.Config.ARGB_8888, true);

        int width = out.getWidth();
        int height = out.getHeight();
        int totalPixcel = width * height;

        int i, j;
        for (j = 0; j < height; j++) {
            for (i = 0; i < width; i++) {
                int pixelColor = out.getPixel(i, j);
                // モノクロ化
                int y = (int) (0.299 * Color.red(pixelColor) +
                        0.587 * Color.green(pixelColor) +
                        0.114 * Color.blue(pixelColor));
                out.setPixel(i, j, Color.rgb(y, y, y));
            }
            float percent = ((i + j*height) /
                    (float)totalPixcel ) * 100;
            onProgressUpdate((int)percent);
        }
        return out;
    }
    @Override
    protected void onProgressUpdate(Integer... progress) {
        // 実行中
        mDialog.setProgress(progress[0]);
    }
    
    @Override
    protected void onPostExecute(Bitmap result) {
        // 実行後にImageViewへ反映
        mDialog.dismiss();
        mImageView.setImageBitmap(result);
    }
}
//}

主な変更点は次の通りです。進捗を表示するためにコンストラクタの引数にContextを追加しました。
準備としてonPreExecuteメソッドでプログレスダイアログ（ProgressDialog）を用意しています。
進捗を更新するためにonProgressUpdateメソッドを追加し、doInBackgroundメソッドからonProgressUpdateメソッドを呼び出しています。
処理が完了したら、onPostExecuteメソッドでダイアログを非表示にして終了です。

AsyncTaskは非同期処理を行えるもっとも手軽な手段です。
しかし、ProgressDialogのようにUIコンポーネントを使う場合は、思いの外、複雑になってしまいます。
Contextを持つ必要があることからも、Activityと密結合せざるを得ません。

サンプルコードでは、UIコンポーネントのProgressDialogをつかって進捗表示する例を挙げました。
非同期処理をしているにも関わらず、ほかのUIを操作できなくなってしまいました。アプリとして、あまり嬉しい挙動ではないでしょう。

実際、AsyncTaskが向いている処理は、ファイルのダウンロードや、データのネットワーク送信など進捗表示を必要としない
バックグラウンド動作です@<fn>{fireandforget}。

//footnote[fireandforget][Fire and Forget。このような非同期処理の特性を指して撃ちっ放しと呼ばれます]

== AsyncTaskLoader

AsyncTaskLoaderは、複数回繰り返される非同期処理に向いています。AsyncTaskクラスを置き換えるものではありません。
効率的に扱うためのLoaderクラスです。Loaderとは、名前の通り読み込む（ロードする）機能を意味します。
ActivityやFragmentといったライフサイクルを考慮して作られている点も大きな利点です。

AsyncTaskLoaderを利用する際には、次の特徴に留意するとよいでしょう。

 * LoaderCallbacksをつかってUIを制御するコードが分離できる
 * 高速化やリソースの効率化の仕組みとしてキャッシュを持つ
 * UIスレッド以外からも生成できる


AsyncTaskと比較しやすいように、画像をモノクロに変換するサンプルコードを見てみましょう（@<list>{07}）。

//list[07][MonochromeTaskLoader.java]{
    public class MonochromeTaskLoader extends AsyncTaskLoader<Bitmap> {
    Bitmap mBitmap;
    
    public MonochromeTaskLoader(Context context, Bitmap image) {
        super(context);
        // 処理対象のビットマップ
        mBitmap = image;
    }
    
    @Override
    public Bitmap loadInBackground() {
        // 非同期で処理する
        Bitmap out = mBitmap.copy(Bitmap.Config.ARGB_8888, true);

        int width = out.getWidth();
        int height = out.getHeight();

        int i, j;
        for (j = 0; j < height; j++) {
            for (i = 0; i < width; i++) {
                int pixelColor = out.getPixel(i, j);
                // モノクロ化
                int y = (int) (0.299 * Color.red(pixelColor) +
                        0.587 * Color.green(pixelColor) +
                        0.114 * Color.blue(pixelColor));
                out.setPixel(i, j, Color.rgb(y, y, y));
            }
        }
        return out;
    }
    
    @Override
    protected void onStartLoading() {
        forceLoad();
    }
}
//}

loadInBackgroundメソッドで画像のモノクロ変換を行っています。UIコンポーネントに関わる処理は後述のLoaderCallbacksが担当するため、非同期に実行される処理のみ記述しています。シンプルな構成です。

onStartLoadingメソッドはLoaderの開始時に呼び出されます。サンプルでは強制的に読み込みを始めていますが、条件を設けることも可能です。Loaderはresume時など独自の判断に基づき、onStartLoadingを呼び出しています（onStartLoadingは直接呼び出してはいけません。かわりにstartLoadingが用意されています）。

またサンプルコードでは利用していませんが、AsyncTaskLoaderには読み込み完了時に呼び出されるonStopLoadingメソッドやキャンセル用のcancelLoadメソッドが用意されています。

ActivityからAsyncTaskLoaderを呼び出す際はLoaderManagerのinitLoaderメソッドを利用します（@<list>{08}）。

//list[08][MainActivity.java]{
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        ...省略...

        mExecButton =  (Button)findViewById(R.id.execButton);
        mExecButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                initAsyncTaskLoader(R.drawable.ic_launcher_big);
            }
        });
        
    }
    
    public void initAsyncTaskLoader(int resId) {
        Bundle args = new Bundle();
        args.putInt("drawableId", resId);
        // Loaderを初期化する
        getLoaderManager().initLoader(0, args, this);
    }
//}

LoaderManagerクラスのinitLoaderメソッドは３つの引数をもち、ローダーID、ローダーに渡す引数、
読み込みイベントを受け取るLoaderCallbacksの３つを指定します。
サンプルコードではIDは0番、引数にモノクロに変換するdrawableIDを指定しています。
LoaderCallbacksは、ここではActivityに実装するため、thisと自分自身を指定します。

LoaderCallbacksとはローダーの読み込みを行うためのインターフェースです。
クライアント（呼び出し元であるActivity）がローダーの管理者であるLoaderManagerとやりとりする仕組みで、次のメソッドの実装が必要です。

 * onCreateLoader：新しくLoaderが生成されるとき
 * onLoadFinished：Loaderでの非同期処理が終了するとき
 * onLoaderReset：Loaderがリセットされるとき
 
次のサンプルコードでLoaderCallbacksインターフェイスの実装をみてみましょう（@<list>{09}）。

//list[09][MainActivity.java]{
public class MainActivity extends Activity implements LoaderCallbacks<Bitmap>{
    ...省略...
    // LoaderCallbacks
    @Override
    public Loader<Bitmap> onCreateLoader(int id, Bundle args) {
        if(args != null) {
            int resId = args.getInt("drawableId");
            Bitmap bitmap = BitmapFactory
                    .decodeResource(getResources(), resId);
            // 非同期処理を開始する
            return new MonochromeTaskLoader(this, bitmap);
        }
        return null;
    }

    @Override
    public void onLoadFinished(Loader<Bitmap> loader, Bitmap bitmap) {
        // 画像を更新する
        mImageView.setImageBitmap(bitmap);
        mImageView.invalidate();
    }

    @Override
    public void onLoaderReset(Loader<Bitmap> loader) {
        // リセットされた場合の処理
    }
}
//}
onCreateLoaderメソッドでは、LoaderのIDと引数を受け取ります。
第２引数のBundleからdrawableIDを取得してMonochromeTaskLoaderを生成しています。

また引数は、LoaderManagerクラスのinitLoaderメソッドの第１引数、第２引数と同等です。
サンプルコードでは利用していませんがIDをつかって処理を変えたり、Loaderを切り替えたり、処理条件を変更できます。

onLoadFinishedメソッドでは、モノクロ変換が完了した画像をImageViewに設定しています。
第１引数でLoaderを受け取ります。ローダー側で必要な処理があればこのメソッド内で実行可能です@<fn>{この型はいったい}。

//footnote[この型はいったい][ここではLoader<Bitmap>という型になっていますが、MonochromeTaskLoaderがAsyncTaskLoader<Bitmap>を継承していることを思い出してみましょう。ここで<Bitmap>はジェネリクス（型変数）で型を指定します。AsyncTaskLoaderはさらにLoader<D>を継承しているため、上位のクラスで表記できるわけです]

onLoaderResetメソッドはサンプルコードでは利用していません。リセット時の挙動（UIコンポーネントの操作など）を記述するとよいでしょう。

AsyncTaskLoaderでは、LoaderManager、Loader、LoaderCallbacksが登場するようになりました。AsyncTaskのみに比べると複雑な構成です。
しかし、サンプルコードでは、LoaderCallbacksにUIコンポーネントの操作がまとめられるなど、それぞれの責務が明確となっています。
AsyncTaskのサンプルコードと、見比べてもすっきりしたと感じるでしょう（実はコード量は増えているのですが、それでも読みやすく感じるのであれば良い構成といえます）。

それぞれの役割としては次のように理解するとよいでしょう。

 * LoaderManager：ActivityやFragmentに対応したライフサイクル管理
 * LoaderCallbacks：LoaderManagerとLoaderが連携するためのインターフェイス
 * Loader：非同期処理を行う
 
ここまででAsyncTaskLoaderを解説しました。Loaderという仕組みは非同期処理を汎用的に使うために作られたといえます@<fn>{Content}。
LoaderManagerにより管理されるため、独自の実装（たとえばActivityやFragmentなどのライフサイクルから切り離した独立性の高い設計）では、
利用しにくい、という事実も押さえておきたい点です。

//footnote[Content][ContentResolverへアクセスするためのCursorLoaderもAsyncTaskLoaderを継承しています]

== ThreadとHandler

ここまで何度も繰り返してきましたが、Androidアプリケーションはシングルスレッドモデルを採用しています。
いかにUIスレッドをブロックしないか、が開発のポイントです。AsyncTask、AsyncTaskLoaderは、非同期処理のための便利なクラスですが、
これらのクラスも大本をたどればThreadクラスとHandlerクラスで作られています。これらは非同期処理の基本ともいえる機能で、
マルチスレッドで処理を行うには避けて通れません。

Threadは、Javaで提供されているクラスです。もっとも基本的なクラスで、スレッドの生成、スリープ、実行など基本的な操作ができます。
処理の実行単位といって差し支えありません。自由度が高い一方、実装コストも高くなります。

Handler（ハンドラー）は、Looper（ルーパー）と連携して動作し、
キューを使った処理順序のスケジューリングや、処理を別スレッドで実行するための機能を持っています。

 * UIスレッドなど指定のスレッドのLooperと連携する
 * 任意のスレッドにメッセージ、処理を送信できる

===[column] キュー

キュー（待ち行列）は、データ構造の一種です。データを先入れ先出し（FIFO：First In First Out）で保持します。先に入れたデータから取り出します。キューに従ってデータ、メッセージを管理することで順番通りに処理できる仕組みです。

===[/column]

ここでは、別スレッドで1秒ごとカウントアップして、結果をTextViewに反映するコードを考えてみます（@<img>{004_Sample03}）。

//image[004_Sample03][１秒ごとカウントアップするサンプルアプリ]{
//}

実際にサンプルコードで確認してみましょう（@<list>{10}）。
Handlerでは処理とメッセージが送れますが、最初に解説する方法は、Threadで非同期処理を行い、処理の完了時にメッセージを通知する手法です。

//list[10][MainActivity.java]{
public class MainActivity extends Activity implements Runnable{

    private Thread mThread;
    private Handler mHandler;
    private TextView mTextView;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mTextView = (TextView)findViewById(R.id.textView);
        
        mHandler = new Handler(){
            //メッセージ受信
            public void handleMessage(android.os.Message message) {
                //メッセージの表示
                String text = (String) message.obj;
                mTextView.setText( text );
                //メッセージの種類に応じてswitch文で制御すれば
                //イベント制御に利用可能
            };
        };
    }
  ...省略...
}
//}

onCreateメソッド内でHandlerのインスタンスを生成します。Handlerは、生成されたスレッドのLooperに紐づけられます。
このサンプルでは暗黙的にUIスレッドで処理するHandlerを作成しています。
handleMessageメソッドでは、TextViewを操作していますが、UIスレッド上で実行するのでシングルスレッドモデルに違反しません。

実際に非同期処理を行うThreadは、次の通りです。まずは開始と終了処理のみを確認してみましょう（@<list>{11}）。

//list[11][MainActivity.java]{
public class MainActivity extends Activity implements Runnable{

    private Thread mThread;
    private Handler mHandler;
    private TextView mTextView;
    
    ...省略...
    @Override
    public void onResume(){
        super.onResume();

        mThread = new Thread(this);
        //スレッド処理を開始
        if(mThread != null ){
            mThread.start();
        }
    }

    @Override
    public void onPause(){
        super.onPause();
        //スレッドを削除
        mThread = null;
    }
    ...省略...
}
//}

onResumeメソッドでスレッドを開始し、onPauseメソッドで処理を終了しています。アプリケーションがフォアグラウンドにいる間は
動作し続けます。onResumeメソッド内、Threadのインスタンスを生成するタイミングでコンストラクタにthis（Activity）を指定していますが、
これは、MainActivityのクラス宣言にある通り、Activityに実装したRunnableインターフェイスを与えています（後述）。

スレッドで処理している部分は次の通りです（@<list>{12}）。

//list[12][MainActivity.java]{
public class MainActivity extends Activity implements Runnable{

    private Thread mThread;
    private Handler mHandler;
    private TextView mTextView;
    ...省略...
    
    //スレッドによる更新処理
    public void run() {
        long time = System.currentTimeMillis();
        long count = 0;

        while (mThread != null) {
            long now = System.currentTimeMillis();
            if(now - time > 1000){
                //Message msg = new Message();
                //Message msg = Message.obtain();
                Message msg = mHandler.obtainMessage();
                msg.obj = new String("ループが"+ count + "回終了しました");

                //ハンドラへのメッセージ送信
                mHandler.sendMessage(msg);

                //スレッドの利用変数を初期化
                time = now;
                count++;
            }
        }
    }
}
//}

Runnableインターフェイスのrunメソッドでシステムの時間を数えています。
ここでは無限ループとならないようにmThread変数の有無をwhileの条件としています。
１秒以上の差があればHandlerに対してメッセージ（TextViewに表示したい文字列）を送信しています。

メッセージの送受信ではなく、UIスレッドで動作させたい処理そのものを送る場合は、次のサンプルコードのようになります（@<list>{13}）。

//list[13][MainActivity.java]{
public class MainActivity extends Activity implements Runnable{

    private Thread mThread;
    private Handler mHandler;
    private TextView mTextView;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mTextView = (TextView)findViewById(R.id.textView);
        
        mHandler = new Handler();
    }
    ...省略...
    //スレッドによる更新処理
    public void run() {
        long time = System.currentTimeMillis();
        long count = 0;

        while (mThread != null) {
            long now = System.currentTimeMillis();
            if(now - time > 1000){
                final String text = new String("ループが"+ 
                        count + "回終了しました");
                mHandler.post(new Runnable() {

                    @Override
                    public void run() {
                        // UIスレッドで動作する
                        mTextView.setText( text );
                    }
                });

                //スレッドの利用変数を初期化
                time = now;
                count++;
            }
        }
    }
}
//}

非同期処理を行っているThreadからHandlerクラスのpostメソッドを使ってUIスレッドで実行したい処理を記述しています。
AsyncTaskのonPostExecuteのようですね。AsyncTaskとは違い、何度でも呼び出すことができます。

postメソッドの引数はRunnableです。Threadを生成した際も引数がRunnableであったことを覚えているかもしれません。
Runnableは、Javaでの実行単位です。

HandlerとThreadが連携することでメッセージの送受信や非同期処理、結果の通知が可能です。
Handlerクラスの実装をまとめると次の通りです。

 * sendMessageメソッド：メッセージを送信する
 * handleMessageメソッド：メッセージを受信する
 * postメソッド：Runnableをつかって処理を送信する

いずれもキューで管理されるため先入れ先出しです。呼ばれた順番に処理されます。
同じHandlerへの操作であれば、あとからpostした処理が先に実行されるようなことはありません@<fn>{Activity}。

Threadクラスでは、次の点に注意して利用しましょう。

 * Runnableで非同期化したい処理を記述する
 * 処理の回数ごとThreadを生成するとコストが高い
 * startメソッドで開始できるが停止時のstopは非推奨

Threadを停止するstopメソッドは構造上の欠陥から非推奨です。interruptメソッドによる割り込みなどが適切ですが、より深いプログラミングの知識が必要です。サンプルコードでは複雑化を避けるためmThreadインスタンスで代用しています。どのように終了を定義しているか、再度読み直してみてください。

また単純な非同期処理であればThreadを使う必要はありません。Threadの生成はコストが高く、頻繁に繰り返すとパフォーマンスに悪影響を及ぼします。

//footnote[Activity][実はActivityにはrunOnUiThreadメソッドというpostメソッドを暗黙的に使う便利な機能が備わっています。Handlerを使わなくてもよくなるわけではありませんが、覚えておいて損はありません]

== Service

Androidでバックグラウンド動作する、常駐型アプリケーションを作成する場合には、Serviceの知識が欠かせません。ここではServiceのライフサイクル（@<img>{005_ServiceLifeCycle}）を解説したあと、実際の利用上の注意点に触れていきます。

//image[005_ServiceLifeCycle][Serviceのライフサイクル]{
//}

Serviceのライフサイクルは、次の３つの状態があります。

 * onCreate：生成時
 * onStartCommand：開始時
 * onDestroy：破棄時
 
Serviceの実行方法によってライフサイクルが異なり、それぞれContextクラスのstartServiceメソッドを使った場合とContextクラス@<fn>{いいかえ}のbindServiceメソッドを使った場合です。それぞれの実行方法の特徴は次の通りです。

//footnote[いいかえ][ActivityはContextを継承しているため、Activityも同じメソッドを持っています]

 * startServiceメソッドとstopServiceメソッド
 ** Service全般として実行中はServiceからActivityへIntentの発行が可能
 ** サービス起動後はActivityからServiceを制御する経路がない
 ** Serviceの生存期間はActivityに依存しない。明示的にstopServiceが呼ばれるまで動き続ける。

 * bindService/unbindService
 ** バインド(bind)という仕組みを使い、ActvitiyとServiceでコネクションを確立する
 ** バインドを使うことでActivityからServiceを制御できる
 ** Serviceの生存期間はコネクションに依存。コネクションが切断されるとServiceは終了する。
 
はじめに一般的なサービスのライフサイクル、startServiceによるService起動を解説します。

=== startServiceによるService起動
ContextクラスのstartServiceメソッドによる実行の場合、ServiceのライフサイクルはonCreate、onStartCommand、onDestroyの３つのコールバックメソッドが呼ばれます。

 * public void onCreate()
 * public void onDestroy()
 * public int onStartCommand(Intent intent, int flags, int startId)
 
onCreateメソッドはServiceのインスタンス生成時（複数回startServiceメソッドを実行した場合、初回のみ）呼び出されます。
onStartCommandメソッドではstartServiceで送られたIntentを受けとります。
stopServiceメソッドによるサービス終了のタイミングでonDestroyメソッドが呼ばれます。
ActivityでstopServiceを呼ばず、アプリケーションを終了した場合はServiceは終了せず、バックグラウンドで動き続けます。

サンプルとして次のようにServiceを実行するボタンを準備し（@<img>{006_Sample04}）、サービスの動作を確認してみましょう（@<list>{14}）。

//image[006_Sample04][Serviceを実行するサンプルアプリ]{
//}

//list[14][MyService.java]{
public class MyService extends Service {
 
    static final String TAG="LocalService";
 
    @Override
    public void onCreate() {
        Log.i(TAG, "onCreate");
        Toast.makeText(this, "MyService#onCreate", Toast.LENGTH_SHORT).show();
    }
 
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i(TAG, "onStartCommand Received start id " + startId + ": " + intent);
        Toast.makeText(this, "MyService#onStartCommand", Toast.LENGTH_SHORT).show();
        //明示的にサービスの起動、停止が決められる場合の返り値
        return START_STICKY;
    }
 
    @Override
    public void onDestroy() {
        Log.i(TAG, "onDestroy");
        Toast.makeText(this, "MyService#onDestroy", Toast.LENGTH_SHORT).show();
    }
}
//}
コールバックメソッドのみ実装した簡単なServiceです。Serviceを使う際は、AndroidManifest.xmlにservice要素を追加します（@<list>{15}）。

//list[15][AndroidManifest.xml]{
    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
        <activity
            android:name="MainActivity"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
        <service android:name="MyService" />
    </application>
//}

Activityでは"Start Service"ボタンで開始、"Stop Service"ボタンで終了します（@<list>{16}）。

//list[16][MainActivity.java]{
public class MainActivity extends Activity {
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
 
        Button btn = (Button) findViewById(R.id.startButton);
        btn.setOnClickListener(btnListener);//リスナの登録
 
        btn  = (Button) findViewById(R.id.stopButton);
        btn.setOnClickListener(btnListener);//リスナの登録
    }
 
    private OnClickListener btnListener = new OnClickListener() {
        public void onClick(View v) {
 
            switch(v.getId()){
 
            case R.id.startButton://startServiceでサービスを起動
                startService(new Intent(MainActivity.this, MyService.class));
                break;
            case R.id.stopButton://stopServiceでサービスの終了
                stopService(new Intent(MainActivity.this, MyService.class));
                break;
            }
        }
    };
}
//}

OnClickListenerでサービスの起動、終了を行います。
startServiceメソッド、stopServiceメソッドの引数にはIntentが必要です。
ここではIntent(Context, Class)コンストラクタでIntentを生成してServiceを呼び出しています。

"Start Service"、"Stop Service"ボタンの順番で押して、Serviceのライフサイクルを確認してみます。

//cmd{
INFO/LocalService(1619): onCreate
INFO/LocalService(1619): onStartCommand Received start id 1:
  Intent { cmp=jp.androidopentextbook.background.sample04/.MyService }
INFO/LocalService(1619): onDestroy
//}


"Start Service"、"Start Service"、"Stop Service"ボタンの順番で押すと、
onCreateメソッドの呼び出しが初回のみであることがわかります。

//cmd{
INFO/LocalService(1619): onCreate
INFO/LocalService(1619): onStartCommand Received start id 1:
  Intent { cmp=jp.androidopentextbook.background.sample04/.MyService }
INFO/LocalService(1619): onStartCommand Received start id 2:
  Intent { cmp=jp.androidopentextbook.background.sample04/.MyService }
INFO/LocalService(1619): onDestroy
//}

Activityと比べると単純なライフサイクルですが、Serviceの終了を忘れると常に動作しつづけます。
バックグラウンド動作している間は、通常より電池の消耗がはげしくなったり、動作が緩慢になったり、他のアプリにも影響を及ぼします。
目に見えない処理だからこそ、ライフサイクルを十分に意識しましょう。

=== bindServiceによるService起動

bind（バインド）という仕組みを使い、ActvitiyとServiceを接続するケースでは、ライフサイクルが多少異なります。
図にある通り、onStartCommandメソッドが呼び出されることがありません（@<img>{007_ServiceBindLifeCycle}）。

//image[007_ServiceBindLifeCycle][Serviceのライフサイクル（Bind時）]{
//}

通常のonCreate、onDestroyメソッドに加えて、バインド時は次のコールバックメソッドを利用します。

 * public abstract IBinder onBind (Intent intent)
 * public boolean onUnbind (Intent intent)
 * public void onRebind (Intent intent)

onBindはバインド（接続時）、Unbindはバインド解除（切断時）に呼び出されます。
onRebindメソッドは図中には現れていませんが、Unbind後に再接続する場合、onRebindメソッドを利用することができます。
使う際はonUnbindメソッドの返り値をtrueに設定します。


==== バインドするサービスを作る

Bind、Unbindに対応したServiceを作ってみましょう。バインドするためにはIBinderクラスを実装します。バインダーと呼ばれる仕組みで、
ServiceとActivityを繋ぐインターフェイスを提供します。サンプルコードは次の通りです（@<list>{17}）。

//list[17][MyService.java]{
public class MyService extends Service {
    // onCreate、onStartCommand、onDestroyはstartServiceと同じなので省略
    /*
     * 以下はBind時に追加するコード
     * */
    //サービスに接続するためのBinder
    public class MyServiceLocalBinder extends Binder {
        //サービスの取得
        MyService getService() {
            return MyService.this;
        }
    }
    //Binderの生成
    private final IBinder mBinder = new MyServiceLocalBinder();
 
    @Override
    public IBinder onBind(Intent intent) {
        Toast.makeText(this, "MyService#onBind"+ ": " + intent, Toast.LENGTH_SHORT).show();
        Log.i(TAG, "onBind" + ": " + intent);
        return mBinder;
    }
 
    @Override
    public void onRebind(Intent intent){
        Toast.makeText(this, "MyService#onRebind"+ ": " + intent, Toast.LENGTH_SHORT).show();
        Log.i(TAG, "onRebind" + ": " + intent);
    }
 
    @Override
    public boolean onUnbind(Intent intent){
        Toast.makeText(this, "MyService#onUnbind"+ ": " + intent, Toast.LENGTH_SHORT).show();
        Log.i(TAG, "onUnbind" + ": " + intent);
 
        //onUnbindをreturn trueでoverrideすると次回バインド時にonRebildが呼ばれる
        return true;
    }
}

//}
ServiceをバインドするためにIBinderを生成して、onBindメソッドの返り値として設定しています。
onBind、onRebind、onUnbindメソッドそれぞれで動作を確認しやすいようにToastを表示します。

Serviceを起動するActivity側も変更します。ボタンを追加してOnClickListenerにbindService、unbindServiceメソッドを追加します（@<list>{18}）。

//list[18][MainActivity.java]{
...省略...
private OnClickListener btnListener = new OnClickListener() {
    public void onClick(View v) {
 
        switch(v.getId()){
 
        case R.id.StartButton://startServiceでサービス起動
            startService(new Intent(MainActivity.this, MyService.class));
            break;
 
        case R.id.StopButton://stopServiceでサービス終了
            stopService(new Intent(MainActivity.this, MyService.class));
            break;
 
        case R.id.bindButton://サービスをバインドするメソッドを呼び出す
            doBindService();
            break;

        case R.id.unbindButton://アンバインドするメソッドを呼び出す
            doUnbindService();
            break;
 
        default:
            break;
        }
 
    }
};
...省略...
//}
doBindServiceメソッド、doUnbindServiceメソッドは処理が長くなるため、作成した独自のメソッドです。
この時点では何も処理しないメソッドとして追加してください。

====

startServiceメソッドの場合と異なり、バインドするためにはServiceConnectionクラスが必要です。
ServiceConnectionはその名前の通り、サービスと接続するための機能を担っています。
Activity側でServiceConnectionクラスのインスタンスを生成して、ServiceConnectionに必要なメソッドを用意します。

 * public void onServiceConnected(ComponentName className, IBinder service)
 * public void onServiceDisconnected(ComponentName className)
 
onServiceConnectedメソッドはActivityとServiceのコネクションが確立した際に呼び出されます。
引数のIBinder serviceで、サービスのバインダーを受け取れます。
バインダー経由でServiceのインスタンスを取得することが可能です。つまりActivityから直接、Service側のメソッド呼び出しができるようになります。

ServiceConnection#onServiceDisconnectedメソッドは、プロセスのクラッシュなど意図しないサービスの切断が発生した場合に利用されます（呼び出されること自体、あまり好ましい状況ではありません、@<list>{19}）。

//list[19][MainActivity.java]{
public class MainActivity extends Activity {

    //取得したServiceの保存
    private MyService mBoundService;
    private boolean mIsBound;

    private ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName className, IBinder service) {

            // サービスとの接続確立時に呼び出される
            Toast.makeText(MainActivity.this, "Activity:onServiceConnected",
                    Toast.LENGTH_SHORT).show();

            // サービスはIBinder経由でアクセス可能になる
            mBoundService = ((MyService.MyServiceLocalBinder) service).getService();
            // 必要であればmBoundServiceを使ってバインドしたサービスのメソッドを呼び出す
        }

        public void onServiceDisconnected(ComponentName className) {
            // サービスとの切断（異常系処理）
            // プロセスのクラッシュなど意図しないサービスの切断が発生した場合に呼ばれる。
            mBoundService = null;
            Toast.makeText(MainActivity.this, "Activity:onServiceDisconnected",
                    Toast.LENGTH_SHORT).show();
        }
    };

    void doBindService() {
        // サービスとの接続を確立する。明示的にServiceを指定
        // (特定のサービスを指定する必要がある。
        // 他のアプリケーションから知ることができない =ローカルサービス)
        bindService(new Intent(MainActivity.this,
                MyService.class), mConnection, Context.BIND_AUTO_CREATE);
        mIsBound = true;
    }

    void doUnbindService() {
        if (mIsBound) {
            // コネクションの解除
            unbindService(mConnection);
            mIsBound = false;
        }
    }
}
//}
サンプルコードのdoBindServiceメソッドはサービスにバインドするためのメソッドです。
doUnbindServiceメソッドはサービスをアンバインド（切断）するメソッドです。

bindServiceとunbindServiceメソッドどちらにも引数にServiceConnectionクラスを利用しています。
ServiceConnectionでは、サービスとの接続時、切断時の処理を記述しています。今回のサンプルではどちらも
Toastを使ってログを画面上に表示します。

最後にサンプルアプリを起動して、
Bind Serviceボタン、Unbind Serviceボタンの順番で押してみましょう。ログでライフサイクルを確認できます。

//cmd{
INFO/LocalService(1619): onCreate
INFO/LocalService(1619): onBind: Intent { 
  cmp=jp.androidopentextbook.background.sample04/.MyService }
INFO/LocalService(1619): onUnbind: Intent { 
  cmp=jp.androidopentextbook.background.sample04/.MyService }
INFO/LocalService(1619): onDestroy
//}

Serviceの利用では、startService、stopServiceメソッドを使う場合とbindService、unbindServiceメソッドの２つの方法があることを解説しました。
前者はServiceの生存期間がActivityに依存しないことから、音楽再生のようにバックグラウンドで常時動いてほしいものが適切です。
しかしながら起動後はServiceを細かく操作できないため、onStartCommandメソッドの引数をもとに制御することになります。

後者の生存期間は、コネクションに依存します。これはサーバーとの通信など利用者が多い機能（各Activityで共通化した処理）と相性が良いでしょう。
そのためのServiceConnectionが存在しており、Activityと密な連携が可能です。

どちらの場合においてもServiceは独自のライフサイクルをもって動作しています。ActivityやFragmentの制御とは切り離した設計が重要です。

== IntentService
IntentServiceはアプリケーションのバックグラウンドで処理を行うための方法です。名前の通り、Serviceを拡張していますので、基本的な扱い方はServiceと同様です。
フォアグラウンドの動作と関係なく（非同期で）動作できますが、そのために専用のサービスを用意すると煩雑になってしまいます。

IntentServiceを使えば、これらの処理を簡略化、簡単に実行することができます。AsynctaskもAndroidアプリケーションのUIスレッドを保護する手法の一つです。
動作は似ていますが、IntentServiceはアプリケーションの動作状況に依存しないで、自分の仕事がなくなるまでバックグラウンドで処理を行える利点があります。
アプリケーションがフォアグラウンド（前面）に位置していなくても、処理が中断されることがありません。またIntentServiceはAsyncTaskと異なり再利用（複数呼び出し）も可能です。
逆に処理中のプログレスバーを表示するなど逐次結果をUIコンポーネントを通じて反映する場合は、AsyncTaskのほうが簡単です。
IntentServiceとAsyncTaskは非同期処理の特性に応じて使い分けるべきでしょう。

ボタンを押すとIntentServiceを実行するサンプルコードで動作を見ていきましょう（@<img>{008_Sample05}）。

//image[008_Sample05][IntentServiceを実行するサンプルアプリ]{
//}

IntentServiceはサービスですので、最初にAndroidManifestに登録します（@<list>{20}）。

//list[20][AndroidManifest.xml]{
    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
        <activity
            android:name="jp.androidopentextbook.background.sample05.MainActivity"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />

                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
        <service android:name="MyIntentService"></service>
    </application>
//}
service要素にMyIntentServiceを追加します。

IntentServiceのソースコードは次の通りです。コンストラクタと非同期処理を行うonHandleIntentメソッドのみのシンプルな構成です（@<list>{21}）。

//list[21][MyIntentService.java]{
public class MyIntentService extends IntentService{
    
    public MyIntentService(String name) {
        super(name);
    }
 
    public MyIntentService(){
        // ActivityのstartService(intent);で呼び出されるコンストラクタはこちら
        super("MyIntentService");
    }
 
    @Override
    protected void onHandleIntent(Intent intent) {
        // 非同期処理を行うメソッド。タスクはonHandleIntentメソッド内で実行する
        Log.d("IntentService","onHandleIntent Start");
        Log.d("IntentService","intent msg:" + 
          intent.getStringExtra("IntentServiceCommand") );
    }
}
//}
IntentServiceのコンストラクタが２つありますが、動作には引数がないMyIntentService()コンストラクタが必要です。
IntentServiceが同時に処理できる作業は１つで、onHandleIntentメソッドが非同期処理を行うメソッドです。

IntentServiceの内部では、起動したあと自動的にワーカースレッド（仕事をするために用意された専用のスレッド）によりキューイングが行われています。
Serviceはワーカースレッドを通じて処理を逐次実行していきます。

サンプルコードを実行すると、次のログが表示されます。
//cmd{
DEBUG/IntentService(478): onHandleIntent Start
DEBUG/IntentService(478): intent msg:TestText
//}

IntentServiceは、Serviceとして動作することで確実にバックグラウンドで動作します。AsyncTask（またはThreadをつかった非同期処理）では、アプリケーションが非表示になる際に処理が中断される恐れがありました@<fn>{onPause}。確実に実行する作業

//footnote[onPause][そのため、onPuaseメソッドなどで停止処理を組み込んだり、ライフサイクルを考慮したAsyncTaskLoaderがあります]


#@# == プロセス間通信（AIDL）
#@# == Androidでの優先度制御
#@# == デバッグツール
#@# systraceは とTraceview、ストリクトモード
